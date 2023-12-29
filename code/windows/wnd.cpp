#include "wnd.h"
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
#include "windows/wndmgr.h"




Wnd::Wnd(size_t windows_index)
	: WINDOWS_INDEX(windows_index)
{
}

void Wnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
}

void Wnd::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
}

LRESULT Wnd::WndProcBase(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
		clock_t now_clock = clock();
        BeginPaint(hWnd, &ps);
		this->OnPaint(hWnd, now_clock);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
    {
		if (0 == WINDOWS_INDEX)
		{
			D2D1Mgr::Instance().InitD2D1Mgr();
		}
		D2D1Mgr::Instance().InitD2D1MgrFromWindows(hWnd, WINDOWS_INDEX);
		D2D1Mgr::Instance().LoadAllImage(WINDOWS_INDEX);
		AnimationMgr::Instance().InitAnimationMgr(WINDOWS_INDEX);
		this->InitWnd(hWnd);
    }
    break;
    case WM_DESTROY:
    {
		this->DestroyWnd(hWnd);
		if (0 == WINDOWS_INDEX)
		{
			D2D1Mgr::Instance().DestroyD2D1Mgr();
			AnimationMgr::Instance().DestroyAnimationMgr();
			ObjMgr::Instance().DeleteAllObj();
			PostQuitMessage(0);
		}
    }
    break;
    case WM_KEYDOWN:
    {
        this->OnKeyDown(wParam, lParam);
    }
    break;
    case WM_TIMER:
    {
		this->OnTimer(hWnd, wParam, lParam);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	return 0;
}

HWND Wnd::GetHWND()
{
	return m_hWnd;
}

BOOL Wnd::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	const WCHAR* szTitle = this->GetTitle();
	const WCHAR* szWindowClass = this->GetWindowsClass();
	m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

ATOM Wnd::MyRegisterClass(HINSTANCE hInstance)
{
	const WCHAR* szWindowClass = this->GetWindowsClass();
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	this->SetWndProc(wcex);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

void Wnd::InitWndImpl(HWND hWnd)
{
}

void Wnd::InitWnd(HWND hWnd)
{
	this->InitWndImpl(hWnd);
	CameraMgr::Instance().PushCamera(new Camera(WINDOWS_INDEX));
}

void Wnd::DestroyWndImpl(HWND hWnd)
{
}

void Wnd::DestroyWnd(HWND hWnd)
{
	this->DestroyWndImpl(hWnd);
	CameraMgr::Instance().DeleteCamera(WINDOWS_INDEX);
}

void Wnd::OnPaint(HWND hWnd, clock_t now_clock)
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

			if (0 == WINDOWS_INDEX)
			{
				Obj* obj = new Scene();
				obj->SetAnimation(animation);
				animation->SetObj(obj);
				ObjMgr::Instance().AddObj(obj);
			}
			else
			{
				Obj* obj = ObjMgr::Instance().GetObjByPath(path);
				if (nullptr == obj)
				{
					break;
				}
				animation->SetObj(obj);
			}
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

			if (0 == WINDOWS_INDEX)
			{
				Obj* obj = new Actor();
				obj->SetIsRigidbody(true);
				obj->SetAnimation(animation);
				obj->SetControlIndex(WINDOWS_INDEX);
				animation->SetObj(obj);
				ObjMgr::Instance().AddObj(obj);
				ControlObjMgr::Instance().PushControlObj(obj);
			}
			else
			{
				Obj* obj = ObjMgr::Instance().GetObjByPath(path);
				if (nullptr == obj)
				{
					break;
				}
				animation->SetObj(obj);
			}
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

			if (0 == WINDOWS_INDEX)
			{
				Obj* obj = new Monster();
				obj->SetAnimation(animation);
				obj->SetCoordinate(200, 200);
				obj->SetIsRigidbody(true);
				animation->SetObj(obj);
				ObjMgr::Instance().AddObj(obj);
			}
			else
			{
				Obj* obj = ObjMgr::Instance().GetObjByPath(path);
				if (nullptr == obj)
				{
					break;
				}
				animation->SetObj(obj);
				obj->SetControlIndex(WINDOWS_INDEX);
				ControlObjMgr::Instance().PushControlObj(obj);
			}
		} while (false);

		do
		{
			Animation* animation = new Animation();
			animation->InitClock();
			animation->SetLayer(2.0);
			std::wstring first_path;
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
				if (i == 0 && 0 != WINDOWS_INDEX)
				{
					first_path = path;
				}
			}
			AnimationMgr::Instance().PushAnimation(WINDOWS_INDEX, animation);

			if (0 == WINDOWS_INDEX)
			{
				Obj* obj = new Skill();
				obj->SetAnimation(animation);
				obj->SetCoordinate(700, 300);
				obj->SetIsRigidbody(true);
				animation->SetObj(obj);
				ObjMgr::Instance().AddObj(obj);
			}
			else
			{
				Obj* obj = ObjMgr::Instance().GetObjByPath(first_path);
				if (nullptr == obj)
				{
					break;
				}
				animation->SetObj(obj);
			}
		} while (false);
		AnimationMgr::Instance().DirtyLayer(WINDOWS_INDEX);
	}

	RigidBodyMgr::Instance().CollisionCheck();
	AnimationMgr::Instance().Play(render_target, WINDOWS_INDEX, now_clock);

	render_target->EndDraw();

	clock_t diff_clock = now_clock - m_last_clock;
	m_last_clock = now_clock;
}
