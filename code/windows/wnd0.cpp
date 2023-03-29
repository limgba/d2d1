#include "wnd0.h"
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


Wnd0& Wnd0::Instance()
{
	static Wnd0 instance;
	return instance;
}

void Wnd0::InitWnd0(HWND hWnd)
{
	this->OnSetTimer(hWnd);
}

void Wnd0::DestroyWnd0(HWND hWnd)
{
	this->OnKillTimer(hWnd);
}

void Wnd0::OnPaint(HWND hWnd)
{
	ID2D1HwndRenderTarget* render_target = D2D1Mgr::Instance().GetID2D1HwndRenderTarget(0);
	if (!render_target)
	{
		return;
	}

	render_target->BeginDraw();

	// Clear Background
	render_target->Clear(D2D1::ColorF(0.0f, 0.0f, 0.00f));

	// Draw Ellipse  
	D2D1_SIZE_F size = render_target->GetSize();

	// Draw Image
	if (0 == AnimationMgr::Instance().AnimationSize())
	{
		do
		{
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(L"background/ndmz.bmp");
			if (nullptr == bitmap)
			{
				break;
			}
			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(0.0);
			animation->PushImageBase(image);
			AnimationMgr::Instance().PushAnimation(animation);

			Obj* obj = new Scene();
			obj->SetAnimation(animation);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
		} while (false);

		do
		{
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(L"picture/white.bmp");
			if (nullptr == bitmap)
			{
				break;
			}
			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(1.0);
			animation->PushImageBase(image);

			AnimationMgr::Instance().PushAnimation(animation);

			Obj* obj = new Actor();
			obj->SetIsRigidbody(true);
			obj->SetAnimation(animation);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
			ControlObjMgr::Instance().PushControlObj(obj);
		} while (false);

		do
		{
			ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(L"picture/white1.bmp");
			if (nullptr == bitmap)
			{
				break;
			}

			ImageBase* image = new ImageBase();
			image->SetID2D1Bitmap(bitmap);
			image->SetIntervalMs(0);
			image->SetNextIndex(0);

			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(2.0);
			animation->PushImageBase(image);

			AnimationMgr::Instance().PushAnimation(animation);

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
				ID2D1Bitmap* bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(path);
				if (nullptr == bitmap)
				{
					continue;
				}

				ImageBase* image = new ImageBase();
				image->SetID2D1Bitmap(bitmap);
				image->SetIntervalMs(500 + i * 500);
				image->SetNextIndex(i + 1);
				animation->PushImageBase(image);
			}
			AnimationMgr::Instance().PushAnimation(animation);

			Obj* obj = new Skill();
			obj->SetAnimation(animation);
			obj->SetCoordinate(700, 300);
			obj->SetIsRigidbody(true);
			animation->SetObj(obj);
			ObjMgr::Instance().AddObj(obj);
		} while (false);
		AnimationMgr::Instance().DirtyLayer();
	}

	RigidBodyMgr::Instance().CollisionCheck();
	AnimationMgr::Instance().Play();

	render_target->EndDraw();
}

void Wnd0::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	int add = 10;
	switch (wParam)
	{
	case 'W':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(0, -add);
	}
	break;
	case 'A':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(-add, 0);
	}
	break;
	case 'S':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(0, add);
	}
	break;
	case 'D':
	{
		ControlObjMgr::Instance().ChangeObjCoordinate(add, 0);
	}
	break;
	}
}

void Wnd0::OnSetTimer(HWND hWnd)
{
	SetTimer(hWnd, TIMER_ID_1, 55, nullptr);
}

void Wnd0::OnKillTimer(HWND hWnd)
{
	KillTimer(hWnd, TIMER_ID_1);
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
	//InvalidateRect(hWnd, NULL, false);
}
