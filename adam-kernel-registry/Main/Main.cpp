#pragma once 
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>

#include "../Includes/Definitions.h"

#include "Callback/CallbackControl.h"
#include "Callback/Callback.h"

/*===============================================================================================
  Project         : KM-UM-Registry-Communication-Framework

  Original Author : amdadm
  GitHub          : https://github.com/amdadm
  Created         : 2025-04-28

  Description     : Framework for establishing communication between Kernel Mode (KM) and User Mode (UM) components through the Windows Registry.

  License         : MIT License
===============================================================================================*/

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
