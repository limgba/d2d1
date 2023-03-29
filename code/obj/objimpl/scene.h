#pragma once
#include "obj/obj.h"

class Scene : public Obj
{
public:
	virtual int GetObjType() { return BITMAP_IMAGE_OBJ_TYPE_SCENE; }
	void SetOffsetX(int x) { m_offset_x = x; }
	void SetOffsetY(int y) { m_offset_y = y; }
	void SetMoveX(int x) { m_move_x = x; }
	void SetMoveY(int y) { m_move_y = y; }
	void SetMoveFrame(int frame) { m_move_frame = frame; }
	
	int m_offset_x = 0;
	int m_offset_y = 0;
	int m_move_x = 0;
	int m_move_y = 0;
	int m_move_frame = 0;
};
