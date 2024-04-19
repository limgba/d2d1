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

enum WRITE_DATA_TYPE
{
	WRITE_DATA_TYPE_CONTAINER = 1,
	WRITE_DATA_TYPE_VALUE = 2,
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
	static int ReadFileName(const std::string& str);
	static int ReadLine(int line, const std::string& str);
	static int PushValue(yaml* parent_yaml, const std::string& key, const std::string& str);
	static bool PushBack(yaml* parent_yaml, const std::string& key, yaml* current_yaml);
	static int ReadHash(std::unordered_map<std::string, yaml*>* hash, const std::string& key, const std::string& str);
	static int ReadList(std::vector<yaml*>* vec, const std::string& str);
	static int ReadValue(yaml* current_yaml, const std::string& str);
	static int WriteFile(yaml* _yaml, const std::string& path);
	static int WriteFileStr(std::ofstream& ofs, yaml* _yaml, int& depth);

	std::vector<yaml*>* GetVector();
	std::unordered_map<std::string, yaml*>* GetHash();
	int* GetInt();
	long long* GetLongLong();
	double* GetDouble();
	std::string* GetStr();
	size_t Size();
	yaml* at(const std::string& key);

	int type() const { return m_type; }
	void mutate_type(int type) { m_type = type; }

	void* value() { return m_value._void; }
	void mutate_value(void* value) { m_value._void = value; }

protected:
	static size_t GetTabCount(const std::string& str);
	static void CheckResize(size_t tab_count);


private:
	static std::vector<yaml*> m_yaml_vec;
	static size_t m_last_line_tab_count;
	int m_type;
	yaml_value m_value;
};
extern yaml g_yaml;

#pragma pack(pop)
