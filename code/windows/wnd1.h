#pragma once
#include <Windows.h>
#include <string>
#include <time.h>

class Wnd1
{
public:
	static const size_t WINDOWS_INDEX = 1;
	const static UINT_PTR TIMER_ID_1 = 1;
	static Wnd1& Instance();
	static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	static ATOM MyRegisterClass(HINSTANCE hInstance);
	virtual const WCHAR* GetTitle();
	virtual const WCHAR* GetWindowsClass();
	void InitWnd(HWND hWnd);
	void DestroyWnd(HWND hWnd);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static unsigned CALLBACK Thread1(void* argv);
	void OnPaint(HWND hWnd, clock_t now_clock);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnSetTimer(HWND hWnd);
	void OnKillTimer(HWND hWnd);
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTimer_1(HWND hWnd, LPARAM lParam);


private:
	clock_t m_last_clock = 0;
	HANDLE hThread1 = nullptr;
	static WCHAR szTitle[];
	static WCHAR szWindowClass[];
};
