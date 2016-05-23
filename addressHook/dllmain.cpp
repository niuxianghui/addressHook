// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#define ADDRESSHOOK_H __declspec(dllexport)
#include "public.h"
//#include <stdio.h>
#include <malloc.h>

DWORD WINAPI dwWaitThread(LPVOID lpArgs)
{
	int a = 0;
	while (true)
	{
		Sleep(10000);
		if (replace > 0)
		{
			if (a < 100) a++;
			if (a == 1) Sleep(10000);
			Sleep(1000);
			//MessageBoxA(FindWindowA(NULL, "League of Legends (TM) Client"), "defunc", "defunc", 0);
			DeDetourfunc();
		}
	}
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		dllmodule = hModule;
		HMODULE m_handle = GetModuleHandleA(NULL);
		DWORD bbb = (DWORD)m_handle;
		HANDLE hello = FindWindowA(NULL, "Author : 哎哟哥哥嗨你好");
		if (hello != NULL) {
			Hook(0x0351C853 - 0x400000 + bbb, NULL);
			deDetourThread = CreateThread(NULL, NULL, &dwWaitThread, NULL, NULL, NULL);
		}
	}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void DeDetourfunc() {
	/*DWORD lpOld;
	HMODULE m_handle = GetModuleHandleA(NULL);
	DWORD bbb = (DWORD)m_handle;
	BYTE *src = (BYTE*)(0x0351C853 - 0x400000 + bbb);
	VirtualProtect(src, 5, PAGE_EXECUTE_READWRITE, &lpOld);
	src[0] = 0x5F;
	src[1] = 0xFF;
	src[2] = 0x74;
	src[3] = 0x24;
	src[4] = 0x30;
	VirtualProtect(src, 5, lpOld, &lpOld);*/
	/*__asm {
		mov edi, dword ptr [015C5ED0h]
		mov dword ptr [edi + 00003394h], 00000000h
	}*/
	HMODULE m_handle = GetModuleHandleA(NULL);
	DWORD bbb = (DWORD)m_handle;
	DWORD lpOld;
	PDWORD pHero = (PDWORD)(0x015C5ED0 - 0x400000 + bbb);
	VirtualProtect(pHero, 4, PAGE_EXECUTE_READWRITE, &lpOld);
	DWORD HeroData = *pHero;
	PDWORD ppSkinNo = (PDWORD)(HeroData + 0x3394);
	VirtualProtect(pHero, 4, lpOld, &lpOld);

	VirtualProtect(ppSkinNo, 4, PAGE_EXECUTE_READWRITE, &lpOld);
	PDWORD pSkinNo = (PDWORD)*ppSkinNo;
	VirtualProtect(ppSkinNo, 4, lpOld, &lpOld);

	VirtualProtect(pSkinNo, 4, PAGE_EXECUTE_READWRITE, &lpOld);
	*pSkinNo = 0x0;
	VirtualProtect(pSkinNo, 4, lpOld, &lpOld);
}

LPVOID DetourFunc(BYTE *src, const BYTE *dst, int len, int TYPE, PMEMORYINFO lpMemInfo)
{
	if (lpMemInfo) {
		lpMemInfo->dwAddr = (DWORD)src;
		lpMemInfo->len = len;
		lpMemInfo->lpOriginCode = (PBYTE)malloc(len + 0x5);
		memcpy_m(lpMemInfo->lpOriginCode, src, len);
		lpMemInfo->IsHook = TRUE;
	}
	DWORD lpOld;
	//DWORD lpOld2;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &lpOld);
	
	switch (TYPE) {
	case DETOUR_TYPE_JMP:
		src[0] = 0xE9;
		break;
	case DETOUR_TYPE_CALL:
		src[0] = 0xE8;
		break;
	}
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	BYTE* Addr = src + 5;
	BYTE* dstination = src + len;

	BYTE* pCode = lpMemInfo->lpOriginCode + len;
	*pCode = 0xE9;
	*(DWORD*)(pCode + 0x1) = (DWORD)(dstination - pCode) - 5;

	while (Addr != dstination)
	{
		*Addr = 0x90;
		Addr++;
	}
	VirtualProtect(src, len, lpOld, &lpOld);
	VirtualProtect((LPVOID)lpMemInfo->lpOriginCode, len + 0x5, PAGE_EXECUTE_READWRITE, &lpOld);
	return (src+len);
}

void *memcpy_m(void *dest, const void *src, int n)
{
	byte *c_dest = (byte *)dest;
	const byte *c_src = (const byte *)src;
	while (n-- > 0)
		*c_dest++ = *c_src++;
	return((void *)dest);
}

char* ANSItoUTF8(const char* szAnsi)
{
	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	//预转换，得到所需空间的大小，这次用的函数和上面名字相反
	int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	//同上，分配空间要给'\0'留个空间
	//UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
	char* szU8 = new char[u8Len + 1];
	//转换
	//unicode版对应的strlen是wcslen
	::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
	//最后加上'\0'
	szU8[u8Len] = '\0';
	return szU8;
}



BOOL ReplaceSkin(char* HeroName, DWORD* SkinID, DWORD esp1) {
	int WantedSkinId;
	WantedSkinId = GetPrivateProfileIntA("SkinInfo", HeroName, 0, ".\\LoLSkin.ini");
	if (replace < 2 && WantedSkinId != 0) {
		*SkinID = WantedSkinId;
		replace++;
		char *ansistr = new char[40];
		int strlong = GetPrivateProfileStringA("SkinInfo", "WantedName", "", ansistr, DWORD(40), ".\\LoLSkin.ini");
		if (strlong > 0)
		{
			wantedName = ANSItoUTF8(ansistr);
			DWORD lpOld;
			VirtualProtect((PDWORD)(esp1 + 24), 40, PAGE_EXECUTE_READWRITE, &lpOld);
			*(PDWORD)(esp1 + 24) = (DWORD)wantedName;
			VirtualProtect((PDWORD)(esp1 + 24), 40, lpOld, &lpOld);
		}
	}

	return TRUE;
}

void _stdcall _Skin_Data(char* HeroName, DWORD SkinNo, DWORD esp1) {
	re_SkinID = SkinNo;
	if (re_SkinID == 0x0) {
		ReplaceSkin(HeroName, &re_SkinID,esp1);
	}
}

void __declspec(naked) _Skin() {
	__asm {
			pushfd
			pushad
			push esp
			push dword ptr [esp + 36 + 8 + 0x30]
			push dword ptr [esp + 8]
			call _Skin_Data
		    popad
			popfd
			pop edi
			push re_SkinID
			jmp reCall
	}
}


BOOL Hook(DWORD Address, DWORD* RE_Addr) {
	reCall = (DWORD)DetourFunc((BYTE*)Address, (BYTE*)_Skin, 5, DETOUR_TYPE_JMP, &memoryinfo);
	return TRUE;
}



