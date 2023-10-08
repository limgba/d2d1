#pragma once
#include <Windows.h>
#include <string>
#include <time.h>
#include <process.h>


class Wnd
{
public:
	virtual const WCHAR* GetTitle() = 0;
	virtual const WCHAR* GetWindowsClass() = 0;
	virtual WNDPROC GetWNDPROC() = 0;
	virtual _beginthreadex_proc_type GetThreadProc() = 0;
	virtual size_t GetWindowsIndex() = 0;
	virtual UINT_PTR GetThreadId_1() = 0;
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	virtual void InitWnd(HWND hWnd);
	virtual void DestroyWnd(HWND hWnd);
	virtual void OnPaint(HWND hWnd, clock_t now_clock);
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnSetTimer(HWND hWnd);
	virtual void OnKillTimer(HWND hWnd);
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTimer_1(HWND hWnd, LPARAM lParam);

private:
	clock_t m_last_clock = 0;
	HANDLE hThread1 = nullptr;
	HWND m_hWnd = nullptr;
};
