#pragma once
#ifndef ADDRESSHOOK_H
#define ADDRESSHOOK_H __declspec(dllimport)
#endif // ADDRESSHOOK_H

#define DETOUR_TYPE_JMP 1
#define DETOUR_TYPE_CALL 2
typedef unsigned char byte;

typedef struct MEMORYINFO {
	BOOL IsHook;
	DWORD dwAddr;
	int len;
	BYTE* lpOriginCode;
}MEMORYINFO;
typedef MEMORYINFO* PMEMORYINFO;

LPVOID DetourFunc(BYTE *src, const BYTE *dst, int len, int TYPE, PMEMORYINFO lpMemInfo);

void *memcpy_m(void *dest, const void *src, int n);

MEMORYINFO memoryinfo;


DWORD re_SkinID = 0 , reCall; // 返回SkinID 回调地址
char *wantedName;
HMODULE dllmodule;
BOOL Hook(DWORD Address, DWORD* RE_Addr);
int replace = 0;

HANDLE deDetourThread = NULL;

void DeDetourfunc();