#pragma once
#include "obj/obj.h"

class Skill : public Obj
{
public:
	virtual int GetObjType() { return BITMAP_IMAGE_OBJ_TYPE_SKILL; }
	//virtual void OnImpact(Obj* obj);
	
};
