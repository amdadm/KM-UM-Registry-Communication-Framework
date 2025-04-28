#pragma once 

#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>
#include <ntimage.h>

#include "../Includes/Structs.h"

uint64_t GetModuleNtoskrnlBase()
{
	uint64_t Base;
	ULONG Size = 0;
	ZwQuerySystemInformation(11, &Size, 0, &Size);
	if (Size == 0)
		return false;

	const unsigned long tag = 'VMON';
	PSYSTEM_MODULE_INFORMATION sys_mods = (PSYSTEM_MODULE_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, Size, tag);
	if (sys_mods == 0)
		return false;

	NTSTATUS status = ZwQuerySystemInformation(11, sys_mods, Size, 0);
	if (!NT_SUCCESS(status))
	{
		ExFreePoolWithTag(sys_mods, tag);
		return false;
	}

	for (unsigned long long i = 0; i < sys_mods->ulModuleCount; i++)
	{
		PSYSTEM_MODULE mod = &sys_mods->Modules[i];
		if (strstr(mod->ImageName, "ntoskrnl.exe"))
		{
			Base = (uint64_t)mod->Base;
			break;
		}
	}

	ExFreePoolWithTag(sys_mods, tag);
	return Base;
}

BOOL CheckMask(PCHAR base, PCHAR pattern, PCHAR mask)
{
	for (; *mask; ++base, ++pattern, ++mask)
	{
		if ('x' == *mask && *base != *pattern)
		{
			return FALSE;
		}
	}

	return TRUE;
}

UINT_PTR FindPattern(PCHAR base, DWORD length, PCHAR pattern, PCHAR mask)
{
	length -= (DWORD)strlen(mask);
	for (DWORD i = 0; i <= length; ++i)
	{
		UINT_PTR addr = UINT_PTR(&base[i]);

		if (CheckMask((PCHAR)addr, pattern, mask))
		{
			return addr;
		}
	}

	return 0;
}

UINT_PTR FindPatternImage(PCHAR base, PCHAR pattern, PCHAR mask)
{
	UINT_PTR match = 0;

	PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)(base + ((PIMAGE_DOS_HEADER)base)->e_lfanew);
	PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(headers);
	for (DWORD i = 0; i < headers->FileHeader.NumberOfSections; ++i)
	{
		PIMAGE_SECTION_HEADER section = &sections[i];
		if ('EGAP' == *(int*)section->Name || memcmp(section->Name, ".text", 5) == 0)
		{
			match = FindPattern(base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
			if (match)
			{
				break;
			}
		}
	}

	return match;
}

uintptr_t GetCallbackListHead()
{
	uint64_t NtoskrnlBase = GetModuleNtoskrnlBase();
	if (!NtoskrnlBase)
		return 0;

	auto CallbackListHead = FindPatternImage(PCHAR(NtoskrnlBase),
		"\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x44\x24\x00\x48\x85\xC0",
		"xxx????x????xxxxxxx?xxx");

	if (!CallbackListHead)
		return 0;

	CallbackListHead = reinterpret_cast<uintptr_t>(PCHAR(CallbackListHead) + 7 + *reinterpret_cast<INT*>(PCHAR(CallbackListHead) + 3));
	return uintptr_t(CallbackListHead);
}

NTSTATUS RegisterCallback(
	PEX_CALLBACK_FUNCTION Function,
	PVOID Context,
	PLARGE_INTEGER Cookie
)
{
	uintptr_t CallbackListHead = GetCallbackListHead();
	if (!CallbackListHead)
		return STATUS_UNSUCCESSFUL;

	auto CallbackEntry = (PRegisterCallbackEntry)ExAllocatePoolWithTag(NonPagedPool, sizeof(RegisterCallbackEntry), 'cbk');
	if (!CallbackEntry)
		return STATUS_INSUFFICIENT_RESOURCES;

	RtlZeroMemory(CallbackEntry, sizeof(RegisterCallbackEntry));

	CallbackEntry->Routine = Function;
	CallbackEntry->Context = Context;
	CallbackEntry->PendingDelete = FALSE;
	CallbackEntry->Cookie.QuadPart = 0;

	InsertTailList((PLIST_ENTRY)CallbackListHead, &CallbackEntry->ListEntryHead);

	*Cookie = CallbackEntry->Cookie;

	return STATUS_SUCCESS;
}

