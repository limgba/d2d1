#include <windows.h>
#include "windows/wnd0.h"
#include "3rdpart/d2d1/d2d1mgr.h"
#include <process.h>

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[] = L"d2d1";                  // 标题栏文本
WCHAR szWindowClass[] = L"d2d1";            // 主窗口类名
HANDLE hThread1;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
unsigned CALLBACK Thread1(void* argv)
{
    while (true)
    {
        Sleep(1000);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    while (true)
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
            if (msg.message == WM_QUIT)
            {
                break;
            }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InvalidateRect(msg.hwnd, NULL, false);
		}
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = nullptr;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    hThread1 = (HANDLE)_beginthreadex(nullptr, 0, Thread1, hWnd, 0, nullptr);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        Wnd0::Instance().OnPaint(hWnd);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
    {
		D2D1Mgr::Instance().InitD2D1Mgr();
		D2D1Mgr::Instance().InitD2D1MgrFromWindows(hWnd, 0);
		D2D1Mgr::Instance().LoadAllImage();
        Wnd0::Instance().InitWnd0(hWnd);
    }
    break;
    case WM_DESTROY:
    {
		D2D1Mgr::Instance().DestroyD2D1Mgr();
        Wnd0::Instance().DestroyWnd0(hWnd);
		CloseHandle(hThread1);
        PostQuitMessage(0);
    }
    break;
    case WM_KEYDOWN:
    {
        Wnd0::Instance().OnKeyDown(wParam, lParam);
    }
    break;
    case WM_TIMER:
    {
        Wnd0::Instance().OnTimer(hWnd, wParam, lParam);
    }
    break;
    case WM_MOVE:
    {
        InvalidateRect(hWnd, NULL, false);
    }
    break;
    case WM_SIZE:
    {
        InvalidateRect(hWnd, NULL, false);
    }
    break;
    case WM_ACTIVATE:
    {
        InvalidateRect(hWnd, NULL, false);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

