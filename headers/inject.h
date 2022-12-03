#pragma once

/*

Bypassing NTFile jmp so that we can use `LoadLibrary`

All credits to:
https://github.com/danielkrupinski

*/

#include <windows.h>

// Modules and other handles
struct base {
	int proc_id;
	HANDLE proc;
	uintptr_t game_module;
};

// Check if we can patch the 5 bytes
bool byPass();

// Restore original 5 bytes
void backUp();

// Handling all the ops and state
int stateHandler(const char* filePath);