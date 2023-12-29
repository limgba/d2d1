#pragma once
#include <Windows.h>
#include <vector>

class Wnd;

class WndMgr
{
public:
	static WndMgr& Instance();
	Wnd* GetWnd(HWND hWnd);
	Wnd* GetWndByIndex(size_t index);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	void InitWnd(HWND hWnd);
	void DestroyWnd(HWND hWnd);

private:
	std::vector<Wnd*> m_wnd_vector;
};
