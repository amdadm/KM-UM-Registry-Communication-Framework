#pragma once

#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>

static LARGE_INTEGER Cookie = { 0x3090 };

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

