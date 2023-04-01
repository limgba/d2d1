#include "wnd1.h"
#include <process.h>
#include "3rdpart/d2d1/d2d1mgr.h"
#include "image/imagebase.h"
#include "animation/animation.h"
#include "animation/animationmgr.h"
#include "obj/objmgr.h"
#include "obj/controlobjmgr.h"
#include "obj/rigidbodymgr.h"
#include "obj/objimpl/scene.h"
#include "obj/objimpl/actor.h"
#include "obj/objimpl/skill.h"
#include "obj/objimpl/monster.h"

HANDLE Wnd1::hThread1 = nullptr;
WCHAR Wnd1::szTitle[] = L"d2d1_1";
WCHAR Wnd1::szWindowClass[] = L"d2d1_1";

Wnd1& Wnd1::Instance()
{
	static Wnd1 instance;
	return instance;
}

BOOL Wnd1::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

ATOM Wnd1::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Wnd1::WndProc;
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

void Wnd1::InitWnd(HWND hWnd)
{
    hThread1 = (HANDLE)_beginthreadex(nullptr, 0, Thread1, hWnd, 0, nullptr);
	this->OnSetTimer(hWnd);
}

void Wnd1::DestroyWnd(HWND hWnd)
{
	CloseHandle(hThread1);
	this->OnKillTimer(hWnd);
}

LRESULT CALLBACK Wnd1::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
		clock_t now_clock = clock();
        BeginPaint(hWnd, &ps);
        Wnd1::Instance().OnPaint(hWnd, now_clock);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
    {
        Wnd1::Instance().InitWnd(hWnd);
    }
    break;
    case WM_DESTROY:
    {
        Wnd1::Instance().DestroyWnd(hWnd);
		CloseHandle(Wnd1::hThread1);
        //PostQuitMessage(0);
    }
    break;
    case WM_KEYDOWN:
    {
        Wnd1::Instance().OnKeyDown(wParam, lParam);
    }
    break;
    case WM_TIMER:
    {
        Wnd1::Instance().OnTimer(hWnd, wParam, lParam);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	return 0;
}

unsigned Wnd1::Thread1(void* argv)
{
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}


void Wnd1::OnPaint(HWND hWnd, clock_t now_clock)
{
	ID2D1HwndRenderTarget* render_target = D2D1Mgr::Instance().GetID2D1HwndRenderTarget();
	if (!render_target)
	{
		return;
	}

	render_target->BeginDraw();

	// Clear Background
	render_target->Clear(D2D1::ColorF(0.0f, 0.0f, 0.00f));

	// Draw Ellipse  
	D2D1_SIZE_F size = render_target->GetSize();

	// Draw Image
	AnimationMgr::Instance().Play(now_clock);

	render_target->EndDraw();

	clock_t diff_clock = now_clock - m_last_clock;
	m_last_clock = now_clock;
}

void Wnd1::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case 'i':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(0, -add);
	}
	break;
	case 'j':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(-add, 0);
	}
	break;
	case 'k':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(0, add);
	}
	break;
	case 'l':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(add, 0);
	}
	break;
	}
}

void Wnd1::OnSetTimer(HWND hWnd)
{
	SetTimer(hWnd, TIMER_ID_2, 55, nullptr);
}

void Wnd1::OnKillTimer(HWND hWnd)
{
	KillTimer(hWnd, TIMER_ID_2);
}

void Wnd1::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TIMER_ID_2:
	{
		this->OnTimer_1(hWnd, lParam);
	}
	break;
	}
}

void Wnd1::OnTimer_1(HWND hWnd, LPARAM lParam)
{
	InvalidateRect(hWnd, NULL, false);
}
