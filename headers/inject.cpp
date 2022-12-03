#include "inject.h"
#include "MemMan.h"


MemMan mem;
LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");

// Moduels and other handles
base Base;


// Check if we can patch the 5 bytes
bool byPass() {
	if (ntOpenFile) {
		char mainBytes[5];
		memcpy(mainBytes, ntOpenFile, sizeof(mainBytes));
		WriteProcessMemory(Base.proc, ntOpenFile, mainBytes, 5, NULL);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Restore original 5 bytes
void backUp() {
	if (ntOpenFile) {
		char mainBytes[5];
		memcpy(mainBytes, ntOpenFile, sizeof(mainBytes));
		WriteProcessMemory(Base.proc, ntOpenFile, mainBytes, 0, NULL);
	}
}

bool Injector(uintptr_t ProcessId, const char* Dll)
{
	if (ProcessId == NULL)
		return FALSE;

	char CustomDLL[MAX_PATH];
	GetFullPathName(Dll, MAX_PATH, CustomDLL, 0);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(CustomDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!WriteProcessMemory(hProcess, allocatedMem, CustomDLL, sizeof(CustomDLL), NULL)) {
		return FALSE;
	}

	CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

	if (hProcess) {
		CloseHandle(hProcess);
	}
	return TRUE;
}

// Handling all the ops and state
int stateHandler(const char* filePath) {
	// Game and module
	Base.proc_id = mem.getPid(L"csgo.exe");
	Base.proc = mem.getProcess(L"csgo.exe");
	Base.game_module = mem.getModule(Base.proc_id, L"client.dll");

	// Check for bypass
	if (byPass()) {
		backUp();
		if (Injector(Base.proc_id, filePath)) {
			return 1;
		}
	}

	return 0;
}
