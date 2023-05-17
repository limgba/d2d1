#include "obj.h"
#include "animation/animation.h"

int Obj::m_auto_inc_id = 0;

Obj::Obj() : m_animation(nullptr)
{
	m_obj_id = m_auto_inc_id;

	++m_auto_inc_id;
	if (m_auto_inc_id < 0)
	{
		m_auto_inc_id = 0;
	}
	m_x = 0;
	m_y = 0;
	m_is_rigid_body = false;
	m_control_index = 0;
}

Obj::~Obj()
{
}

void Obj::OnImpact(Obj* obj)
{
}

void Obj::SetAnimation(Animation* animation)
{
	if (nullptr == animation || nullptr != m_animation)
	{
		return;
	}
	m_animation = animation;
}

Animation* Obj::GetAnimation()
{
	return m_animation;
}

void Obj::SetCoordinate(int x, int y)
{
	m_x = x;
	m_y = y;
}

int Obj::x()
{
	return m_x;
}

int Obj::y()
{
	return m_y;
}

int Obj::GetObjId() const
{
	return m_obj_id;
}

void Obj::SetIsRigidbody(bool is_rigid_body)
{
	m_is_rigid_body = is_rigid_body;
}

bool Obj::IsRigidBody() const
{
	return m_is_rigid_body;
}

void Obj::SetControlIndex(size_t index)
{
	m_control_index = index;
}

size_t Obj::GetControlIndex()
{
	return m_control_index;
}
