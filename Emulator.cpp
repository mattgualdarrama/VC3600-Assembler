#include "stdafx.h"
#include "Emulator.h"
#pragma once


/**/
/*

NAME

emulator::insertMemory - inserts data into the emulator's memory

SYNOPSIS

bool emulator::insertMemory(int a_location, int a_contents);
a_location	-> location in memory
a_contents	-> machine code stored there

DESCRIPTION

makes sure location is valid. Puts memory in memory location
and records the starting location for emulation.

RETURNS

True if memory inserted correctly. False otherwise.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

bool emulator::insertMemory(int a_location, int a_contents) {

	if (a_location > 10000) {
		cerr << "Error: location out of bounds." << endl;
		return false;
	}

	m_memory[a_location] = a_contents;
	if (start <= 0) {
		start = a_location;
	}

	return true;
}
/* bool emulator::insertMemory(int a_location, int a_contents); */

/**/
/*

NAME

emulator::runProgram - runs emulator

SYNOPSIS

bool emulator::runProgram();

DESCRIPTION

separates the op code and operand. Finds which operation
to perform through the op code. returns when Halt() is
called.

RETURNS

True if it completes. 

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
bool emulator::runProgram() {
	m_loc = start;

	for (;;) {
		
		// dividing by 10000 gets the first 2 digits
		instruction.first = m_memory[m_loc]/10000;
		
		// modding by 10000 gets the last 4 digits
		instruction.second = m_memory[m_loc] % 10000;

		// if assembler instruction, the last 4 digits are the content
		if (instruction.first == 0) {
			m_loc++;
			continue;
		}
	
		int cont = instruction.first;

		if (cont == 1) {
			Add();
		}
		else if (cont == 2) {
			Sub();
		}
		else if (cont == 3) {
			Mult();
		}
		else if (cont == 4) {
			Div();
		}
		else if (cont == 5) {
			Load();
		}
		else if (cont == 6) {
			Store();
		}
		else if (cont == 7) {
			Read();
		}
		else if (cont == 8) {
			Write();
		}
		else if (cont == 9) {
			B();
		}
		else if (cont == 10) {
			Bm();
		}
		else if (cont == 11) {
			Bz();
		}
		else if (cont == 12) {
			Bp();
		}
		else if (cont == 13) {
			Halt();
		}
		else {
			m_loc++;
		}

		if (isdone) {
			return true;
		}
	}
}
/* bool emulator::runProgram(); */

/**/
/*

NAME

emulator::Add - performs addition

SYNOPSIS

void emulator::Add();

DESCRIPTION

Adds what is in memory to the accumulator. Makes sure it 
doesn't go out of bounds.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Add() {
	int total = accumulator + m_memory[instruction.second];
	if (total > 99999 || total < -99999) {
		cerr << "Addition caused memory to go out of bounds." << endl;
		exit(1);
	} 
	accumulator = total;
	m_loc++;
}
/* void emulator::Add();*/

/**/
/*

NAME

emulator::Sub - performs subtraction

SYNOPSIS

void emulator::Sub();

DESCRIPTION

Subtracts what is in memory from the accumulator. Makes sure it
doesn't go out of bounds.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Sub() {
	int difference = accumulator - m_memory[instruction.second];
	if (difference < -99999 || difference > accumulator) {
		cerr << "Subtration caused memory to go out of bounds." << endl;
		exit(1);
	}
	accumulator = difference;
	m_loc++;
}
/* void emulator::Sub(); */

/**/
/*

NAME

emulator::Mult - performs multiplication

SYNOPSIS

void emulator::Mult();

DESCRIPTION

Multiplies what is in memory with the accumulator. Makes sure it
doesn't go out of bounds.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Mult() {
	int product = accumulator * m_memory[instruction.second];
	if (product > 99999 || product < -99999) {
		cerr << "Multiplication caused memory to go out of bounds." << endl;
		exit(1);
	}
	m_loc++;
}
/* void emulator::Mult(); */

/**/
/*

NAME

emulator::Div - performs division

SYNOPSIS

void emulator::Div();

DESCRIPTION

Divides the accumulator with what is in memory. Makes sure it
doesn't go out of bounds.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Div() {
	if (instruction.second == 0) {
		cerr << "Error: attempted to divide by zero." << endl;
		exit(1);
	}
	accumulator = accumulator / m_memory[instruction.second];
	m_loc++;
}
/* void emulator::Div(); */

/**/
/*

NAME

emulator::Load

SYNOPSIS

void emulator::Load();

DESCRIPTION

Loads what's in memory into the accumulator.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Load() {
	accumulator = m_memory[instruction.second];
	m_loc++;
}
/* void emulator::Load(); */

/**/
/*

NAME

emulator::Store 

SYNOPSIS

void emulator::Store();

DESCRIPTION

Stores what's in the accumulator into memory.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Store() {////////
	m_memory[instruction.second] = accumulator;
	m_loc++;
}
/* void emulator::Store(); */

/**/
/*

NAME

emulator::Read - reads from user

SYNOPSIS

void emulator::Read();

DESCRIPTION

Reads input from user. If invalid it asks for
input until it is valid. Stores in memory.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Read() {
	string input;
	cout << "?";
	cin >> input;

	if (input.size() > 6) {
		input = input.substr(0, 6);
	}

	for (int i = 0; i < input.size(); i++) {
		size_t pos;
		if ((pos = input.find_first_not_of("0123456789")) != string::npos) {
			if (input[pos] != '-') {
				cout << "Invalid input. Try again: " << endl;
				Read();
			}
		}
	}

	m_memory[instruction.second] = atoi(input.c_str());///////
	m_loc++;

}
/* void emulator::Read(); */

/**/
/*

NAME

emulator::Write 

SYNOPSIS

void emulator::Write();

DESCRIPTION

Writes what's in memory to the screen.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Write() {
	cout << m_memory[instruction.second] << endl;
	m_loc++;
}
/* void emulator::Write(); */

/**/
/*

NAME

emulator::B

SYNOPSIS

void emulator::B();

DESCRIPTION

Branches to the location in memory

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::B() {
	m_loc = instruction.second;
}
/* void emulator::B(); */

/**/
/*

NAME

emulator::B

SYNOPSIS

void emulator::Bm();

DESCRIPTION

branches if what is in the accumulator is negative.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Bm() {
	if (accumulator < 0) {
		m_loc = instruction.second;
	}
	else {
		m_loc++;
	}
}
/* void emulator::Bm(); */

/**/
/*

NAME

emulator::Bz

SYNOPSIS

void emulator::Bz();

DESCRIPTION

branches if what is in the accumulator is zero.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Bz() {
	if (accumulator == 0) {
		m_loc = instruction.second;
	}
	else {
		m_loc++;
	}
}
/* void emulator::Bz(); */

/**/
/*

NAME

emulator::Bp

SYNOPSIS

void emulator::Bp();

DESCRIPTION

branches if what is in the accumulator is positive.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Bp() {
	if (accumulator > 0) {
		m_loc = instruction.second;
	}
	else {
		m_loc++;
	}
}
/* void emulator::Bp(); */

/**/
/*

NAME

emulator::Halt

SYNOPSIS

void emulator::Halt();

DESCRIPTION

Flips isdone flag to true so the emulator knows the program
is done running and to terminate.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void emulator::Halt() {
	isdone = true;
}
/* void emulator::Halt(); */


