#include "wnd2.h"
#include <process.h>
#include "obj/controlobjmgr.h"
#include "wndmgr.h"
#include "wnddef.h"

WND_DEFINE(2)

void Wnd2::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case '3':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, -add);
	}
	break;
	case '1':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), -add, 0);
	}
	break;
	case '2':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, add);
	}
	break;
	case '4':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), add, 0);
	}
	break;
	}
}
