//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H
#pragma once

class Errors {

public:
    
    // Initializes error reports.
    //static void InitErrorReporting( );

    // Records an error message.
    static void RecordError( string a_emsg );

    // Displays the collected error message.
    static void DisplayErrors( );

	inline static vector<string> getErrors() {
		return m_ErrorMsgs;
	}

private:

    static vector<string> m_ErrorMsgs;
};
#endif