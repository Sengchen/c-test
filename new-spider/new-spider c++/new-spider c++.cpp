// new-spider c++.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <stdlib.h>

#define num 1

int main()
{
	using namespace std;

	LPCWSTR lpsz[num] = { L"cmd.exe" };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return -1;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	int k(num);

	HANDLE hProcess;

	BOOL has = Process32First(hSnapshot, &pe);
	cout << has << endl;
	while (has)
	{
		wprintf(L"%s\n", pe.szExeFile);
		has = Process32Next(hSnapshot, &pe);
	}

	CloseHandle(hSnapshot);

	MessageBox(NULL, L"已读取全部进程", L"SUCCESS", MB_OK);

	system("pause");
    return 0;
}

