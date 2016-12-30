//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"


/**/
/*

NAME

Assembler::Assembler - constructor for the assembler

SYNOPSIS

Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv );
We are passing argc and argv to this constructor. See main program.

DESCRIPTION

This function is the constructor class for the assembler. Argv and argc is
passed to this constructor. The file is then opened and gives an error
if it cannot be opened.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}
/* Assembler::Assembler(int argc, char *argv[]); */


// Destructor currently does nothing.  You might need to add something as you develop this project.
Assembler::~Assembler( )
{
}
/* Assembler::~Assembler() */


/**/
/*

NAME

Assembler::PassI - finds labels in the code and stores in symbol table

SYNOPSIS

void Assembler::PassI();

DESCRIPTION

This function retrieves each line of code seperately and parses
the line. If a symbol is found it is added to the symbol table along with
its location. If END is reached, it returns.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Assembler::PassI( ) 
{

	// Tracks the location of the instructions to be generated.
    int loc = 0;       
	int lineCount = 0;

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string buff; 
        if( ! m_facc.GetNextLine( buff ) ) {
			
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

		lineCount++;

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( buff );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) {
			m_inst.SetSymTab(m_symtab.GetSymbolTable());
			return;
		}

		

		// If the instruction has a label, record it and its location in the
		// symbol table.
		// Labels can only be on machine language and assembler language
		// instructions.  So, skip other instruction types.
		// && st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr
		if (m_inst.isLabel() ) {
			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }
		
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
	
}
/* void Assembler::PassI(); */


/**/
/*

NAME

Assembler::PassII - 

SYNOPSIS

void Assembler::PassII();

DESCRIPTION

Retrieves each line of code and parses it. By using the line's 
instruction type it tries to translate the line of assembly 
language into machine code, which is stored in a vector pair.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Assembler::PassII() {

	int loc = 0;
	m_facc.rewind();
	string allErrors = "";

	for (int line = 0;; line++) {
		string error = "";
		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			error = "Error: Missing end statement.";
			cerr << error << endl;
			Errors::RecordError(error);
			m_inst.DisplayTranslation();
			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		m_inst.SetSymTab(m_symtab.GetSymbolTable());

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) {
			m_inst.AddTranslation(-1, 0, 0, m_inst.GetInstruction());
			
			// check if there are any lines after the end statement
			string line;
			while (m_facc.GetNextLine(line)) {
				istringstream iss(line);
				string character;
				while (iss >> character) {
					if (character != "\t" || character != "\n" || character != " ") {
						error = "Error: Instructions after end statement.";
						cerr << error << endl;
						Errors::RecordError(error);
					}
				}
			}
			// Break from the loop
			m_inst.DisplayTranslation();
			return;
		}
		else if (st == Instruction::ST_Comment) {
			m_inst.AddTranslation(-1, 0, 0, m_inst.GetInstruction());
		}
		else if (st == Instruction::ST_AssemblerInstr || st == Instruction :: ST_MachineLanguage) {

			// finds numeric version of opcode and operand if available
			m_inst.SetNumericOperand(m_inst.GetOperand(), m_symtab.GetSymbolTable());
			m_inst.SetNumericOpCode(m_inst.GetOpCode());

			m_inst.AddTranslation(loc, m_inst.GetNumOpCode(), m_inst.GetNumOperand(), m_inst.GetInstruction());
		}
		loc = m_inst.LocationNextInstruction(loc);
	}
}
/* void Assembler::PassII(); */


/**/
/*

NAME

Assembler::RunEmulator - runs the emulator

SYNOPSIS

void Assembler::RunEmulator();

DESCRIPTION

Makes sure there is no errors in code. Inserts memory
into emulator and then runs the emulator.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void Assembler::RunEmulator() {

	// makes sure there are no errors to mess up emulation
	if (Errors::getErrors().size() != 0) {
		cerr << "Can not run emulation - errors in code." << endl;
		exit(1);
	}

	vector<pair<int, string>> code = m_inst.GetTranslatedCode();

	// inserts machine code into emulator memory if possible
	for (int i = 0; i < code.size(); i++) {

		// get numerical values
		int location = code[i].first;
		int content = atoi(code[i].second.c_str());

		m_emul.insertMemory(location, content);
	}

	cout << "Results from emulating program: " << endl;
	cout << endl;

	m_emul.runProgram();

	cout << endl;
	cout << "End of emulation" << endl;
}
/* void Assembler::RunEmulator(); */