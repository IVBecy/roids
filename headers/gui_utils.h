/*

Utility functions for the GUI

*/
#pragma once
#include <windows.h>

// File selector
const char* openFile(HWND hWnd);

// Normal text
void guiText(HWND hWnd, const char* msg, int x, int y, int WIDTH, int HEIGHT);