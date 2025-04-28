#pragma once
#include <ntdef.h>
#include <minwindef.h>
#include <intrin.h>

typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef struct _RegisterCallbackEntry
{
	LIST_ENTRY ListEntryHead;
	BOOLEAN PendingDelete;
	LARGE_INTEGER Cookie;
	void* Context;
	void* Routine;
} RegisterCallbackEntry, * PRegisterCallbackEntry;

typedef struct _SYSTEM_MODULE
{
	ULONG_PTR Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG_PTR ulModuleCount;
	SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

extern "C" {
	NTKERNELAPI NTSTATUS PsLookupProcessByProcessId( IN HANDLE ProcessId, OUT PEPROCESS* Process );
	NTKERNELAPI PVOID PsGetProcessSectionBaseAddress( IN PEPROCESS Process );
    NTKERNELAPI NTSTATUS ZwQuerySystemInformation( DWORD32 systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength );
}