#pragma once
#include <vector>
#include <unordered_map>
class Obj;

class ObjMgr
{
public:
	static ObjMgr& Instance();
	void DeleteAllObj();
	void AddObj(Obj* obj);
	void DeleteObj(int obj_id);
	Obj* GetObj(int obj_id);

private:
	void RemoveLink(Obj* obj);
	
private:
	std::unordered_map<int, Obj*> m_obj_unordered_map;
};
