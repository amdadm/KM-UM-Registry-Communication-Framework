#pragma once 

#include "../Includes/Structs.h"

uintptr_t GetBaseAddress(int processPid) {

    PEPROCESS targetProcess = nullptr;
    NTSTATUS  status = PsLookupProcessByProcessId(HANDLE(processPid), &targetProcess);

    if (!NT_SUCCESS(status))
        return 0;

    const auto base_address = uintptr_t(PsGetProcessSectionBaseAddress(targetProcess));
    ObDereferenceObject(targetProcess);

    return base_address;
}