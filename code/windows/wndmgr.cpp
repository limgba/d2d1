#include "wndmgr.h"
#include "wnd.h"
#include "wnd0.h"
#include "wnd1.h"

WndMgr& WndMgr::Instance()
{
	static WndMgr instance;
	return instance;
}

Wnd* WndMgr::GetWnd(HWND hWnd)
{
	for (Wnd* wnd : m_wnd_vector)
	{
		if (nullptr == wnd)
		{
			continue;
		}
		if (wnd->GetHWND() != hWnd)
		{
			continue;
		}
		return wnd;
	}
	return nullptr;
}

Wnd* WndMgr::GetWndByIndex(size_t index)
{
	if (index >= m_wnd_vector.size())
	{
		return nullptr;
	}
	return m_wnd_vector[index];
}

BOOL WndMgr::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	for (Wnd* wnd : m_wnd_vector)
	{
		if (nullptr == wnd)
		{
			continue;
		}
		if (!wnd->InitInstance(hInstance, nCmdShow))
		{
			return FALSE;
		}
	}
	return TRUE;
}

ATOM WndMgr::MyRegisterClass(HINSTANCE hInstance)
{
	m_wnd_vector.push_back(new Wnd0());
	m_wnd_vector.push_back(new Wnd1());
	ATOM ret = 0;
	for (Wnd* wnd : m_wnd_vector)
	{
		if (nullptr == wnd)
		{
			continue;
		}
		ret &= wnd->MyRegisterClass(hInstance);
	}
	return ret;
}

void WndMgr::InitWnd(HWND hWnd)
{
	for (Wnd* wnd : m_wnd_vector)
	{
		if (nullptr == wnd)
		{
			continue;
		}
		wnd->InitWnd(hWnd);
	}
}

void WndMgr::DestroyWnd(HWND hWnd)
{
	for (Wnd* wnd : m_wnd_vector)
	{
		if (nullptr == wnd)
		{
			continue;
		}
		wnd->DestroyWnd(hWnd);
	}
}


