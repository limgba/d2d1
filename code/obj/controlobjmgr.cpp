#include "controlobjmgr.h"
#include "obj.h"
#include "rigidbodymgr.h"
#include <algorithm>
#include "camera/cameramgr.h"

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

void ControlObjMgr::SetObjCoordinate(size_t index, int x, int y)
{
	if (index >= m_control_obj_list.size())
	{
		return;
	}
	Obj* obj = m_control_obj_list[index];
	if (nullptr == obj)
	{
		return;
	}
	Camera* camera = CameraMgr::Instance().GetCamera(index);
	if (nullptr == camera)
	{
		return;
	}
	obj->SetCoordinate(x, y);
	RigidBodyMgr::Instance().OnObjMove(obj);
	RigidBodyMgr::Instance().CollisionCheck();
	//camera->ResetCoordinate();
}

void ControlObjMgr::ChangeObjCoordinate(size_t index, int x, int y)
{
	if (index >= m_control_obj_list.size())
	{
		return;
	}
	Obj* obj = m_control_obj_list[index];
	if (nullptr == obj)
	{
		return;
	}
	Camera* camera = CameraMgr::Instance().GetCamera(index);
	if (nullptr == camera)
	{
		return;
	}
	obj->SetCoordinate(obj->x() + x, obj->y() + y);
	RigidBodyMgr::Instance().OnObjMove(obj);
	RigidBodyMgr::Instance().CollisionCheck();
	//camera->ResetCoordinate();
}

Obj* ControlObjMgr::GetControlObj(size_t index)
{
	if (index >= m_control_obj_list.size())
	{
		return nullptr;
	}
	return m_control_obj_list[index];
}

