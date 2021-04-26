#include <iostream>
#include<Windows.h>
#include <tchar.h>
#include <psapi.h>
#include <stdio.h>


bool CheckAndDeleteProcess(DWORD processID, const wchar_t processName[MAX_PATH])
{
	TCHAR szProcessName[MAX_PATH];

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, processID);
	LPDWORD exitCode = 0;

	// Get the process name and delete it by name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
		if (*szProcessName == *processName) {
			TerminateProcess(
				hProcess,
				1
			);
		}

		CloseHandle(hProcess);
		return 0;
	}
}

void DeleteProcName(const wchar_t processName[MAX_PATH]) {
	TCHAR szProcessName[MAX_PATH];

	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return;
	}
	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	for (size_t i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			if (CheckAndDeleteProcess(aProcesses[i], processName)) {
			};
		}
	}
}

int main()
{
	DeleteProcName(L"Telegram.exe");
}