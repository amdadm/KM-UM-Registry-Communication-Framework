#pragma once 
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>

#include "../Includes/Definitions.h"

#include "Callback/CallbackControl.h"
#include "Callback/Callback.h"

NTSTATUS DriverEntry( _In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath )
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrintEx(0, 0, "Driver loaded!\n");

	RtlInitUnicodeString(&RegistryKey, L"MyRegID");
	auto Status = RegisterCallback(PEX_CALLBACK_FUNCTION(RegistryCallback), nullptr, &Cookie);
	if (!NT_SUCCESS(Status))
		return Status;

	DbgPrintEx(0, 0, "Success!\n");

	return Status;
}