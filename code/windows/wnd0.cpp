#include "wnd0.h"
#include "wndmgr.h"
#include <process.h>
#include "3rdpart/d2d1/d2d1mgr.h"
#include "image/imagebase.h"
#include "animation/animation.h"
#include "animation/animationmgr.h"
#include "obj/objmgr.h"
#include "obj/controlobjmgr.h"
#include "obj/rigidbodymgr.h"
#include "obj/objimpl/scene.h"
#include "obj/objimpl/actor.h"
#include "obj/objimpl/skill.h"
#include "obj/objimpl/monster.h"
#include "camera/cameramgr.h"


Wnd0& Wnd0::Instance()
{
	static Wnd0 instance;
	return instance;
}


const WCHAR* Wnd0::GetTitle()
{
	return L"d2d1_0";
}

const WCHAR* Wnd0::GetWindowsClass()
{
	return L"d2d1_0";
}

_beginthreadex_proc_type Wnd0::GetThreadProc()
{
	return Wnd0::ThreadProc;
}

size_t Wnd0::GetWindowsIndex()
{
	return WINDOWS_INDEX;
}

UINT_PTR Wnd0::GetThreadId_1()
{
	return TIMER_ID_1;
}

WNDPROC Wnd0::GetWNDPROC()
{
	return Wnd0::WndProc;
}

LRESULT CALLBACK Wnd0::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Wnd* wnd = WndMgr::Instance().GetWnd(WINDOWS_INDEX);
	if (nullptr == wnd)
	{
		return 0;
	}
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
		clock_t now_clock = clock();
        BeginPaint(hWnd, &ps);
        wnd->OnPaint(hWnd, now_clock);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
    {
		D2D1Mgr::Instance().InitD2D1Mgr();
		D2D1Mgr::Instance().InitD2D1MgrFromWindows(hWnd, WINDOWS_INDEX);
		D2D1Mgr::Instance().LoadAllImage(WINDOWS_INDEX);
		AnimationMgr::Instance().InitAnimationMgr(WINDOWS_INDEX);
        wnd->InitWnd(hWnd);
    }
    break;
    case WM_DESTROY:
    {
        wnd->DestroyWnd(hWnd);
		D2D1Mgr::Instance().DestroyD2D1Mgr();
		AnimationMgr::Instance().DestroyAnimationMgr();
		ObjMgr::Instance().DeleteAllObj();
        PostQuitMessage(0);
    }
    break;
    case WM_KEYDOWN:
    {
        wnd->OnKeyDown(wParam, lParam);
    }
    break;
    case WM_TIMER:
    {
        wnd->OnTimer(hWnd, wParam, lParam);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	return 0;
}

unsigned Wnd0::ThreadProc(void* argv)
{
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}


void Wnd0::OnPaint(HWND hWnd, clock_t now_clock)
{
	WindowsDependent* windows_dependent = D2D1Mgr::Instance().GetWindowsDependent(WINDOWS_INDEX);
	if (nullptr == windows_dependent)
	{
		return;
	}
	ID2D1HwndRenderTarget* render_target = windows_dependent->render_target;
	if (!render_target)
	{
		return;
	}

	render_target->BeginDraw();

	// Clear Background
	render_target->Clear(D2D1::ColorF(0.0f, 0.0f, 0.00f));

	// Draw Ellipse  
	//D2D1_SIZE_F size = render_target->GetSize();

	// Draw Image
	if (0 == m_last_clock)
	{
		do
		{
			std::wstring path = L"background/ndmz.bmp";
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(WINDOWS_INDEX, path);
			if (nullptr == bitmap)
			{
				break;
			}
			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);
			image->SetPath(path);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(0.0);
			animation->PushImageBase(image);
			AnimationMgr::Instance().PushAnimation(WINDOWS_INDEX, animation);

			Obj* obj = new Scene();
			obj->SetAnimation(animation);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
		} while (false);

		do
		{
			std::wstring path = L"picture/white.bmp";
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(WINDOWS_INDEX, path);
			if (nullptr == bitmap)
			{
				break;
			}
			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);
			image->SetPath(path);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(1.0);
			animation->PushImageBase(image);

			AnimationMgr::Instance().PushAnimation(WINDOWS_INDEX, animation);

			Obj* obj = new Actor();
			obj->SetIsRigidbody(true);
			obj->SetAnimation(animation);
			obj->SetControlIndex(WINDOWS_INDEX);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
			ControlObjMgr::Instance().PushControlObj(obj);
		} while (false);

		do
		{
			std::wstring path = L"picture/white1.bmp";
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(WINDOWS_INDEX, path);
			if (nullptr == bitmap)
			{
				break;
			}

			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);
			image->SetPath(path);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(2.0);
			animation->PushImageBase(image);

			AnimationMgr::Instance().PushAnimation(WINDOWS_INDEX, animation);

			Obj* obj = new Monster();
			obj->SetAnimation(animation);
			obj->SetCoordinate(200, 200);
			obj->SetIsRigidbody(true);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
		} while (false);

		do
		{
			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(2.0);
			for (int i = 0; i < 7; ++i)
			{
				std::wstring path = L"picture/xiaojiantou" + std::to_wstring(i) + L".bmp";
				ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(WINDOWS_INDEX, path);
				if (nullptr == bitmap)
				{
					continue;
				}

				ImageBase* image = new ImageBase();
				image->SetID2D1Bitmap(bitmap);
				image->SetIntervalMs(500 + i * 500);
				image->SetNextIndex(i + 1);
				image->SetPath(path);
				animation->PushImageBase(image);
			}
			AnimationMgr::Instance().PushAnimation(WINDOWS_INDEX, animation);

			Obj* obj = new Skill();
			obj->SetAnimation(animation);
			obj->SetCoordinate(700, 300);
			obj->SetIsRigidbody(true);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
		} while (false);
		AnimationMgr::Instance().DirtyLayer(WINDOWS_INDEX);
	}

	RigidBodyMgr::Instance().CollisionCheck();
	AnimationMgr::Instance().Play(render_target, WINDOWS_INDEX, now_clock);

	render_target->EndDraw();

	clock_t diff_clock = now_clock - m_last_clock;
	m_last_clock = now_clock;
}

void Wnd0::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case 'W':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(WINDOWS_INDEX, 0, -add);
	}
	break;
	case 'A':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(WINDOWS_INDEX, -add, 0);
	}
	break;
	case 'S':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(WINDOWS_INDEX, 0, add);
	}
	break;
	case 'D':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(WINDOWS_INDEX, add, 0);
	}
	break;
	}
}

void Wnd0::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TIMER_ID_1:
	{
		this->OnTimer_1(hWnd, lParam);
	}
	break;
	}
}

void Wnd0::OnTimer_1(HWND hWnd, LPARAM lParam)
{
	InvalidateRect(hWnd, NULL, false);
}
