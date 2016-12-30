//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//

#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"
#pragma once


/**/
/*
NAME

Instruction::ParseInstruction - parses the string

SYNOPSIS

Instruction::InstructionType Instruction::ParseInstruction(string a_buff);
a_buff	-> the string being parsed

DESCRIPTION

This function parses the lines on the first pass and logs them, searching for the label, 
op code, and the operand.  It removes comments from the line and checks if the whole line 
is a comment. Then each line is parsed into a vector and stored in the variables m_OpCode,
m_Operand, and m_Label, and m_NumOpCode. Also finds the instrustion type and checks for some
errors.

RETURNS

Instruction type.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

Instruction::InstructionType Instruction::ParseInstruction(string a_buff) 
{
	m_instruction = a_buff;
	m_Label = "";
	m_OpCode = "";
	m_Operand = "";

	// remove any blank space in the front or back of statement
	int strBegin = a_buff.find_first_not_of(" \t");
	int strEnd = a_buff.find_last_not_of(" \t");

	// if blank line, treat as a comment so nothing happens to it
	if (strBegin == -1 && strEnd == -1) {
		m_type = ST_Comment;
		return m_type;
	}
	int strRange = strEnd - strBegin + 1;
	a_buff = a_buff.substr(strBegin, strRange);

	// If line is a comment, return. If comment at end of line, remove.
	long pos;
	if ((pos = a_buff.find(";")) != string::npos) {
		if (pos == 0) {
			// whole line is a comment, don't need to parse it
			m_type = ST_Comment;
			return m_type;
		}
		else {
			// erases comment from end of line
			a_buff.erase(pos, a_buff.length());
		}
	}

	istringstream ss_buff(a_buff);
	string ibuff;
	vector<string> instructions;

	// Take each word or group of numbers and separate into a vector.
	while (true) {
		ibuff = "";
		ss_buff >> ibuff;
		if (ibuff.size() == 0) break;
		instructions.push_back(ibuff);
	}

	
	// Set the label, opcode, and operand if available.
	if (instructions.size() == 3) {
		m_Label = instructions[0];
		transform(instructions[1].begin(), instructions[1].end(), instructions[1].begin(), ::toupper);
		m_OpCode = instructions[1];
		m_Operand = instructions[2];
	}
	else if (instructions.size() == 2) {
		string str = instructions[0];
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		if (opCodeInfo.find(str) != opCodeInfo.end() || isNumeric(instructions[0])) {
			m_OpCode = str;
			m_Operand = instructions[1];
		}
		else {
			m_Label = instructions[0];
			transform(instructions[1].begin(), instructions[1].end(), instructions[1].begin(), ::toupper);
			m_OpCode = instructions[1];
			SaveError(errors, "Error: No operand specified.");
		}
	}
	else if (instructions.size() == 1) {
		transform(instructions[0].begin(), instructions[0].end(), instructions[0].begin(), ::toupper);
		m_OpCode = instructions[0];
	}
	else {
		SaveError(errors, "Error: Too many instructions on line.");
		m_Label = instructions[0];
		transform(instructions[1].begin(), instructions[1].end(), instructions[1].begin(), ::toupper);
		m_OpCode = instructions[1];
		m_Operand = instructions[2];
	}

	// attempt to make operand valid if it is not
	string ParseStr[20];
	int val = IsValidOperand(m_Operand);
	while (val != 0) {
		switch (val) {
		case 1:
			SaveError(errors, "Error: Operand symbol too long. Truncating to max length.");
			m_Operand = m_Operand.substr(0, 10);
			break;
		case 2:
			SaveError(errors, "Error: Operand symbol does not start with a letter. Adding 'a' to front.");
			m_Operand = m_Operand.insert(m_Operand[0], "a");
			break;
		case 3:
			SaveError(errors, "Error: Operand symbol contains invalid character. Removing said characters");

			for (unsigned int i = 0; i < m_Operand.length(); i++) {
				ParseStr[i] = m_Operand[i];
			}
			// make sure operand is only made of letters and numbers
			for (unsigned int i = 0; i < m_Operand.size(); i++) {
				if (find(alphabet.begin(), alphabet.end(), ParseStr[i]) == alphabet.end() &&
					find(digits.begin(), digits.end(), ParseStr[i]) == digits.end()) {
					m_Operand.erase(i);
				}
			}
			break;
		case 4:
			SaveError(errors, "Error: Numeric operand too long. Truncating to max length.");
			m_Operand = m_Operand.substr(0, 4);
			break;
		default:
			// do nothing, no errors
			break;
		}
	}


	map<string, int> ::iterator oc = opCodeInfo.find(m_OpCode);
	// stores instruction type and number version of op code if available.
	if (oc != opCodeInfo.end()) {
		if (oc->second == 0) {
			if (oc->first == "END") {
				m_type = ST_End;
			}
			else {
				m_type = ST_AssemblerInstr;
			}
		}
		else {
			m_type = ST_MachineLanguage;
		}
	}
	else {
		m_type = ST_MachineLanguage;
	}
	return m_type;
}
/* Instruction::InstructionType Instruction::ParseInstruction(string a_buff); */


