#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum YAML_TYPE_ENUM
{
	YAML_TYPE_ENUM_INVALID = 0,
	YAML_TYPE_ENUM_HASH,
	YAML_TYPE_ENUM_LIST,
	YAML_TYPE_ENUM_INTEGER,
	YAML_TYPE_ENUM_LONG_LONG,
	YAML_TYPE_ENUM_DOUBLE,
	YAML_TYPE_ENUM_STRING,
};

class yaml;
union yaml_value
{
	yaml_value(void* __void)
		: _void(__void)
	{}
	void* _void;
	std::unordered_map<std::string, yaml*>* _hash;
	std::vector<yaml*>* _vector;
	int* _int;
	long long* _longlong;
	double* _double;
	std::string* _string;
};

#pragma pack(push)
#pragma pack(4)

class yaml
{
public:
	yaml();
	~yaml();
	explicit yaml(const yaml& rhs) = delete;
	yaml& operator=(const yaml& rhs) = delete;

	static int ReadFileList();
	static int ReadFile(const std::string& str);
	static int WriteFile(yaml* _yaml, const std::string& path);
	static int WriteFileStr(yaml* _yaml, int& depth);
	static int ReadFileName(const std::string& str);
	static int ReadLine(int line, const std::string& str);
	static int ReadHash(std::unordered_map<std::string, yaml*>* hash, const std::string& key, const std::string& str);
	static int ReadList(std::vector<yaml*>* vec, const std::string& str);
	static int ReadValue(yaml* current_yaml, const std::string& str);
	static bool PushBack(yaml* parent_yaml, const std::string& key, yaml* current_yaml);
	yaml* operator[](const std::string& key);

	std::vector<yaml*>* GetVector();
	std::unordered_map<std::string, yaml*>* GetHash();
	int* GetInt();
	long long* GetLongLong();
	double* GetDouble();
	std::string* GetStr();

	int type() { return m_type; }
	void mutate_type(int type) { m_type = type; }

	void* value() { return m_value._void; }
	void mutate_value(void* value) { m_value._void = value; }

protected:
	static size_t GetTabCount(const std::string& str);


private:
	static std::vector<yaml*> m_yaml_vec;
	static std::vector<std::string> m_key_vec;
	int m_type;
	yaml_value m_value;
};
extern yaml g_yaml;

#pragma pack(pop)
