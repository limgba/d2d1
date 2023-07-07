#include "camera.h"
#include <d2d1.h>
#include "3rdpart/d2d1/d2d1mgr.h"
#include "animation/animation.h"
#include "image/imagebase.h"
#include "obj/controlobjmgr.h"
#include "obj/objmgr.h"

Camera::Camera(int index)
{
	m_index = index;
}

void Camera::ResetCoordinate()
{
	WindowsDependent* windows_dependent = D2D1Mgr::Instance().GetWindowsDependent(m_index);
	if (nullptr == windows_dependent)
	{
		return;
	}
	ID2D1HwndRenderTarget* render_target = windows_dependent->render_target;
	if (nullptr == render_target)
	{
		return;
	}
	Obj* scene = ObjMgr::Instance().GetObjByType(BITMAP_IMAGE_OBJ_TYPE_SCENE);
	if (nullptr == scene)
	{
		return;
	}
	D2D1_SIZE_U scene_size;
	{
		Animation* animation = scene->GetAnimation();
		if (nullptr == animation)
		{
			return;
		}
		ImageBase* image_base = animation->GetImageBase();
		if (nullptr == image_base)
		{
			return;
		}
		ID2D1Bitmap* bitmap = image_base->GetID2D1Bitmap();
		if (nullptr == bitmap)
		{
			return;
		}
		scene_size = bitmap->GetPixelSize();
	}

	Obj* control_obj = ControlObjMgr::Instance().GetControlObj(m_index);
	if (nullptr == control_obj)
	{
		return;
	}
	D2D1_SIZE_U control_size;
	{
		Animation* animation = control_obj->GetAnimation();
		if (nullptr == animation)
		{
			return;
		}
		ImageBase* image_base = animation->GetImageBase();
		if (nullptr == image_base)
		{
			return;
		}
		ID2D1Bitmap* bitmap = image_base->GetID2D1Bitmap();
		if (nullptr == bitmap)
		{
			return;
		}
		control_size = bitmap->GetPixelSize();
	}
	D2D1_SIZE_U render_size = render_target->GetPixelSize();
	UINT32 x = control_obj->x() + control_size.width / 2;
	UINT32 y = control_obj->y() + control_size.height / 2;
	if (x < render_size.width / 2 || x > INT_MAX)
	{
		x = render_size.width / 2;
	}
	else if (x > scene_size.width - render_size.width / 2)
	{
		x = scene_size.width - render_size.width / 2;
	}
	if (y < render_size.height / 2 || y > INT_MAX)
	{
		y = render_size.height / 2;
	}
	else if (y > scene_size.height - render_size.height / 2)
	{
		y = scene_size.height - render_size.height / 2;
	}
	this->SetCoordinate(x, y);
}
