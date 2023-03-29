#pragma once
#include <string>
class Animation;

enum BITMAP_IMAGE_OBJ_TYPE
{
	BITMAP_IMAGE_OBJ_TYPE_NONE = 0,
	BITMAP_IMAGE_OBJ_TYPE_SCENE,
	BITMAP_IMAGE_OBJ_TYPE_PLAYER,
	BITMAP_IMAGE_OBJ_TYPE_MONSTER,
	BITMAP_IMAGE_OBJ_TYPE_SKILL = 100,
	BITMAP_IMAGE_OBJ_TYPE_DROP,
};

constexpr int LeftOffsetShort(int a, int b)
{
	constexpr int offset = static_cast<int>(sizeof(short) * CHAR_BIT);
	return (a << offset) + b;
}

static const int BITMAP_IMAGE_OBJ_TYPE_PLAYER_SKILL = LeftOffsetShort(BITMAP_IMAGE_OBJ_TYPE_PLAYER, BITMAP_IMAGE_OBJ_TYPE_SKILL);
static const int BITMAP_IMAGE_OBJ_TYPE_PLAYER_MONSTER = LeftOffsetShort(BITMAP_IMAGE_OBJ_TYPE_PLAYER,  BITMAP_IMAGE_OBJ_TYPE_MONSTER);
static const int BITMAP_IMAGE_OBJ_TYPE_PLAYER_DROP = LeftOffsetShort(BITMAP_IMAGE_OBJ_TYPE_PLAYER, BITMAP_IMAGE_OBJ_TYPE_DROP);
static const int BITMAP_IMAGE_OBJ_TYPE_MONSTER_SKILL = LeftOffsetShort(BITMAP_IMAGE_OBJ_TYPE_MONSTER, BITMAP_IMAGE_OBJ_TYPE_SKILL);


class Obj
{
public:
	Obj();
	virtual ~Obj();

	virtual int GetObjType() = 0;
	virtual void OnImpact(Obj* obj);
	void SetAnimation(Animation* animation);
	Animation* GetAnimation();
	void SetCoordinate(int x, int y);
	int x();
	int y();
	int GetObjId() const;
	void SetIsRigidbody(bool is_rigid_body);
	bool IsRigidBody() const;

private:
	static int m_auto_inc_id;
	Animation* m_animation;
	int m_obj_id;
	int m_x;
	int m_y;
	bool m_is_rigid_body;
};
