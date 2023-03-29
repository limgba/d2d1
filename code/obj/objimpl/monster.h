#pragma once
#include "obj/obj.h"

class Monster : public Obj
{
public:
	virtual int GetObjType() { return BITMAP_IMAGE_OBJ_TYPE_MONSTER; }
	//virtual void OnImpact(Obj* obj);
	
};
