// new-spider c++.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <stdlib.h>

#include "new-spider c++.h"
#include "object.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

#define num 1

void processList()
{
	LPCWSTR lpsz[num] = { L"cmd.exe" };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"读取进程失败", L"FAILED", E_MBN_FAILURE);
		return ;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	int k(num);

	BOOL has = Process32First(hSnapshot, &pe);
	cout << has << endl;
	while (has)
	{
		wprintf(L"%s\n", pe.szExeFile);
		has = Process32Next(hSnapshot, &pe);
	}

	CloseHandle(hSnapshot);

	MessageBox(NULL, L"已读取全部进程", L"SUCCESS", MB_OK);
}

void hash_code_test(string test)
{
	object obj;
	int hash = obj.hashCode(test.c_str());
	cout << test << " : " << hash << endl;

	object *obj2 = new object();
	hash = obj2->hashCode(test.c_str());
	cout << test << " : " << hash << endl;
}

void json()
{
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document document;
#if 1
	if (document.Parse(json).HasParseError())
		return ;
#else
	char buffer[sizeof(json)];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())
		return;
#endif // 0
	document.Parse(json);
	printf("Parse to Document Success!\n");

	Value &value = document["stars"];
	if (document.HasMember("project") && document["project"].IsString())
	{
		cout << document["project"].GetString() << endl;
	}
}

extern void http();
extern void file();
extern void address_object_test();
extern void address_point_object_test();

extern void download();

int main()
{
	// http();
	// file();

	// address_point_object_test();

	download();
	system("pause");
    return 0;
}

