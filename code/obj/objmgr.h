#pragma once
#include <vector>
#include <unordered_map>
#include <string>
class Obj;

class ObjMgr
{
public:
	static ObjMgr& Instance();
	void DeleteAllObj();
	void AddObj(Obj* obj);
	void DeleteObj(int obj_id);
	Obj* GetObj(int obj_id);
	Obj* GetObjByType(int obj_type);
	Obj* GetObjByPath(const std::wstring& path);

private:
	void RemoveLink(Obj* obj);
	
private:
	std::unordered_map<int, Obj*> m_obj_unordered_map;
};
