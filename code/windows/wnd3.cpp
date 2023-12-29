#include "wnd3.h"
#include <process.h>
#include "obj/controlobjmgr.h"
#include "wndmgr.h"
#include "wnddef.h"

WND_DEFINE(3)

void Wnd3::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case '9':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, -add);
	}
	break;
	case '7':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), -add, 0);
	}
	break;
	case '8':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), 0, add);
	}
	break;
	case '0':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(this->GetWindowsIndex(), add, 0);
	}
	break;
	}
}
