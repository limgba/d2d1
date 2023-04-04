#pragma once
#include <vector>
class Obj;

class ControlObjMgr
{
public:
	static ControlObjMgr& Instance();

	void PushControlObj(Obj* obj);
	void RemoveControlObj(Obj* obj);
	void SetObjCoordinate(size_t index, int x, int y);
	void ChangeObjCoordinate(size_t index, int x, int y);
	
private:
	std::vector<Obj*> m_control_obj_list;
};
