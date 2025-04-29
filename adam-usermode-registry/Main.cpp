#pragma once

#include "Communication/Registry.h"

/*____________________________________________________________________________________________________________
Original Author: amdadm
Github: https://github.com/amdadm
kernel driver
        Compile-time, Usermode + Kernelmode, high-performance kernel driver framework for C++11+
                            *Not removing this part is appreciated*
____________________________________________________________________________________________________________*/

int main()
{
	ProcessPid = GetProcessID("Notepad.exe");

	std::cout << " [+] Notepad PID: " << ProcessPid << std::endl;

	BaseAddress = GetBaseAddress();

	std::cout << std::hex << " [+] Notepad Base Address: 0x"  << BaseAddress << std::endl;

	system("pause");
}
