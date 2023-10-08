#pragma once
#include <Windows.h>
#include <string>
#include <time.h>
#include "wnd.h"


class Wnd0 : public Wnd
{
public:
	const static size_t WINDOWS_INDEX = 0;
	const static UINT_PTR TIMER_ID_1 = 1;
	static Wnd0& Instance();
	virtual const WCHAR* GetTitle();
	virtual const WCHAR* GetWindowsClass();
	virtual _beginthreadex_proc_type GetThreadProc();
	virtual size_t GetWindowsIndex();
	virtual UINT_PTR GetThreadId_1();
	virtual WNDPROC GetWNDPROC();
	void InitWnd(HWND hWnd);
	void DestroyWnd(HWND hWnd);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static unsigned CALLBACK ThreadProc(void* argv);
	void OnPaint(HWND hWnd, clock_t now_clock);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnKillTimer(HWND hWnd);
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTimer_1(HWND hWnd, LPARAM lParam);

private:
	clock_t m_last_clock = 0;
	HANDLE hThread1 = nullptr;
	static WCHAR szTitle[];
	static WCHAR szWindowClass[];
};
