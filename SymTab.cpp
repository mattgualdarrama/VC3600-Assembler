//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"



/**/
/*

NAME

SymbolTable::AddSymbol - adds a symbol to the symbol table

SYNOPSIS

void SymbolTable::AddSymbol( string &a_symbol, int a_loc );
a_symbol	-> the symbol being added to the symbol table.
a_loc		-> the location of the symbol being added.


DESCRIPTION

This function will attempt to add a symbol to the symbol table. It
first checks if the symbol was multiply defined, and if so it is not
re-added to the table and the location is changed to indicate an error.

If it was not found in the symbol table the label and location is added.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st;
    st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    
    // Record the location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/* SymbolTable::AddSymbol( string &a_symbol, int a_loc ); */



/**/
/*

NAME

SymbolTable::DisplaySymbolTable - prints formatted symbol table to the screen.

SYNOPSIS

void SymbolTable::DisplaySymbolTable();


DESCRIPTION

This function will print the title of the table on one row and the 
title of the rows on another. Then it iterates through the Symbol Table 
and prints each symbol number, the symbol and its location to the screen.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void SymbolTable::DisplaySymbolTable() {

	cout << "Symbol Table:" << endl;
	cout << endl;
	cout << "Symbol #\tSymbol\tLocation" << endl;
	map<string, int>::const_iterator it = m_symbolTable.begin();
	int count = 0;
	for (it; it != m_symbolTable.end(); ++it) {
		cout << count << "\t\t" << it->first << "\t" << it->second << endl;
		count++;
	}
	cout << "_______________________________________" << endl;
	cout << endl;

	cout << "Press Enter to continue...";
	char temp = 'x';
	while (temp != '\n')
		cin.get(temp);

	cout << endl;
}
/* SymbolTable::DisplaySymbolTable(); */



/**/
/*

NAME

SymbolTable::LookupSymbol - searches for a symbol in the table

SYNOPSIS

void SymbolTable::AddSymbol( string &a_symbol, int a_loc );
a_symbol	-> the symbol being added to the symbol table.
a_loc		-> the location of the symbol being added.


DESCRIPTION

This function will attempt to find the symbol entered in the symbol
table. 

RETURNS

If it is not found it returns false. If it is found the location
is set using the reference variable &a_loc and returns true.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

bool SymbolTable :: LookupSymbol(string &a_symbol, int &a_loc) {
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st == m_symbolTable.end()) {
		//not found
		return false;
	}
	else {
		a_loc = st->second;
	}
	return true;
}
/* SymbolTable::AddSymbol( string &a_symbol, int a_loc ); */