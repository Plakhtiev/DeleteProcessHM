#include <iostream>
#include<Windows.h>
#include <psapi.h>
#include <stdio.h>


bool CheckAndDeleteProcess(DWORD processID, const wchar_t processNameForDel[MAX_PATH])
{
	wchar_t currProcessName[MAX_PATH];

	// Get a handle to the process.

	HANDLE currProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, processID);
	LPDWORD exitCode = 0;

	// Get the process name and delete it by name.

	if (NULL != currProcess)
	{
		HMODULE arrayListMod;
		DWORD sizeOfArray;

		if (EnumProcessModules(currProcess, &arrayListMod, sizeof(arrayListMod),
			&sizeOfArray))
		{
			GetModuleBaseName(currProcess, arrayListMod, currProcessName,
				sizeof(currProcessName) / sizeof(wchar_t));
		}
		if (*currProcessName == *processNameForDel) {
			TerminateProcess(
				currProcess,
				1
			);
		}

		CloseHandle(currProcess);
		return 0;
	}
}

void DeleteProcName(const wchar_t processNameForDel[MAX_PATH]) {
	wchar_t currProcessName[MAX_PATH];

	// Get the list of process identifiers.
	DWORD arrayProcesses[1024], sizeOfArray, countProcessIdentifiers;

	if (!EnumProcesses(arrayProcesses, sizeof(arrayProcesses), &sizeOfArray)) {
		return;
	}
	// Calculate how many process identifiers were returned.

	countProcessIdentifiers = sizeOfArray / sizeof(DWORD);

	for (size_t i = 0; i < countProcessIdentifiers; i++)
	{
		if (arrayProcesses[i] != 0)
		{
			if (CheckAndDeleteProcess(arrayProcesses[i], processNameForDel)) {
				return;
			};
		}
	}
}

int main()
{
	DeleteProcName(L"Telegram.exe");
}