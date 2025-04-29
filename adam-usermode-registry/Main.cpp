#pragma once

#include "Communication/Registry.h"

/*===============================================================================================
  Project         : KM-UM-Registry-Communication-Framework

  Original Author : amdadm
  GitHub          : https://github.com/amdadm
  Created         : 2025-04-28

  Description     : Framework for establishing communication between Kernel Mode (KM) and User Mode (UM) components through the Windows Registry.

  License         : MIT License
===============================================================================================*/

int main()
{
	ProcessPid = GetProcessID("Notepad.exe");

	std::cout << " [+] Notepad PID: " << ProcessPid << std::endl;

	BaseAddress = GetBaseAddress();

	std::cout << std::hex << " [+] Notepad Base Address: 0x"  << BaseAddress << std::endl;

	system("pause");
}
