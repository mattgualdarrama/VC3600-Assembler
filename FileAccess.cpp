//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*

NAME

FileAccess::FileAccess - opens file

SYNOPSIS

FileAccess:FileAccess(int argc, void *argv[]);
argc	-> holds the number of command line arguments
argv[]	-> holds the command line arguments (path to file name)

DESCRIPTION

This function attempts to open the file entered as a command line argument.
Makes sure there is only one parameter entered and that the file
was opened correctly.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/* FileAccess::FileAccess( int argc, char *argv[] ); */


/**/
/*

NAME

FileAccess::~FileAccess - closes file

SYNOPSIS

FileAccess:~FileAccess();

DESCRIPTION

This function closes the file that was opened by the constructor
class.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

FileAccess::~FileAccess( )
{
    m_sfile.close( );
}
/* FileAccess::~FileAccess(); */



/**/
/*

NAME

FileAccess::GetNextLine - gets the next line from the file

SYNOPSIS

bool FileAccess::GetNextLine( string &a_buff );
a_buff	-> the current line

DESCRIPTION

This function checks if there is no more data to read. If there is,
it gets the next line and stores it in a_buff, which was passed
by reference.


RETURNS

True if there is another line to read and it was read successfully, 
false if there is no more data.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

bool FileAccess::GetNextLine( string &a_buff )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_buff );
    
    // Return indicating success.
    return true;
}
/* bool FileAccess::GetNextLine( string &a_buff ); */


/**/
/*

NAME

FileAccess::rewind - moves back to the beginning of the file

SYNOPSIS

FileACcess:rewind();

DESCRIPTION

This function clears the relevant flags and goes back to the
beginning of the file.


RETURNS

Nothing.

AUTHOR

Sabrina Hemming

DATE

12/22/2016

*/
/**/

void FileAccess::rewind( )
{
    // Clean tall file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/* void FileAccess::rewind(); */
    
