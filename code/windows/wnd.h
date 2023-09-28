#pragma once
#include <Windows.h>
#include <string>
#include <time.h>


class Wnd
{
public:
	const static size_t WINDOWS_INDEX = 0;
	const static UINT_PTR TIMER_ID_1 = 1;
	virtual const WCHAR* GetTitle() = 0;
	virtual const WCHAR* GetWindowsClass() = 0;
	virtual void SetWndProc(WNDCLASSEXW& wcex) = 0;
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	void InitWnd(HWND hWnd);
	void DestroyWnd(HWND hWnd);
	void OnPaint(HWND hWnd, clock_t now_clock);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnSetTimer(HWND hWnd);
	void OnKillTimer(HWND hWnd);
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTimer_1(HWND hWnd, LPARAM lParam);

private:
	clock_t m_last_clock = 0;
	HANDLE hThread1 = nullptr;
	HWND m_hWnd = nullptr;
};
