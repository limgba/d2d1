#include "wnd.h"
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
#include "camera/cameramgr.h"


BOOL Wnd::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	const WCHAR* szTitle = this->GetTitle();
	const WCHAR* szWindowClass = this->GetWindowsClass();
	m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

ATOM Wnd::MyRegisterClass(HINSTANCE hInstance)
{
	const WCHAR* szWindowClass = this->GetWindowsClass();
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->GetWNDPROC();
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

void Wnd::InitWnd(HWND hWnd)
{
    hThread1 = (HANDLE)_beginthreadex(nullptr, 0, this->GetThreadProc(), hWnd, 0, nullptr);
	this->OnSetTimer(hWnd);
	CameraMgr::Instance().PushCamera(new Camera(this->GetWindowsIndex()));
}

void Wnd::DestroyWnd(HWND hWnd)
{
	CloseHandle(hThread1);
	this->OnKillTimer(hWnd);
	CameraMgr::Instance().DeleteCamera(this->GetWindowsIndex());
}

void Wnd::OnPaint(HWND hWnd, clock_t now_clock)
{
}

void Wnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
}

void Wnd::OnSetTimer(HWND hWnd)
{
	SetTimer(hWnd, this->GetThreadId_1(), 55, nullptr);
}

void Wnd::OnKillTimer(HWND hWnd)
{
	KillTimer(hWnd, this->GetThreadId_1());
}

void Wnd::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 1:
	{
		this->OnTimer_1(hWnd, lParam);
	}
	break;
	}
}

void Wnd::OnTimer_1(HWND hWnd, LPARAM lParam)
{
	InvalidateRect(hWnd, NULL, false);
}

