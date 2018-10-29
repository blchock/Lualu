// LuaLu.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "LuaLu.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LUALU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUALU));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUALU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_LUALU));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	lua = new LuaObject;
	hInst = hInstance; // 将实例句柄存储在全局变量中

	int screenW = GetSystemMetrics(SM_CXSCREEN);	//获取屏幕宽度
	int screenH = GetSystemMetrics(SM_CYSCREEN); //获取屏幕高度
	int sX = (screenW-lua->config_W-lua->config_WD)/2;
	int sY = (screenH-lua->config_H-lua->config_HD)/2;
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		sX, sY, lua->config_W+lua->config_WD, lua->config_H+lua->config_HD, NULL, NULL, hInstance, NULL);
	//获取对话框资源
	lua->INPUTBOX = IDD_INPUTBOX;
	lua->IDCINP = IDC_INP;
	lua->IDCSHOW = IDC_SHOW;

	if (!hWnd)
	{
		return FALSE;
	}
	SetWindowText(hWnd,lua->config_Title);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		//// 分析菜单选择:
		//switch (wmId)
		//{
		//default:
		//	return DefWindowProc(hWnd, message, wParam, lParam);
		//}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(!lua->beginPaint)
		{
			lua->beginPaint = TRUE;
			lua->hdc = CreateCompatibleDC(hdc);
			lua->canvasDC = CreateCompatibleDC(hdc);			//显示设备
			lua->bgDC = CreateCompatibleDC(hdc);
			lua->saveDC = CreateCompatibleDC(hdc);
			//HBITMAP bmp = (HBITMAP)LoadImage(NULL,"1.bmp",IMAGE_BITMAP,160,96,LR_LOADFROMFILE);	加载背景
			paintBMP = CreateCompatibleBitmap(hdc,lua->config_W,lua->config_H);
			canvasBMP = CreateCompatibleBitmap(hdc,lua->config_W,lua->config_H);			//显示位图
			bgBMP = CreateCompatibleBitmap(hdc,lua->config_W,lua->config_H);
			saveBMP = CreateCompatibleBitmap(hdc,lua->config_W*SAVEMUTIL,lua->config_H*SAVEMUTIL);
			SelectObject(lua->hdc,paintBMP);
			SelectObject(lua->canvasDC,canvasBMP);
			SelectObject(lua->bgDC,bgBMP);
			SelectObject(lua->saveDC,saveBMP);
			lua->paintTemp = &paintBMP;
			lua->canvasTemp = &canvasBMP;
			lua->bgTemp = &bgBMP;
			lua->saveTemp = &saveBMP;
			BitBlt(lua->canvasDC,0,0,160,96,lua->hdc,0,0,SRCCOPY);
			//
			lua->hWnd = hWnd;
			lua->hInst = hInst;
			lua->Run(lua->config_Script);
		}
		BitBlt(hdc,0,0,lua->config_W,lua->config_H,lua->canvasDC,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		lua->keyText = new char[50];
		::GetKeyNameText(lParam,lua->keyText,50);
		lua->keyDown();
		return 0;
	case WM_KEYUP:
		lua->keyText = new char[50];
		::GetKeyNameText(lParam, lua->keyText, 50);
		lua->keyUp();
		return 0;
	case WM_LBUTTONDOWN:
		lua->mouseDown("lb",LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		lua->mouseDown("rb",LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_MBUTTONDOWN:
		lua->mouseDown("mb",LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		lua->mouseUp("lb", LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		lua->mouseUp("rb", LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MBUTTONUP:
		lua->mouseUp("mb", LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		//开启MOUSEHOVER支持
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = hWnd;//指定要追踪的窗口
		csTME.dwHoverTime = 10;  //鼠标在按钮上停留超过10ms，才认为状态为HOVER
		TrackMouseEvent(&csTME); //开启Windows的WM_MOUSELEAVE，WM_MOUSEHOVER事件支持
		break;
	case WM_MOUSEHOVER:
		lua->mouseHover(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_DESTROY:
		delete lua;
		DeleteObject(paintBMP);
		DeleteObject(canvasBMP);
		DeleteObject(bgBMP);
		DeleteObject(saveBMP);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
