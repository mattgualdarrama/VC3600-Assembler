//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once
#include "SymTab.h"

// The elements of an instruction.
class Instruction {

public:

	Instruction() {};
    Instruction( map<string, int> a_SymTab ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage,  // A machine language instruction.
        ST_AssemblerInstr,   // Assembler Language instruction.
        ST_Comment,          // Comment or blank line
        ST_End               // end instruction.
    };

    // Parse the Instruction.
	InstructionType ParseInstruction(string a_buff);
	
    // Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

    // To access the label
    inline string &GetLabel( ) {
        return m_Label;
    };

	// access the numeric op code
	inline int &GetNumOpCode() {
		return m_NumOpCode;
	}

	// access the string op code
	inline string &GetOpCode() {
		return m_OpCode;
	}

	// access the string operand
	inline string &GetOperand() {
		return m_Operand;
	}

	// access the original line of instruction
	inline string &GetInstruction() {
		return m_instruction;
	}

	// access the numeric operand
	inline int &GetNumOperand() {
		return m_OperandValue;
	}

	// access to whether the operand is numeric
	inline bool &GetIsNumericOperand() {
		return m_IsNumericOperand;
	}

	inline vector<pair<int, string>> &GetTranslatedCode() {
		return m_translatedCode;
	}

    // To determine if a label is blank.
    inline bool isLabel( ) {
        return ! m_Label.empty();
    };

	void SetSymTab(map<string, int> a_symTab);

	// Gets numeric value of operand if possible
	void SetNumericOperand(string a_Operand, map<string, int> a_symtab);

	// Gets numeric value of operation code if possible
	void SetNumericOpCode(string a_opcode);

	// Adds next line of the translation
	void AddTranslation(int a_location, int a_numOpCode, int a_numOperand, string a_statement);

	// Displays translation of the program.
	void DisplayTranslation();

	// Saves the errors that occur
	void SaveError(string &errors, string msg);


private:

	map<string, int> m_SymTab; // Symbol table object

    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;      // The operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;	// == true if the operand is numeric.
    int m_OperandValue;		 // The value of the operand if it is numeric.

	vector<int> m_locations;

	stringstream m_Output;		// stores formatted output of translation with errors
	vector<pair <int,string>> m_translatedCode;	// holds location and the machine code
	string errors;				// stores the errors of one line of code

	// checks if label is valid
	bool IsValidLabel(string a_label);
	
	// checks if operand is valid
	int IsValidOperand(string a_operand);

	// check if the label was previously defined
	bool IsDefinedLabel(string label);

	
	// map of op code and assembler instructions for easy translation
	map<string, int> opCodeInfo =
	{
		{ "ADD", 1 },{ "SUB", 2 },{ "MULT", 3 }, { "DIV", 4 },
		{ "LOAD", 5 },{ "STORE", 6 }, { "READ", 7 },{ "WRITE", 8 },
		{ "B", 9 },{ "BM", 10 },{ "BZ", 11 },{ "BP", 12 },
		{ "HALT", 13 },{ "DC", 0 },{ "DS", 0 },{ "ORG", 0 },{ "END", 0 }
	};

	// vector of alphabet to check if a character is a letter
	vector<string> alphabet =
	{
		"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
		"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
		"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
	};

	// vector of numbers stored as strings to check if a character is a number
	vector<string> digits =
	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
	};

	// checks if string is made of all numbers
	bool isNumeric(string str); 

	// adds zeros to translation if needed, "?"s if invalid op code or operand
	string formatTranslation(int opCode, int operand);   
};
