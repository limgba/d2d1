#include <windows.h>
#include "windows/wnd0.h"
#include "windows/wnd1.h"
#include "windows/wndmgr.h"
#include "3rdpart/d2d1/d2d1mgr.h"
#include "3rdpart/yaml/yaml.h"
#include "animation/animationmgr.h"
#include "obj/objmgr.h"

//#define USE_PEEKMESSAGE 1

HINSTANCE hInst;                                // 当前实例

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int ret = 0;
    ret = yaml::ReadFileList();
    if (ret < 0)
    {
        return -1000000 + ret;
    }

    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return -2000000;
    }

    MSG msg;
#ifdef USE_PEEKMESSAGE
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
    }
#else
    HACCEL hAccelTable = LoadAccelerators(hInstance, L"");
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
#endif

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    return WndMgr::Instance().MyRegisterClass(hInstance);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中
    return WndMgr::Instance().InitInstance(hInstance, nCmdShow);
}

