#include "yaml.h"
#include <regex>
#include <fstream>

std::vector<yaml*> yaml::m_yaml_vec;
std::vector<std::string> yaml::m_key_vec;
yaml g_yaml;
yaml::yaml()
	: m_type(0)
	, m_value(nullptr)
{
}

yaml::~yaml()
{
	if (nullptr == m_value._void)
	{
		return;
	}
	switch (m_type)
	{
	case YAML_TYPE_ENUM_HASH:
	{
		for (auto& pair : *m_value._hash)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		for (size_t i = 0; i < m_value._vector->size(); ++i)
		{
			delete (*m_value._vector)[i];
			(*m_value._vector)[i] = nullptr;
		}
	}
	break;
	case YAML_TYPE_ENUM_INTEGER:
	{
		delete m_value._int;
		m_value._int = nullptr;
	}
	break;
	case YAML_TYPE_ENUM_LONG_LONG:
	{
		delete m_value._longlong;
		m_value._longlong = nullptr;
	}
	break;
	case YAML_TYPE_ENUM_DOUBLE:
	{
		delete m_value._double;
		m_value._double = nullptr;
	}
	break;
	case YAML_TYPE_ENUM_STRING:
	{
		delete m_value._string;
		m_value._string = nullptr;
	}
	break;
	}

}

int yaml::ReadFileList()
{
	static const std::string CONFIG_PATH_HEAD = "../wndtest/d2d1/config/";
	std::string path;
	int ret = 0;

	path = CONFIG_PATH_HEAD + "control.yaml";
	ret = ReadFile(path);
	if (ret)
	{
		return ret;
	}
	return 0;
}

int yaml::ReadFile(const std::string& str)
{
	static std::regex file_name("/(.*)\\.ya*ml");
	std::smatch match_result;
	if (!std::regex_search(str, match_result, file_name))
	{
		return -1;
	}
	std::ifstream ifs(str);
	if (!ifs.is_open())
	{
		return -2;
	}
		
	std::string stem = match_result[1].str();
	int ret = ReadFileName(stem);
	if (ret < 0)
	{
		ifs.close();
		return -10 + ret;
	}
	std::string line_str;
	int line = 0;
	while (std::getline(ifs, line_str))
	{
		ret = ReadLine(++line, line_str);
		if (ret < 0)
		{
			ifs.close();
			return line;
		}
	}
	ifs.close();
	return 0;
}

int yaml::WriteFile(yaml* _yaml, const std::string& path)
{
	if (nullptr == _yaml || nullptr == _yaml->value())
	{
		return -1;
	}
	std::ofstream ofs(path);
	if (!ofs.is_open())
	{
		return -2;
	}

	return 0;
}

