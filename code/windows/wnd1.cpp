#include "wnd1.h"
#include <process.h>
#include "obj/controlobjmgr.h"
#include "wndmgr.h"
#include "wnddef.h"

WND_DEFINE(1)

void Wnd1::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case 'I':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, -add);
	}
	break;
	case 'J':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), -add, 0);
	}
	break;
	case 'K':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, add);
	}
	break;
	case 'L':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), add, 0);
	}
	break;
	}
}
