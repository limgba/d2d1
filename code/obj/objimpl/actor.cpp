#include "actor.h"
#include "animation/animation.h"
#include "image/imagebase.h"
#include "obj/obj.h"
#include "obj/controlobjmgr.h"
#include <d2d1.h>

void Actor::OnImpact(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	switch (obj->GetObjType())
	{
	case BITMAP_IMAGE_OBJ_TYPE_SKILL:
	{
		ControlObjMgr::Instance().SetObjCoordinate(0, 0);
	}
	break;
	case BITMAP_IMAGE_OBJ_TYPE_MONSTER:
	{
		ID2D1Bitmap* my_bitmap = this->GetAnimation()->GetImageBase()->GetID2D1Bitmap();
		ID2D1Bitmap* obj_bitmap = obj->GetAnimation()->GetImageBase()->GetID2D1Bitmap();
		if (nullptr == my_bitmap || nullptr == obj_bitmap)
		{
			return;
		}
		D2D1_SIZE_U my_size = my_bitmap->GetPixelSize();
		D2D1_SIZE_U obj_size = obj_bitmap->GetPixelSize();
		double mcx = my_size.width / 2.0;
		double mcy = my_size.height / 2.0;
		double ocx = obj_size.width / 2.0;
		double ocy = obj_size.height / 2.0;
		double max_distance_x = mcx + ocx;
		double max_distance_y = mcy + ocy;

		double mx = this->x() + mcx;
		double my = this->y() + mcy;
		double ox = obj->x() + ocx;
		double oy = obj->y() + ocy;
		double distance_x = mx - ox;
		double distance_y = my - oy;
		int add = 10;
		int offset = 1;
		double ddx = max_distance_x - std::abs(distance_x);
		double ddy = max_distance_y - std::abs(distance_y);
		if (ddx < add)
		{
			if (distance_x > 0)
			{
				ControlObjMgr::Instance().ChangeObjCoordinate(ddx + offset, 0);
			}
			else
			{
				ControlObjMgr::Instance().ChangeObjCoordinate(-ddx - offset, 0);
			}
		}
		if (ddy < add)
		{
			if (distance_y > 0)
			{
				ControlObjMgr::Instance().ChangeObjCoordinate(0, ddy + offset);
			}
			else
			{
				ControlObjMgr::Instance().ChangeObjCoordinate(0, -ddy - offset);
			}
		}
	}
	break;

	}
}
