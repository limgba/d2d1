#pragma once
#include <vector>

class Obj;

class RigidBodyMgr
{
public:
	static RigidBodyMgr& Instance();
	RigidBodyMgr();
	~RigidBodyMgr();

	void PushRigidBody(Obj* obj);
	void RemoveRigidBody(Obj* obj);

	void OnObjMove(Obj* obj);
	void CollisionCheck();
	void OnImpact(Obj* obj_1, Obj* obj_2);

private:
	size_t m_dirty_rigid_body_count;
	std::vector<Obj*> m_rigid_body_list;
};