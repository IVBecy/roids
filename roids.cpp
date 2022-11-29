/*

Gui management

HELP:
https://learn.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-170

*/
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <shobjidl.h>
#include "headers/inject.h"
#include "headers/gui_utils.h"

// Vars
HINSTANCE hInst;
static TCHAR szWindowClass[] = _T("Roids");
static TCHAR szTitle[] = _T("Roids injector");
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WIDTH = 400;
int HEIGHT = 400;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE previousInstance, LPTSTR arguments, int nCmdShow) {
	// Configure GUI
	WNDCLASSEX wcex{ };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(20,20,20)));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	hInst = hInstance;

	// Create Window
	HWND hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		szWindowClass,
		szTitle,
		WS_OVERLAPPED |WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH, HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	CreateWindow("BUTTON", "Select DLL",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT,
		(WIDTH * 0.025), (HEIGHT - 90),
		100, 40,
		hWnd,
		(HMENU)1,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);


	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


// Window configure (operations)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	// Actions
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			const char* filePath = openFile(hWnd);
			int in = stateHandler(filePath);
			switch (in) {
			case 1:
				guiText(hWnd, "Injection successful", 0, 10, WIDTH, HEIGHT);
				break;
			case 0:
				guiText(hWnd, "Injection failed", 0, 10, WIDTH, HEIGHT);
				break;
			default:
				guiText(hWnd, "Error, maybe try running it with admin privs", 0, 10, WIDTH, HEIGHT);
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(173,255,47));
		SetBkColor(hdcStatic, RGB(20,20,20));
		return (LRESULT)GetStockObject(NULL_BRUSH);
		break;
	}
	// Quitting
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}