/**/
/*
NAME

Instruction::IsValidLabel - checks if label is valid

SYNOPSIS

Instruction::IsValidLabel(string a_label);
a_label	-> the string being checked for validity

DESCRIPTION

This function makes sure the label is not too long, the first
character is a letter, and all the characters are either
a letter or a number.

RETURNS

True if label is valid, false otherwise.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
bool Instruction :: IsValidLabel(string a_label) {
	if (a_label.length() > 10) {
		return false;
	}
	
	string ParseStr[20];
	for (unsigned int i = 0; i < a_label.length(); i++) {
		ParseStr[i] = a_label[i];
	}

	if (find(alphabet.begin(), alphabet.end(), ParseStr[0]) == alphabet.end()) {
		return false;
	}
	

	for (unsigned int i = 1; i < a_label.size(); i++) {
		if (find(alphabet.begin(), alphabet.end(), ParseStr[i]) == alphabet.end() &&
			find(digits.begin(), digits.end(), ParseStr[i]) == digits.end()) {
			return false;
		}
	}
	
	
	return true;
}
/* bool Instruction :: IsValidLabel( string a_label); */

/**/
/*
NAME

Instruction::IsDefinedLabel - checks if label is in label symbol table

SYNOPSIS

Instruction::IsDefinedLabel(string a_label);
a_label	-> the string being searched for

DESCRIPTION

This function searches through the symbol table to see if the
string is already in there or not.

RETURNS

true if present, false otherswise.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
bool Instruction::IsDefinedLabel(string label) {
	map<string, int>::const_iterator it = m_SymTab.begin();
	if (m_SymTab.find(label) != m_SymTab.end()) {
		return true;
	}
	return false;
}
/* bool Instruction::IsDefinedLabel(string label); */

/**/
/*
NAME

Instruction::SetNumericOpCode - sets numeric version of op code

SYNOPSIS

Instruction::SetNumericOpCode(string a_opcode);
a_opcode	-> the string to turn into numeric op code

DESCRIPTION

This function checks if the string is completely numeric. If it is and
is the correct length, it sets the numeric op code. If not it looks
for the string in the map of op codes. If it is found it sets the numeric 
op code. If it is not found or there is an error the numeric op code
is set to -1.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void Instruction::SetNumericOpCode(string a_opcode) {

	// if it is in correct number form
	if (isNumeric(a_opcode)) {
		if (a_opcode.size() > 6) {
			m_NumOpCode = -1;
		}
		m_NumOpCode = atoi(a_opcode.c_str());
	}

	// find opcode in map
	map<string, int> ::iterator oci = opCodeInfo.find(m_OpCode);

	// stores -1 to show translation wasn't found
	if (oci == opCodeInfo.end()) {
		m_NumOpCode = -1;
	}

	// stores numeric version of op code
	else {
		m_NumOpCode = oci->second;
	}
}
/* void Instruction::SetNumericopCode(string a_opcode); */


