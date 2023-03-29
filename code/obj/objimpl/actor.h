#pragma once
#include "obj/obj.h"

class Actor : public Obj
{
public:
	virtual int GetObjType() { return BITMAP_IMAGE_OBJ_TYPE_PLAYER; }
	virtual void OnImpact(Obj* obj);
	
};
