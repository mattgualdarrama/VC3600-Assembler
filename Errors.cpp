#include "stdafx.h"
#include "Errors.h"



vector<string> Errors::m_ErrorMsgs;

/**/
/*

NAME

Errors::RecordError - Records an error message.

SYNOPSIS

void Errors::RecordError (string &a_emsg)
a_emsg	-> the error being recorded

DESCRIPTION

This function records the error sent so that they can all be 
accessed in one place

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
}
/* Errors::RecordError(string &a_emsg); */


/**/
/*

NAME

Errors::DisplayErrors - Displays the collected error messages

SYNOPSIS

void Errors :: DisplayErrors()

DESCRIPTION

This function prints all the error messages recorded onto 
the screen in the order they were recieved. Reports if no errors
were found.

RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void Errors :: DisplayErrors() {
	cout << "Errors: " << endl;

	if (m_ErrorMsgs.size() == 0) {
		cout << "No errors recorded." << endl;
	}
	else {
		for (unsigned int i = 0; i < m_ErrorMsgs.size(); i++) {
			cout << m_ErrorMsgs[i] << endl;
		}
	}
	
}
/* Errors :: DisplayErrors(); */