int yaml::ReadFileName(const std::string& str)
{
	if (nullptr == g_yaml.value())
	{
		g_yaml.mutate_type(YAML_TYPE_ENUM_HASH);
		g_yaml.mutate_value(new std::unordered_map<std::string, yaml*>);
	}
	std::unordered_map<std::string, yaml*>* file_mgr = g_yaml.GetHash();
	if (nullptr == file_mgr)
	{
		return -1;
	}
	auto file_it = file_mgr->find(str);
	yaml* file = new yaml;
	file->mutate_type(YAML_TYPE_ENUM_HASH);
	file->mutate_value(new std::unordered_map<std::string, yaml*>);
	if (file_it != file_mgr->end())
	{
		delete file_it->second;

		file_it->second = file;
	}
	else
	{
		file_mgr->emplace(str, file);
	}

	m_yaml_vec.clear();
	m_yaml_vec.push_back(&g_yaml);
	m_yaml_vec.push_back(file);
	m_key_vec.clear();
	return 0;
}
int yaml::ReadLine(int line, const std::string& str)
{
	if (str.empty())
	{
		return 0;
	}
	std::smatch match_result;
	static std::regex _table("\t*(.*)");
	std::string value_str;
	if (std::regex_match(str, match_result, _table))
	{
		value_str = match_result[1].str();
	}
	if (value_str.empty())
	{
		return 0;
	}
	const size_t tab_count = yaml::GetTabCount(str);
	const size_t cur_size = tab_count + 2;
	if (cur_size < m_yaml_vec.size())
	{
		m_yaml_vec.resize(cur_size);
	}
	if (tab_count < m_key_vec.size())
	{
		m_key_vec.resize(tab_count);
	}
	yaml* parent_yaml = m_yaml_vec.back();
	if (nullptr == parent_yaml)
	{
		return -1;
	}
	static std::regex _hash("(\\w+): (.*)");
	static std::regex _list("- (.*)");
	yaml* current_yaml = nullptr;

	size_t parent_container_size = 0;
	if (nullptr != parent_yaml->GetHash())
	{
		parent_container_size = parent_yaml->GetHash()->size();
	}
	else if (nullptr != parent_yaml->GetVector())
	{
		parent_container_size = parent_yaml->GetVector()->size();
	}

	if (std::regex_match(value_str, match_result, _hash))
	{
		std::string key_str = match_result[1].str();
		std::string hash_value_str = match_result[2].str();
		if (!hash_value_str.empty())
		{
			ReadHash(parent_yaml->GetHash(), key_str, hash_value_str);
		}
		else
		{
			current_yaml = new yaml;
			current_yaml->mutate_type(YAML_TYPE_ENUM_HASH);
			current_yaml->mutate_value(new std::unordered_map<std::string, yaml*>);
			yaml::PushBack(parent_yaml, key_str, current_yaml);

			m_yaml_vec.push_back(current_yaml);
			m_key_vec.push_back(key_str);
		}
	}
	else if (std::regex_match(value_str, match_result, _list))
	{

		std::string list_value_str = match_result[1].str();
		if (!list_value_str.empty())
		{
			ReadList(current_yaml->GetVector(), list_value_str);
		}
		else
		{
			current_yaml = new yaml;
			current_yaml->mutate_type(YAML_TYPE_ENUM_LIST);
			current_yaml->mutate_value(new std::vector<yaml*>);
			std::string key_str = std::to_string(parent_container_size);
			yaml::PushBack(parent_yaml, key_str, current_yaml);

			m_yaml_vec.push_back(current_yaml);
			m_key_vec.push_back(key_str);
		}
	}
	else
	{
		return -100;
	}

	return 0;
}

int yaml::ReadHash(std::unordered_map<std::string, yaml*>* hash, const std::string& key, const std::string& str)
{
	if (nullptr == hash || key.empty() || str.empty())
	{
		return -1;
	}
	yaml* current_yaml = new yaml;
	if (nullptr == current_yaml)
	{
		return -2;
	}
	int ret = ReadValue(current_yaml, str);
	if (ret != 0)
	{
		delete current_yaml;
		current_yaml = nullptr;
		return ret;
	}
	hash->emplace(str, current_yaml);
	return 0;
}

int yaml::ReadList(std::vector<yaml*>* vec, const std::string& str)
{
	if (nullptr == vec || str.empty())
	{
		return -1;
	}
	yaml* current_yaml = new yaml;
	if (nullptr == current_yaml)
	{
		return -2;
	}
	int ret = ReadValue(current_yaml, str);
	if (ret != 0)
	{
		delete current_yaml;
		current_yaml = nullptr;
		return ret;
	}
	vec->push_back(current_yaml);
	return 0;
}

int yaml::ReadValue(yaml* current_yaml, const std::string& str)
{
	if (str.empty())
	{
		return -1;
	}
	std::smatch match_result;
	static std::regex _integer("-{0,1}\\d{1,9}");
	static std::regex _longlong("-{0,1}\\d{1,19}");
	static std::regex _double("-{0,1}\\d+\\.\\d+");
	static std::regex _string("\\w+");
	if (std::regex_match(str, match_result, _integer))
	{
		current_yaml->mutate_type(YAML_TYPE_ENUM_INTEGER);
		current_yaml->mutate_value(new int);

		int _value = std::stoi(str);
		memcpy(current_yaml->value(), &_value, sizeof(_value));
	}
	else if (std::regex_match(str, match_result, _longlong))
	{
		current_yaml->mutate_type(YAML_TYPE_ENUM_LONG_LONG);
		current_yaml->mutate_value(new long long);

		long long _value = std::stoll(str);
		memcpy(current_yaml->value(), &_value, sizeof(_value));
	}
	else if (std::regex_match(str, match_result, _double))
	{
		current_yaml->mutate_type(YAML_TYPE_ENUM_DOUBLE);
		current_yaml->mutate_value(new double);

		double _value = std::stod(str);
		memcpy(current_yaml->value(), &_value, sizeof(_value));
	}
	else if (std::regex_match(str, match_result, _string))
	{
		current_yaml->mutate_type(YAML_TYPE_ENUM_STRING);
		std::string* _value = new std::string(str);
		current_yaml->mutate_value(_value);
	}
	else
	{
		return -100;
	}
	return 0;
}

