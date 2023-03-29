#include "objmgr.h"
#include "obj.h"
#include "rigidbodymgr.h"
#include "controlobjmgr.h"
#include "animation/animationmgr.h"
#include <algorithm>

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

void ObjMgr::RemoveLink(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	RigidBodyMgr::Instance().RemoveRigidBody(obj);
	ControlObjMgr::Instance().RemoveControlObj(obj);
}

