#pragma once

#include "Communication/Registry.h"

int main()
{
	ProcessPid = GetProcessID("Notepad.exe");

	std::cout << " [+] Notepad PID: " << ProcessPid << std::endl;

	BaseAddress = GetBaseAddress();

	std::cout << std::hex << " [+] Notepad Base Address: 0x"  << BaseAddress << std::endl;

	system("pause");
}
