#include "controlobjmgr.h"
#include "obj.h"
#include "rigidbodymgr.h"
#include <algorithm>

ControlObjMgr& ControlObjMgr::Instance()
{
	static ControlObjMgr mgr;
	return mgr;
}

void ControlObjMgr::PushControlObj(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	m_control_obj_list.push_back(obj);
}

void ControlObjMgr::RemoveControlObj(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	auto delete_it = std::remove(m_control_obj_list.begin(), m_control_obj_list.end(), obj);
	if (delete_it == m_control_obj_list.end())
	{
		return;
	}
	m_control_obj_list.erase(delete_it, m_control_obj_list.end());
}

void ControlObjMgr::SetObjCoordinate(int x, int y)
{
	for (auto* obj : m_control_obj_list)
	{
		obj->SetCoordinate(x, y);
		RigidBodyMgr::Instance().OnObjMove(obj);
	}
	RigidBodyMgr::Instance().CollisionCheck();
}

void ControlObjMgr::ChangeObjCoordinate(int x, int y)
{
	for (auto* obj : m_control_obj_list)
	{
		obj->SetCoordinate(obj->x() + x, obj->y() + y);
		RigidBodyMgr::Instance().OnObjMove(obj);
	}
	RigidBodyMgr::Instance().CollisionCheck();
}

