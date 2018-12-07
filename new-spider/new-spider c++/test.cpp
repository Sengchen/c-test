// standard application framework extensions
#include "stdafx.h"

#include <iostream>
#include <string>
#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include <direct.h>

#pragma  comment(lib,"ws2_32.lib") //套接字编程需要的库文件
#pragma comment(lib,"URlmon")

#include <fstream>

using namespace std;

void address_object_test_sub(std::string str)
{
	str = "test_sub_1";
}

void address_object_test_sub(std::string* str)
{
	*str = "test_sub_2";
}

void address_object_test()
{
	std::string test("test");
	std::string* pTest = &test;

	cout << test << endl;
	cout << &test << endl;
	cout << *pTest << endl;
	cout << pTest << endl;
	cout << &pTest << endl;
	cout << &*pTest << endl;

	/*address_object_test_sub(test);
	std::cout << test << std::endl;

	address_object_test_sub(&test);
	std::cout << test << std::endl;*/
}

void address_point_object_test()
{
	/*int var = 20;
	int *ip;
	ip = &var;
	std::cout << ip << ":" << *ip << std::endl;*/

	const int LEN = 3;
	/*int var[LEN] = { 10, 100, 1000 };
	int *ptr;
	ptr = var;
	for (int i = 0; i < LEN; i++)
	{
		std::cout << ptr << std::endl;
		std::cout << *ptr << std::endl;
		ptr++;
	}*/

	/*int var[LEN] = { 10, 100, 1000 };
	int *ptr[LEN];
	for (int i = 0; i < LEN; i++)
	{
		ptr[i] = &var[i];
		cout << ptr[i] << ":" << *ptr[i] << endl;
	}*/

	// ptr是数组首地址
	// ptr[i]是指针值
	// &ptr[i]是指针指向的地址
	/*char* ptr[LEN] = { "chris", "test", "jones" };

	cout << "数组地址:" << ptr << endl;

	for (int i = 0; i < LEN; i++)
	{
		cout << "指针值:" << ptr[i] << endl;
		cout << "指针指向地址:" << &ptr[i] << endl;
	}*/

	int a = 2;
	int* b = &a;
	cout << b << ":" << *b << endl;

	char c = 't';
	char* d = &c;
	cout << d << ":" << *d << endl;
}

void http()
{
	using namespace std;

	string host = "www.baidu.com";

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
	{
		// step 2
		cout << "域名解析..." << endl;
		struct addrinfo hints;
		struct addrinfo *res, *cur;
		struct sockaddr_in *addr;
		char ipbuf[16];

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;     /* Allow IPv4 */
		hints.ai_flags = AI_PASSIVE;/* For wildcard IP address */
		hints.ai_protocol = 0;         /* Any protocol */
		hints.ai_socktype = SOCK_STREAM;

		if (getaddrinfo(host.c_str(), NULL, &hints, &res) != 0)
		{
			cout << "域名解析失败" << endl;
			return;
		}

		for (cur = res; cur != NULL; cur = cur->ai_next) {
			addr = (struct sockaddr_in *)cur->ai_addr;
		}
		
		cout << "ip: " << inet_ntop(AF_INET, &addr->sin_addr, ipbuf, 16) << endl;

		// step 3
		cout << "创建套接字..." << endl;
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1 || sock == -2)
		{
			cout << "套接字创建失败" << endl;
			return;
		}

		// step 4
		SOCKADDR_IN si;
		si.sin_family = AF_INET;
		si.sin_port = htons(80);
		memcpy(&si.sin_addr.S_un.S_addr, &addr->sin_addr.S_un.S_addr, 4);
		// si.sin_addr.S_un.S_addr = (ULONG)&addr->sin_addr.S_un.S_addr;

		// 建立连接
		cout << "开始连接..." << endl;
		if (connect(sock, (SOCKADDR *)&si, sizeof(si)) != 0)
		{
			// WSAETIMEDOUT
			cout << "连接失败: " << WSAGetLastError() << endl;
			return;
		}

		long timeout = 60000;
		int m = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
		int n = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

		// step 5
		string request = "GET / HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
		// 请求
		cout << "发送请求..." << endl;
		if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR)
		{
			cout << "请求发送失败" << endl;
			closesocket(sock);
			return;
		}

		cout << "请求成功" << endl;

		// step 6
		int size = 256;
		char* buff = new char[size];
		memset(buff, 0, size);
		int len = 0;
		int ret = 1;
		while (ret > 0)
		{
			ret = recv(sock, buff + len, size - len, 0);
			if (ret > 0)
			{
				len += ret;
			}
			if (size - len < 100)
			{
				size *= 2;
				char* new_buff = new char[size];
				memset(new_buff, 0, size);
				memcpy(new_buff, buff, size / 2);
				delete[] buff;
				buff = new_buff;
			}
			cout << "*";
		}
		buff[len] = '\0';
		/*ifstream ifs;
		ifs.open("C:/Users/Administrator/Desktop/test.txt", iostream::in | iostream::out | iostream::binary);*/
		ofstream ofs;
		ofs.open("C:/Users/Administrator/Desktop/test.txt", iostream::in | iostream::out | iostream::binary);
		ofs.write(buff, len);
		cout << buff << endl;
		ofs.close();
	}
	else {
		cout << "初始化套接字失败" << endl;
		return;
	}
}

void file()
{
	using namespace std;
	fstream file_stream;
	file_stream.open("C:/Users/Administrator/Documents/QQ号码_2407474463.txt", iostream::in | iostream::out);
	int len = 0;
	
	if (file_stream.is_open())
	{
		while (!file_stream.eof())
		{
			char buffer[1024];
			file_stream.getline(buffer, sizeof(buffer));
			cout << buffer << endl;
		}
	}
	file_stream.clear();
	file_stream.close();
	return;
}

void download()
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	strcat_s(buffer, "/test.jpg");
	HRESULT hResult = URLDownloadToFileA(NULL, "https://img-blog.csdn.net/20170118221744178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXE3ODQ0Mjc2MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center", buffer, 0, NULL);
}