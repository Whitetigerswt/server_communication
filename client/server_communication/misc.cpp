#include "stdafx.h"

void vp(DWORD address, int s)
{
	DWORD d;
	VirtualProtect( (PVOID)address, s, PAGE_EXECUTE_READWRITE, &d );
}