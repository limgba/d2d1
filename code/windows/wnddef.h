#pragma once

#define WND_DEFINE(Num) \
Wnd##Num::Wnd##Num()\
	: Wnd(Num)\
{\
}\
const WCHAR* Wnd##Num::GetTitle()\
{\
	return L"GameWindows"#Num;\
}\
const WCHAR* Wnd##Num::GetWindowsClass()\
{\
	return L"GameWindows"#Num;\
}\
void Wnd##Num::SetWndProc(WNDCLASSEXW& wcex)\
{\
	wcex.lpfnWndProc = WndProc;\
}\
void Wnd##Num::InitWndImpl(HWND hWnd)\
{\
	hThread1 = (HANDLE)_beginthreadex(nullptr, 0, Thread1, hWnd, 0, nullptr);\
	this->OnSetTimer(hWnd);\
}\
void Wnd##Num::DestroyWndImpl(HWND hWnd)\
{\
	CloseHandle(hThread1);\
	this->OnKillTimer(hWnd);\
}\
LRESULT CALLBACK Wnd##Num::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)\
{\
	Wnd* wnd = WndMgr::Instance().GetWndByIndex(Num);\
	if (nullptr == wnd)\
	{\
		return 0;\
	}\
	return wnd->WndProcBase(hWnd, message, wParam, lParam);\
}\
unsigned Wnd##Num::Thread1(void* argv)\
{\
	while (true)\
	{\
		Sleep(1000);\
	}\
	return 0;\
}\
void Wnd##Num::OnSetTimer(HWND hWnd)\
{\
	SetTimer(hWnd, TIMER_ID_1, 55, nullptr);\
}\
void Wnd##Num::OnKillTimer(HWND hWnd)\
{\
	KillTimer(hWnd, TIMER_ID_1);\
}\
void Wnd##Num::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)\
{\
	switch (wParam)\
	{\
	case TIMER_ID_1:\
	{\
		this->OnTimer_1(hWnd, lParam);\
	}\
	break;\
	}\
}\
void Wnd##Num::OnTimer_1(HWND hWnd, LPARAM lParam)\
{\
	InvalidateRect(hWnd, NULL, false);\
}

