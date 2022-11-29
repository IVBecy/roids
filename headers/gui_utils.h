/*

Utility functions for the GUI

*/
#pragma once
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

// File selector
const char* openFile(HWND hWnd) {
	LPSTR filebuff = new char[256];
	OPENFILENAME open = { 0 };
	open.lStructSize = sizeof(OPENFILENAME);
	open.hwndOwner = hWnd;
	open.lpstrFilter = "Image Files(.dll)\0*.dll;\0\0";
	open.lpstrCustomFilter = NULL;
	open.lpstrFile = filebuff;
	open.lpstrFile[0] = '\0';
	open.nMaxFile = 256;
	open.nFilterIndex = 1;
	open.lpstrInitialDir = NULL;
	open.lpstrTitle = "Select your DLL\0";
	open.nMaxFileTitle = strlen("Select your DLL\0");
	open.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileName(&open))
	{
		return open.lpstrFile;
	}
	return 0;
}

// Normal text
void guiText(HWND hWnd,const char* msg, int x, int y, int WIDTH, int HEIGHT) {
	CreateWindow("static", msg, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,0, 10, WIDTH, 50, hWnd, NULL, NULL, NULL);
}