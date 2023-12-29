#pragma once
#include <Windows.h>
#include <string>
#include <time.h>
#include "wnd.h"
#include "wnddecl.h"


//class Wnd0 : public Wnd
//{
//public:
//	Wnd0();
//	virtual const WCHAR* GetTitle();
//	virtual const WCHAR* GetWindowsClass();
//	virtual void SetWndProc(WNDCLASSEXW& wcex);
//	virtual void InitWndImpl(HWND hWnd);
//	virtual void DestroyWndImpl(HWND hWnd);
//	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
//	virtual void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
//	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//	static unsigned CALLBACK Thread1(void* argv);
//	void OnSetTimer(HWND hWnd);
//	void OnKillTimer(HWND hWnd);
//	void OnTimer_1(HWND hWnd, LPARAM lParam);
//
//private:
//	const static UINT_PTR TIMER_ID_1 = 1;
//	HANDLE hThread1 = nullptr;
//};

WND_DECLARE(0)
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
WND_DECLARE_END
