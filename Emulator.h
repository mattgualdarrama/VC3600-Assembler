//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H
#pragma once

class emulator {

public:

    const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
    emulator() {

        memset( m_memory, 0, MEMSZ * sizeof(int) );
    }

    // Records instructions and data into VC3600 memory.
    bool insertMemory( int a_location, int a_contents );
    
    // Runs the VC3600 program recorded in memory.
    bool runProgram( );

private:

    int m_memory[MEMSZ];       // The memory of the VC3600.
    int accumulator;			// The accumulator for the VC3600
	int m_loc;					// current location in code
	int start = -1;				// start of the code
	pair<int, int> instruction;	// op code and operand in that order
	bool isdone = false;		// determines if the program is done running

	void Add();
	void Sub();
	void Mult();
	void Div();
	void Load();
	void Store();
	void Read();
	void Write();
	void B();
	void Bm();
	void Bz();
	void Bp();
	void Halt();

};

#endif

