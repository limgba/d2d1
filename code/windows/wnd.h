#pragma once
#include <Windows.h>
#include <string>
#include <time.h>


class Wnd
{
public:
	Wnd(size_t windows_index);
	virtual const WCHAR* GetTitle() = 0;
	virtual const WCHAR* GetWindowsClass() = 0;
	virtual void SetWndProc(WNDCLASSEXW& wcex) = 0;
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	size_t GetWindowsIndex() { return WINDOWS_INDEX; }
	virtual LRESULT WndProcBase(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void InitWndImpl(HWND hWnd);
	virtual void DestroyWndImpl(HWND hWnd);

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	HWND GetHWND();
	void InitWnd(HWND hWnd);
	void DestroyWnd(HWND hWnd);
	void OnPaint(HWND hWnd, clock_t now_clock);

private:
	size_t WINDOWS_INDEX = 0;
	HWND m_hWnd = nullptr;
	clock_t m_last_clock = 0;
};
