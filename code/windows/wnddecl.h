#pragma once

#define WND_DECLARE(Num) \
class Wnd##Num : public Wnd\
{\
private:\
	const static UINT_PTR TIMER_ID_1 = 1;\
	HANDLE hThread1 = nullptr;\
public:\
	Wnd##Num();\
	virtual const WCHAR* GetTitle();\
	virtual const WCHAR* GetWindowsClass();\
	virtual void SetWndProc(WNDCLASSEXW& wcex);\
	virtual void InitWndImpl(HWND hWnd);\
	virtual void DestroyWndImpl(HWND hWnd);\
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);\
	static unsigned CALLBACK Thread1(void* argv);\
	void OnSetTimer(HWND hWnd);\
	void OnKillTimer(HWND hWnd);\
	virtual void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);\
	void OnTimer_1(HWND hWnd, LPARAM lParam);


#define WND_DECLARE_END };

