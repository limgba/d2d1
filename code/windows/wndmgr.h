#pragma once
#include <Windows.h>
#include <string>
#include <time.h>
#include <vector>

class Wnd;

class WndMgr
{
public:
	static WndMgr& Instance();
	Wnd* GetWnd(size_t index);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
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
	std::vector<Wnd*> m_wnd_vector;
};