/**/
/*
NAME

Instruction::IsValidOperand - checks if operand is valid

SYNOPSIS

Instruction::InstructionType Instruction::ParseInstruction(string a_operand);
a_operand	-> string being checked for validity

DESCRIPTION

This function checks if the operand follows the format given in the specifications.

RETURNS

1 if symbol too long
2 if it  doesn't start with a letter
3 if it contains something other than numbers or letters
4 if the numeric operand was too long
0 if valid

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
int Instruction::IsValidOperand(string a_operand) {

	if (isNumeric(a_operand)) {

		// Return false if the address is too big
		if (a_operand.length() > 4) {
			m_IsNumericOperand = false;
			return 4;
		}
		m_IsNumericOperand = true;
		return 0;
	}
	else {

		// return false if symbol is too long
		if (a_operand.length() > 10) {
			SaveError(errors, "Error: Operand too long. Truncated to maximum length.");
			return 1;
		}

		// make sure first value is a letter
		string ParseStr[20];
		for (unsigned int i = 0; i < a_operand.length(); i++) {
			ParseStr[i] = a_operand[i];
		}
		if (find(alphabet.begin(), alphabet.end(), ParseStr[0]) == alphabet.end()) {
			return 2;
		}

		// make sure operand is only made of letters and numbers
		for (unsigned int i = 1; i < a_operand.size(); i++) {
			if (find(alphabet.begin(), alphabet.end(), ParseStr[i]) == alphabet.end() &&
				find(digits.begin(), digits.end(), ParseStr[i]) == digits.end()) {
				return 3;
			}
		}
		// return false otherwise
		m_IsNumericOperand = false;
	}
	return 0;

}
/* int Instruction::IsValidOperand(string a_operand); */


/**/
/*

NAME

Instruction :: isNumeric - checks if operand is a numeric value

SYNOPSIS

bool Instruction :: isNumeric(string str);
str		-> the word being checked for numeric values

DESCRIPTION

This function checks if the word is a number or not. 

RETURNS

True if all values in the parameter are numbers. False if
anything else is found

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

bool Instruction::isNumeric(string str) 
{
	// makes each character of str it's own string
	string ParseStr[20];
	for (unsigned int i = 0; i < str.length(); i++) {
		ParseStr[i] = str[i];
	}

	// checks if each character of str is a number
	for (unsigned int i = 0; i < str.length(); i++) {
		if (find(digits.begin(), digits.end(), ParseStr[i]) == digits.end()) {
			return false;
		}
	}
	
	return true;
}
/* bool Instruction::isNumeric(string str); */




/**/
/*

NAME

Instruction :: LocationNextInstruction - finds address for the next line of code

SYNOPSIS

void Instruction :: LocationNextInstruction(int a_loc);
a_loc		-> the location of the current line of code.

DESCRIPTION

This function checks if there is a jump in the location of code that could
be caused by DS or ORG. If there is, the correct jump is taken. If the 
line is a comment the location doesn't change. Otherwise move forward one.

RETURNS

int a_loc	-> The location of the next line of code. 

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/


int Instruction:: LocationNextInstruction(int a_loc)
{

	transform(m_OpCode.begin(), m_OpCode.end(), m_OpCode.begin(), ::toupper);

	// Finds location of next instruction.
	if (m_OpCode == "DS") {
		a_loc += atoi(m_Operand.c_str());
	}
	else if (m_OpCode == "ORG") {
		a_loc = atoi(m_Operand.c_str());
	}

	else if (m_type != ST_Comment) {
		a_loc += 1;
	}
	return a_loc;
}
/* int Instruction :: LocationNextInstruction(int a_loc); */


