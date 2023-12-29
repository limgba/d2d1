#include "wnd0.h"
#include <process.h>
#include "obj/controlobjmgr.h"
#include "wndmgr.h"
#include "wnddef.h"

WND_DEFINE(0)

void Wnd0::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case 'W':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, -add);
	}
	break;
	case 'A':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), -add, 0);
	}
	break;
	case 'S':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, add);
	}
	break;
	case 'D':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), add, 0);
	}
	break;
	}
}
