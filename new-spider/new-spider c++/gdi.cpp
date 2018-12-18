#include "stdafx.h"

#include <iostream>
#include <string.h>
#include <afx.h>
#include <gdiplus.h>

#include "resource.h"

#pragma comment(lib, "GdiPlus.lib")

using namespace std;
using namespace Gdiplus;

Gdiplus::Image* readImage(UINT resId, LPCTSTR res, HWND* hwnd)
{
	// HINSTANCE hInst = AfxGetResourceHandle();
	HINSTANCE instance = GetModuleHandle(NULL);
	HRSRC hrSrc = FindResource(instance, MAKEINTRESOURCE(resId), res);
	if (hrSrc)
	{
		DWORD len = SizeofResource(instance, hrSrc);
		BYTE* byte = (BYTE*)LoadResource(instance, hrSrc);
		if (byte)
		{
			HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
			BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
			memcpy(pmem, hrSrc, len);
			IStream* pstm;
			CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
			Gdiplus::Image* img = Gdiplus::Image::FromStream(pstm, TRUE);

			GlobalUnlock(m_hMem);
			pstm->Release();
			FreeResource(hrSrc);

			return img;
		}
	}

	return NULL;
}

void gdi()
{
	HINSTANCE instance;
	instance = GetModuleHandle(NULL);

	WNDCLASS Render_WND;
	Render_WND.cbClsExtra = 0;
	Render_WND.cbWndExtra = 0;
	Render_WND.hCursor = LoadCursor(instance, IDC_ARROW);	//鼠标风格
	Render_WND.hIcon = LoadIcon(instance, IDI_APPLICATION);	//图标风格
	Render_WND.lpszMenuName = NULL;	//菜单名
	Render_WND.style = CS_HREDRAW | CS_VREDRAW;	//窗口的风格
	Render_WND.hbrBackground = (HBRUSH)COLOR_WINDOW;	//背景色
	Render_WND.lpfnWndProc = DefWindowProc;	//【关键】采用自定义消息处理函数，也可以用默认的DefWindowProc
	Render_WND.lpszClassName = _T("RenderWindow");	//【关键】该窗口类的名称
	Render_WND.hInstance = instance;	//【关键】表示创建该窗口的程序的运行实体代号
	if (!RegisterClass(&Render_WND))
	{
		MessageBox(NULL, _T("RegisterClass Fialed!"), _T("Error"), MB_ICONERROR);
	}

	// 建立一个窗口 class name must same
	HWND hwnd = CreateWindow(Render_WND.lpszClassName, _T("我喜欢玩儿游戏！！！"), WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, instance, NULL); 
	
	
	// 去标题栏
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_CAPTION);
	// 去边框
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME));
	// 显示窗口  
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// 消息循环  
	/*MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	ULONG_PTR p_gdiToken;
	GdiplusStartupInput gdi_plus_startup_input;
	GdiplusStartup(&p_gdiToken, &gdi_plus_startup_input, NULL);
	
	// Image* img = readImage(IDB_PNG1, _T("PNG"), &hwnd);
	
	// HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hrSrc = FindResource(instance, MAKEINTRESOURCE(IDB_PNG1), _T("PNG"));
	if (hrSrc)
	{
		DWORD len = SizeofResource(instance, hrSrc);
		BYTE* byte = (BYTE*)LoadResource(instance, hrSrc);
		if (byte)
		{
			HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
			BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
			memcpy(pmem, hrSrc, len);
			IStream* pstm;
			CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
			cout << sizeof(pstm) << endl;
			Gdiplus::Image* img = Gdiplus::Image::FromStream(pstm, TRUE);

			GlobalUnlock(m_hMem);
			pstm->Release();
			FreeResource(hrSrc);
			RECT re;
			re.bottom = 200; re.left = 0; re.right = 200; re.top = 0;
			Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromHWND(hwnd);
			// Gdiplus::Image image(_T("C:/Users/Administrator/Pictures/baidushangqiao invite background.png"));
			graphics->DrawImage(img, 0, 0);
		}
	}
	
	
	system("pause");
	printf("停止...\n");
	GdiplusShutdown(p_gdiToken);
	return;
}