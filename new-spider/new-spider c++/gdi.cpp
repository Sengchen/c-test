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
	Render_WND.hCursor = LoadCursor(instance, IDC_ARROW);	//�����
	Render_WND.hIcon = LoadIcon(instance, IDI_APPLICATION);	//ͼ����
	Render_WND.lpszMenuName = NULL;	//�˵���
	Render_WND.style = CS_HREDRAW | CS_VREDRAW;	//���ڵķ��
	Render_WND.hbrBackground = (HBRUSH)COLOR_WINDOW;	//����ɫ
	Render_WND.lpfnWndProc = DefWindowProc;	//���ؼ��������Զ�����Ϣ��������Ҳ������Ĭ�ϵ�DefWindowProc
	Render_WND.lpszClassName = _T("RenderWindow");	//���ؼ����ô����������
	Render_WND.hInstance = instance;	//���ؼ�����ʾ�����ô��ڵĳ��������ʵ�����
	if (!RegisterClass(&Render_WND))
	{
		MessageBox(NULL, _T("RegisterClass Fialed!"), _T("Error"), MB_ICONERROR);
	}

	// ����һ������ class name must same
	HWND hwnd = CreateWindow(Render_WND.lpszClassName, _T("��ϲ�������Ϸ������"), WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, instance, NULL); 
	
	
	// ȥ������
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_CAPTION);
	// ȥ�߿�
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME));
	// ��ʾ����  
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// ��Ϣѭ��  
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
	printf("ֹͣ...\n");
	GdiplusShutdown(p_gdiToken);
	return;
}