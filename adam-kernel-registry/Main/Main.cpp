#pragma once 
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>

#include "../Includes/Definitions.h"

#include "Callback/CallbackControl.h"
#include "Callback/Callback.h"

/*____________________________________________________________________________________________________________
Original Author: amdadm
Github: https://github.com/amdadm
License: See end of file
kernel driver
        Compile-time, Usermode + Kernelmode, high-performance kernel driver framework for C++11+
                            *Not removing this part is appreciated*
____________________________________________________________________________________________________________*/

NTSTATUS DriverEntry( _In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath )
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrintEx(0, 0, "Driver loaded!\n");

	RtlInitUnicodeString(&RegistryKey, L"RegKeyCommunicationTest");
	auto Status = RegisterCallback(PEX_CALLBACK_FUNCTION(RegistryCallback), nullptr, &Cookie);
	if (!NT_SUCCESS(Status))
		return Status;

	DbgPrintEx(0, 0, "Success!\n");

	return Status;
}
