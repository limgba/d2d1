#include "rigidbodymgr.h"
#include "image/imagebase.h"
#include "animation/animation.h"
#include "obj/obj.h"
#include "shape/shape.h"
#include <algorithm>
#include <d2d1.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

RigidBodyMgr& RigidBodyMgr::Instance()
{
	static RigidBodyMgr mgr;
	return mgr;
}

RigidBodyMgr::RigidBodyMgr()
{
	m_dirty_rigid_body_count = 0;
}

RigidBodyMgr::~RigidBodyMgr()
{
}

void RigidBodyMgr::PushRigidBody(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}

	if (obj->IsRigidBody())
	{
		++m_dirty_rigid_body_count;
		m_rigid_body_list.push_back(obj);
	}
}

void RigidBodyMgr::RemoveRigidBody(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	size_t i = 0;
	for (auto it = m_rigid_body_list.begin(); it != m_rigid_body_list.end(); ++it)
	{
		if (obj->GetObjId() == (*it)->GetObjId())
		{
			m_rigid_body_list.erase(it);
			if (i < m_dirty_rigid_body_count)
			{
				--m_dirty_rigid_body_count;
			}
			break;
		}
		++i;
	}
}

void RigidBodyMgr::OnObjMove(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	if (!obj->IsRigidBody())
	{
		return;
	}
	size_t i = m_dirty_rigid_body_count;
	bool is_find = false;
	for (; i < m_rigid_body_list.size(); ++i)
	{
		if (obj == m_rigid_body_list[i])
		{
			is_find = true;
			break;
		}
	}
	if (!is_find)
	{
		return;
	}
	if (m_dirty_rigid_body_count < m_rigid_body_list.size() && i < m_rigid_body_list.size())
	{
		std::swap(m_rigid_body_list[m_dirty_rigid_body_count], m_rigid_body_list[i]);
		++m_dirty_rigid_body_count;
	}
}

void RigidBodyMgr::CollisionCheck()
{
	if (0 == m_dirty_rigid_body_count)
	{
		return;
	}
	lmb::Rectangle rect1;
	lmb::Rectangle rect2;
	for (size_t i = 0; i < m_dirty_rigid_body_count && i < m_rigid_body_list.size(); ++i)
	{
		auto* obj_1 = m_rigid_body_list[i];
		if (nullptr == obj_1)
		{
			continue;
		}
		for (size_t j = i; j < m_rigid_body_list.size(); ++j)
		{
			auto* obj_2 = m_rigid_body_list[j];
			if (nullptr == obj_2)
			{
				continue;
			}
			if (obj_1 == obj_2)
			{
				continue;
			}
			auto* animation_1 = obj_1->GetAnimation();
			auto* animation_2 = obj_2->GetAnimation();
			if (nullptr == animation_1 || nullptr == animation_2)
			{
				continue;
			}
			auto* image_base_1 = animation_1->GetImageBase();
			auto* image_base_2 = animation_2->GetImageBase();
			if (nullptr == image_base_1 || nullptr == image_base_2)
			{
				continue;
			}
			ID2D1Bitmap* bitmap_image_1 = image_base_1->GetID2D1Bitmap();
			ID2D1Bitmap* bitmap_image_2 = image_base_2->GetID2D1Bitmap();
			D2D1_SIZE_U bitmap_image_size_1 = bitmap_image_1->GetPixelSize();
			D2D1_SIZE_U bitmap_image_size_2 = bitmap_image_2->GetPixelSize();
			rect1.left = obj_1->x();
			rect1.top = obj_1->y();
			rect1.right = obj_1->x() + bitmap_image_size_1.width;
			rect1.bottom = obj_1->y() + bitmap_image_size_1.height;

			rect2.left = obj_2->x();
			rect2.top = obj_2->y();
			rect2.right = obj_2->x() + bitmap_image_size_2.width;
			rect2.bottom = obj_2->y() + bitmap_image_size_2.height;

			if (!lmb::Impact(rect1, rect2))
			{
				continue;
			}
			RigidBodyMgr::Instance().OnImpact(obj_1, obj_2);
		}
	}
	m_dirty_rigid_body_count = 0;
}

void RigidBodyMgr::OnImpact(Obj* obj_1, Obj* obj_2)
{
	if (nullptr == obj_1 || nullptr == obj_2)
	{
		return;
	}
	int obj_type1 = std::min(obj_1->GetObjType(), obj_2->GetObjType());
	int obj_type2 = std::max(obj_1->GetObjType(), obj_2->GetObjType());
	const int obj_type = LeftOffsetShort(obj_type1, obj_type2);
	if (obj_1->GetObjType() != obj_type1)
	{
		std::swap(obj_1, obj_2);
	}
	obj_1->OnImpact(obj_2);
}

