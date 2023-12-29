#pragma once
#include <Windows.h>
#include <string>
#include <time.h>
#include "wnd.h"
#include "wnddecl.h"


WND_DECLARE(3)
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
WND_DECLARE_END
