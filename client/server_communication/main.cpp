#include "stdafx.h"
#include "main.h"

uint32_t g_dwSAMP_Addr = NULL;

void disableSampAC() {

	getSampBaseAddress();
	static struct patch_set sampPatchDisableAntiCheat_patch =
	{
		"Disable AC",
		0,
		0,
		{
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x2903DB ), NULL, (uint8_t *)"\xE9\x90\x4E\xFB\xFF", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + 0xA16FA ), NULL, (uint8_t *)"\xEB", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + 0xA92C7 ), NULL, (uint8_t *)"\xEB", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + 0xA9B48 ), NULL, (uint8_t *)"\xEB", NULL },
			{ 6, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x290014 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x290028 ), NULL, (uint8_t *)"\xE9\xDF\x00\xFA\xFF", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x221968 ), NULL, (uint8_t *)"\xE9\x9F\xE7\x00\x00", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x5AD1B ), NULL, (uint8_t *)"\xEB", NULL },
			{ 6, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x23EB8D ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x280E78 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x6C122 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x6C127 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90", NULL },
			{ 5, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x6C149 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90", NULL },
			{ 6, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x6BFE9 ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90", NULL },
			{ 2, (void *)( (uint8_t *)g_dwSAMP_Addr + 0x986AE ), NULL, (uint8_t *)"\x90\x90", NULL }
		}
	};
	
	patcher_install(&sampPatchDisableAntiCheat_patch);
}

void getSampBaseAddress() {
	g_dwSAMP_Addr = (DWORD)GetModuleHandle (L"samp.dll");
}