#pragma once 

#include "../Includes/Structs.h"

uintptr_t GetBaseAddress(int ProcessPid) {

    PEPROCESS TargetProcess = nullptr;
    
    NTSTATUS  status = PsLookupProcessByProcessId(HANDLE(ProcessPid), &TargetProcess);
    if (!NT_SUCCESS(status))
        return 0;

    const auto BaseAddress = uintptr_t(PsGetProcessSectionBaseAddress(TargetProcess));
    ObDereferenceObject(TargetProcess);

    return BaseAddress;
}
