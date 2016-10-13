#define _CRT_SECURE_NO_WARNINGS

#include "winfuncs.h"

#include <cstdio>

HWND hwnd;

#include <io.h>
#include <fcntl.h>

bool create_console()
{
	FreeConsole();
	if (AllocConsole())
	{
		
		int hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		return true;
	}

	return false;
}