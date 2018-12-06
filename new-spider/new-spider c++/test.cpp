#include "stdafx.h"

#include <iostream>
#include <string>
#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#pragma  comment(lib,"ws2_32.lib") //套接字编程需要的库文件

#include <fstream>

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

	using namespace std;
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

void http()
{
	using namespace std;

	string host = "localhost";

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
		ofs.close();
	}
	else {
		cout << "初始化套接字失败" << endl;
		return;
	}
}