#pragma once
#include <Windows.h>
#include <string>

#define TIMER_ID_1 1

class Wnd0
{
public:
	static Wnd0& Instance();
	void InitWnd0(HWND hWnd);
	void DestroyWnd0(HWND hWnd);
	void OnPaint(HWND hWnd);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnSetTimer(HWND hWnd);
	void OnKillTimer(HWND hWnd);
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnTimer_1(HWND hWnd, LPARAM lParam);
};