/**/
/*

NAME

Instruction :: SetNumericOperand - finds and sets numeric representation of operand

SYNOPSIS

int Instruction :: SetNumericOperand (string a_Operand, map<string, int> a_SymbolTable);
a_Operand		-> the operand being converted into a numeric value
a_SymbolTable	-> holds the symbol and its location, which is used to find its numeric value

DESCRIPTION

This function checks if the string entered is already made only of digits. If not, the symbol 
table is then searched for the string parameter. If a valid integer value is found for
the operand, it is stored in m_OperandValue and m_IsNumericValue is set to true. Otherwise
m_IsNumericValue is set to false.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Instruction::SetNumericOperand(string a_Operand, map<string, int> a_SymTab) {

	// Determine if operand is already numeric
	if (isNumeric(a_Operand)) {
		m_IsNumericOperand = true;
		m_OperandValue = atoi(m_Operand.c_str());
	}
	else {
		// Find numeric representation of the symbol
		map<string, int>::iterator it = a_SymTab.find(a_Operand);

		// store -1 to show numeric representation wasn't found
		if (it == a_SymTab.end()) {
			m_OperandValue = -1;
		}

		// store 0 to show no translation needed
		else if (it->first == "DS" || it->first == "ORG") {
			m_OperandValue = 0;
			m_NumOpCode = 0;
		}

		// store numeric representation
		else {
			m_OperandValue = it->second;
		}
		m_IsNumericOperand = false;
	}
}
/* int Instruction::SetNumericOperand(string a_Operand, map<string, int> a_SymbolTable); */



/**/
/*

NAME

Instruction :: addTranslation - adds a translation to the table

SYNOPSIS

void Instruction::addTranslation(int location, int a_numOpCode, int a_numOperand, string statement)
location		-> location in memory of the instruction
a_numOpCode		-> numeric representation of operation codes
a_numOperand	-> numeric representation of operand
statement		-> the original statement of the program

DESCRIPTION

This function will attempt to add a location to the table. It
first checks if the location was multiply defined, and if so it is not
re-added to the table, the location is changed to indicate an error, and
the statement is erased to also indicate an error.

If it was not found in the symbol table the location, contents, and 
statement are added.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/


void Instruction::AddTranslation(int a_location, int a_numOpCode, int a_numOperand, string a_statement) {

	bool error = false;

	if (a_location > 9999 || a_location < -1) {
		SaveError(errors, "Error: Instruction location out of bounds.");
		error = true;
	}

	// If the location is already being used, give an error
	if (find(m_locations.begin(), m_locations.end(), a_location) != m_locations.end()) {
		SaveError(errors, "Error: Multiply defined location.");
		error = true;
	}

	if (m_OpCode == "DC" || m_OpCode == "ORG") {
		if (!isNumeric(m_Operand)) {
			m_OperandValue = -1;
		}
	}

	else if (m_OpCode == "DS") {
		if (isNumeric(m_Operand)) {
			if ((stoi(m_Operand) + a_location) > 9999) {
				SaveError(errors, "Error: Defined storage too large. Exceeds allocated memory.");
			}
		}
		else {
			m_OperandValue = -1;
		}
	}
	
	else if (a_numOpCode < 0 || a_numOpCode > 7) {
		if (!IsDefinedLabel(m_Operand)) {
			m_OperandValue = -1;
		}
	}

	// if there is a label, checks if it is multiply defined
	if (m_Label != "") {
		map<string, int>::iterator st = m_SymTab.find(m_Label);
		if (st != m_SymTab.end() && st->second == -999) {
			SaveError(errors, "Error: Multiply defined label.");
			m_OperandValue = -1;
		}
	}
	
	if (a_numOpCode == 0) {

		// records comment
		if (m_type == ST_Comment) {
			m_Output << "\t\t\t\t" << a_statement << "\n";
		}

		// records end statement
		else if (m_type == ST_End) {
			m_Output << "\t\t\t\t" << a_statement << "\n";
		}

		// records other lines that don't require a translation
		else  {
			string tmp = formatTranslation(m_NumOpCode, a_numOperand);
			m_translatedCode.push_back(make_pair(a_location, tmp));
			m_locations.push_back(a_location);
			if (m_OpCode == "ORG" || m_OpCode == "DS") {
				m_Output << a_location << "\t\t\t\t" << a_statement << "\n";
			}
			else {
				m_Output << a_location << "\t\t" << tmp << "\t\t" << a_statement << "\n";
			}
		}
	}
	else {
		string contents = formatTranslation(a_numOpCode, a_numOperand);

		// Record the location, machine code, and the output for the translator
		m_translatedCode.push_back(make_pair(a_location, contents));
		m_locations.push_back(a_location);
		m_Output << a_location << "\t\t" << contents << "\t\t" << a_statement << "\n";
	}

	// adds any errors that occured under the translation
	m_Output << errors;

}
/* Instruction::AddTranslation(int location, int contents, string statement); */



