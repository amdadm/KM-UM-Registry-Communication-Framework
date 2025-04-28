#pragma once 

#include "../../Features/GetBaseAddress.h"

#include "../../Includes/Structs.h"
#include "../../Includes/Definitions.h"

UNICODE_STRING RegistryKey;

NTSTATUS RegistryCallback(PVOID callbackContext, PVOID arg1, PVOID arg2) {

    UNREFERENCED_PARAMETER(callbackContext);

    if (INT(arg1) != 16)
        return STATUS_SUCCESS;

    PREG_POST_OPERATION_INFORMATION postInfo = (PREG_POST_OPERATION_INFORMATION)arg2;
    PREG_SET_VALUE_KEY_INFORMATION preInfo = (PREG_SET_VALUE_KEY_INFORMATION)postInfo->PreInformation;

    NTSTATUS status;
    ULONG info_size = 0;

    if (RtlEqualUnicodeString(preInfo->ValueName, &RegistryKey, TRUE) == 0)
        return STATUS_SUCCESS;

    if (!preInfo->Data)
        return STATUS_SUCCESS;

    PRequest Data = *reinterpret_cast<PRequest*>(preInfo->Data);

    if (!Data)
        return STATUS_SUCCESS;;

    switch (Data->Type)
    {
        // Base Address
        case 4: {

            auto ModuleInfo = PGetBaseAddressRequest(Data->Instruction);
        
            ULONGLONG handle = GetBaseAddress(ModuleInfo->ProcessId);
            ModuleInfo->BaseAddress = handle;

            status = STATUS_SUCCESS;

            info_size = sizeof(GetBaseAddressRequest);

        } break;

        // ...
    }

    return STATUS_SUCCESS;
}