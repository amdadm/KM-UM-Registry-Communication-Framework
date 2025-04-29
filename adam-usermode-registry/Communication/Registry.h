#pragma once 

#include <iostream>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

typedef struct _Request
{
	UINT32 Type;
	PVOID Instruction;
} Request, * PRequest;

typedef struct _GetBaseAddressRequest
{
	ULONG       ProcessId;
	ULONGLONG   BaseAddress;
} GetBaseAddressRequest, * PGetBaseAddressRequest;

ULONG ProcessPid;
uintptr_t BaseAddress;

void RegistryRequest(uint32_t type, void* instruction) {

	HKEY hKey = NULL;
	void* Pointer = NULL;
	char Code[30] = "RegKeyCommunicationTest";
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Policies\\Microsoft\\Windows\\Safer\\CodeIdentifiers", 0, KEY_ALL_ACCESS, &hKey);

	if (hKey != NULL && hKey != INVALID_HANDLE_VALUE) {

		auto SetRegistryValue = [&](BYTE* Pointer, SIZE_T size, DWORD Type) -> BOOL
			{
				if (RegSetValueExA(hKey, Code, 0, Type, reinterpret_cast<BYTE*>(Pointer), size) == ERROR_SUCCESS)
				{
					RegDeleteValue(hKey, Code);
					RegCloseKey(hKey);
					return TRUE;
				}
				return FALSE;
			};

		Request Request;

		Request.Type = type;
		Request.Instruction = instruction;

		Pointer = &Request;
		SetRegistryValue(reinterpret_cast<BYTE*>(&Pointer), sizeof uintptr_t, REG_QWORD);
	}
}

DWORD GetProcessID(std::string processName) {

	DWORD ProcessPid = 0;
	PROCESSENTRY32 Entry;
	Entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &Entry) == TRUE)
	{
		while (Process32Next(snapshot, &Entry) == TRUE)
		{
			if (strcmp(Entry.szExeFile, processName.c_str()) == 0)
			{
				ProcessPid = Entry.th32ProcessID;
				break;
			}
		}
	}

	CloseHandle(snapshot);
	return ProcessPid;
}

uint64_t GetBaseAddress() {

	GetBaseAddressRequest Request;

	Request.ProcessId = ProcessPid;
	Request.BaseAddress = 0;

	RegistryRequest(1, &Request);

	return Request.BaseAddress;
}