/**/
/*

NAME

Instruction :: formatTranslation - adds zeros to translation if needed

SYNOPSIS

string Instruction :: formatTranslation(int opCode, int operand)
opCode	-> numeric op code to be included in translation
operand	-> numeric operand to be included in the translation

DESCRIPTION

This function will check the value of the op code and determine whether
a zero must be added for it to take up 2 places. Then it checks the 
value of the operand to see if or how many zeros must be added for it
to take up 4 places.


RETURNS

A string with the translation correctly formatted.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

string Instruction :: formatTranslation(int opCode, int operand) {
	// formats translated op code and operand into a string of correct length
	stringstream content;
	if (opCode == -1) {
		SaveError(errors, "Error: Illegal operation code.");
		content << "??";
	}
	else if (opCode < 10) {
		content << "0" << opCode;
	}
	else {
		content << opCode;
	}

	if (operand == -1) {
		SaveError(errors, "Error: Illegal operand code.");
		content << "????";
	}
	else if (operand < 10) {
		content << "000" << operand;
	}
	else if (operand < 100) {
		content << "00" << operand;
	}
	else if (operand < 1000) {
		content << "0" << operand;
	}
	else {
		content << operand;
	}

	return content.str();
}
/* string Instruction :: formatTranslation(int opCode, int operand); */


/**/
/*

NAME

Instruction::DisplayTranslation - prints formatted translation of the program

SYNOPSIS

void Instruction::DisplayTranslation();

DESCRIPTION

This function will print the title of the table on one row and the
title of the rows on another. Then it iterates through the translation table
and prints each location, the machine code contents of the location, and the
original statement to the screen.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Instruction::DisplayTranslation() {

	cout << "Translation of Program:" << endl;
	cout << "Location\tContents\tOriginal Statement" << endl;
	
	cout << m_Output.str();

	cout << endl;
	cout << "Press enter to continue...";
	char temp = 'x';
	while (temp != '\n')
		cin.get(temp);
	cout << endl;
}
/* void Instruction::DisplayTranslation(); */


/**/
/*

NAME

Instruction::SaveError - saves error using Error class

SYNOPSIS

void Instruction::SaveError(string &errors, string msg)
errors	-> string to collect the error messages
msg		-> Message describing the error that occurred.


DESCRIPTION

This function will add the error message to a string passed
by reference followed by a new line character, then uses the
RecordError function from the Errors class to save the error
that occurred.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Instruction::SaveError(string &errors, string msg) {
	errors += (msg + "\n");
	Errors::RecordError(msg);
}
/* void Instruction::SaveError(string &errors, string msg) */

/**/
/*

NAME

Instruction::SetSymbolTable - puts all the values into symbol table

SYNOPSIS

void Instruction::SetSymbolTable(map<string, int> a_symtab)
a_symtab	-> the map who's values are being entered in


DESCRIPTION

This function take each value of the entered symbol table and
enter it into this class's symbol table.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/
void Instruction::SetSymTab(map<string, int> a_symtab) {
	for (map<string, int>::iterator it = a_symtab.begin(); it != a_symtab.end(); it++)
	{
		m_SymTab[it->first] = it->second;
	}
}
/* void Instuction :: SetSymTab(map<string, int> a_symtab); */