bool yaml::PushBack(yaml* parent_yaml, const std::string& key, yaml* current_yaml)
{
	if (nullptr == parent_yaml || nullptr == current_yaml)
	{
		return false;
	}
	if (nullptr != parent_yaml->GetHash())
	{
		parent_yaml->GetHash()->emplace(key, current_yaml);
		return true;
	}
	else if (nullptr != parent_yaml->GetVector())
	{
		parent_yaml->GetVector()->push_back(current_yaml);
		return true;
	}
	return false;
}

std::vector<yaml*>* yaml::GetVector()
{
	if (YAML_TYPE_ENUM_LIST != m_type)
	{
		return nullptr;
	}
	return m_value._vector;
}

std::unordered_map<std::string, yaml*>* yaml::GetHash()
{
	if (YAML_TYPE_ENUM_HASH != m_type)
	{
		return nullptr;
	}
	return m_value._hash;
}

int* yaml::GetInt()
{
	if (YAML_TYPE_ENUM_INTEGER != m_type)
	{
		return nullptr;
	}
	return m_value._int;
}
long long* yaml::GetLongLong()
{
	if (YAML_TYPE_ENUM_LONG_LONG != m_type)
	{
		return nullptr;
	}
	return m_value._longlong;
}
double* yaml::GetDouble()
{
	if (YAML_TYPE_ENUM_DOUBLE != m_type)
	{
		return nullptr;
	}
	return m_value._double;
}
yaml* yaml::operator[](const std::string& key)
{
	if (key.empty())
	{
		return nullptr;
	}
	std::size_t pos = key.find_first_of('/');
	std::string one_key;
	std::string other_key;
	if (std::string::npos != pos)
	{
		one_key = key.substr(0, pos);
		if (pos + 1 < key.size())
		{
			other_key = key.substr(pos + 1);
		}
	}
	else
	{
		one_key = key;
	}
	switch (m_type)
	{
	case YAML_TYPE_ENUM_HASH:
	{
		auto* hash = this->GetHash();
		if (nullptr == hash)
		{
			return nullptr;
		}
		auto it = hash->find(one_key);
		if (hash->end() == it)
		{
			return nullptr;
		}
		yaml* _yaml = it->second;
		if (nullptr == _yaml)
		{
			return nullptr;
		}
		if (other_key.empty())
		{
			return _yaml;
		}
		return (*_yaml)[other_key];
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		auto* vec = this->GetVector();
		if (nullptr == vec)
		{
			return nullptr;
		}
		std::smatch match_result;
		static std::regex _integer("-{0,1}\\d{1,9}");
		if (!std::regex_match(one_key, match_result, _integer))
		{
			return nullptr;
		}
		int index = std::stoi(one_key);
		if (index < 0 || index >= vec->size())
		{
			return nullptr;
		}
		yaml* _yaml = (*vec)[index];
		if (nullptr == _yaml)
		{
			return nullptr;
		}
		if (other_key.empty())
		{
			return _yaml;
		}
		return (*_yaml)[other_key];
	}
	break;
	}
	return nullptr;
}
std::string* yaml::GetStr()
{
	if (YAML_TYPE_ENUM_STRING != m_type)
	{
		return nullptr;
	}
	return m_value._string;
}

size_t yaml::GetTabCount(const std::string& str)
{
	size_t count = 0;
	size_t pos = 0;
	while (pos < str.size())
	{
		pos = str.find("\t", pos);
		if (pos == std::string::npos)
		{
			break;
		}
		++pos;
		++count;
	}
	return count;
}

