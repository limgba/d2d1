#include "objmgr.h"
#include "obj.h"
#include "rigidbodymgr.h"
#include "controlobjmgr.h"
#include "animation/animationmgr.h"
#include "animation/animation.h"
#include <algorithm>
#include "image/imagebase.h"

ObjMgr& ObjMgr::Instance()
{
	static ObjMgr mgr;
	return mgr;
}

void ObjMgr::DeleteAllObj()
{
	for (auto pair : m_obj_unordered_map)
	{
		if (nullptr == pair.second)
		{
			continue;
		}
		this->RemoveLink(pair.second);

		delete pair.second;
		pair.second = nullptr;
	}
}

void ObjMgr::AddObj(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	m_obj_unordered_map.emplace(obj->GetObjId(), obj);
	RigidBodyMgr::Instance().PushRigidBody(obj);
}

void ObjMgr::DeleteObj(int obj_id)
{
	auto it = m_obj_unordered_map.find(obj_id);
	if (it == m_obj_unordered_map.end())
	{
		return;
	}
	this->RemoveLink(it->second);

	delete it->second;
	it->second = nullptr;
}

Obj* ObjMgr::GetObj(int obj_id)
{
	auto it = m_obj_unordered_map.find(obj_id);
	if (it == m_obj_unordered_map.end())
	{
		return nullptr;
	}
	return it->second;
}

Obj* ObjMgr::GetObjByPath(const std::wstring& path)
{
	for (auto& pair : m_obj_unordered_map)
	{
		Obj* obj = pair.second;
		if (nullptr == obj)
		{
			continue;
		}
		Animation* animation = obj->GetAnimation();
		if (nullptr == animation)
		{
			continue;
		}
		ImageBase* image_base = animation->GetFirstImageBase();
		if (nullptr == image_base)
		{
			continue;
		}
		if (image_base->GetPath() == path)
		{
			return obj;
		}
	}
	return nullptr;
}

void ObjMgr::RemoveLink(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	RigidBodyMgr::Instance().RemoveRigidBody(obj);
	ControlObjMgr::Instance().RemoveControlObj(obj);
}

