#pragma once
#include <Windows.h>
#include <string>
#include <time.h>

#define TIMER_ID_2 2

class Wnd1
{
public:
	static Wnd1& Instance();
	static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	static ATOM MyRegisterClass(HINSTANCE hInstance);
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

public:
	static HANDLE hThread1;
	static WCHAR szTitle[];
	static WCHAR szWindowClass[];

private:
	clock_t m_last_clock = 0;
};
