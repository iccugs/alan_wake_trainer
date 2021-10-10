#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "ascii.h"

bool bHealth = false;

void Menu()
{
	std::cout << "godmode = " << bHealth << "\n";
	std::cout << "press insert to exit trainer\n";
}

int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 1);
	uintptr_t moduleBase = 0, playerPtr = 0, healthAddr = 0;
	const float maxHealth = 1;
	DWORD procId = GetProcId(L"AlanWake.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"AlanWake.exe");
		AsciiArt();
		getchar();
		ClearScreen();
		Menu();
	}

	else
	{
		std::cout << "Alan Wake process not found, press enter to exit\n";
		getchar();
		return(0);
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		playerPtr = moduleBase + 0x002C18A4;
		healthAddr = FindDMAAddy(hProcess, playerPtr, { 0x04,0x04,0x04,0xE0,0x2C });

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;

			ClearScreen();
			Menu();
		}
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&maxHealth, sizeof(maxHealth), hProcess);
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return(0);
		}

		Sleep(100);

	}

	ClearScreen();
	std::cout << "AlanWake.exe not found, press enter to exit\n";
	getchar();
	return(0);
}