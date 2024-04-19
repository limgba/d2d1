#include "yaml.h"
#include <regex>
#include <fstream>

std::vector<yaml*> yaml::m_yaml_vec;
size_t yaml::m_last_line_tab_count = 0;
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
	WriteFile(&g_yaml, CONFIG_PATH_HEAD + "control_copy.yaml");
	return 0;
}

int yaml::ReadFile(const std::string& str)
{
	static std::regex file_name(".*/(.*)\\.ya*ml");
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
	yaml* current_yaml = new yaml;
	auto file_it = file_mgr->find(str);
	if (file_it != file_mgr->end())
	{
		delete file_it->second;
		file_it->second = current_yaml;
	}
	else
	{
		file_mgr->emplace(str, current_yaml);
	}

	m_yaml_vec.clear();
	m_last_line_tab_count = 0;
	m_yaml_vec.push_back(current_yaml);
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
	if (!(tab_count == m_last_line_tab_count 
		|| tab_count == m_last_line_tab_count + 1
		|| tab_count < m_last_line_tab_count
		))
	{
		return -1000000000 - line;
	}
	yaml::CheckResize(tab_count);
	if (m_yaml_vec.empty())
	{
		return -2000000000;
	}
	yaml* parent_yaml = m_yaml_vec.back();
	if (nullptr == parent_yaml)
	{
		return -2000000001;
	}

	static std::regex _hash("(\\w+): (.*)");
	static std::regex _list("- (.*)");
	if (std::regex_match(value_str, match_result, _hash))
	{
		std::string key_str = match_result[1].str();
		std::string hash_value_str = match_result[2].str();
		if (nullptr == parent_yaml->value())
		{
			parent_yaml->mutate_type(YAML_TYPE_ENUM_HASH);
			parent_yaml->mutate_value(new std::unordered_map<std::string, yaml*>);
		}
		if (!hash_value_str.empty())
		{
			int ret = PushValue(parent_yaml, key_str, hash_value_str);
			if (ret < 0)
			{
				return -2000 + ret;
			}
		}
		else
		{
			yaml* current_yaml = new yaml;
			yaml::PushBack(parent_yaml, key_str, current_yaml);

			m_yaml_vec.push_back(current_yaml);
		}
	}
	else if (std::regex_match(value_str, match_result, _list))
	{
		if (nullptr == parent_yaml->value())
		{
			parent_yaml->mutate_type(YAML_TYPE_ENUM_LIST);
			parent_yaml->mutate_value(new std::vector<yaml*>);
		}

		std::string list_value_str = match_result[1].str();
		if (!list_value_str.empty())
		{
			std::string parent_size_str = std::to_string(parent_yaml->Size());
			int ret = PushValue(parent_yaml, parent_size_str, list_value_str);
			if (ret < 0)
			{
				return -3000 + ret;
			}
		}
		else
		{
			std::string parent_size_str = std::to_string(parent_yaml->Size());
			yaml* current_yaml = new yaml;
			yaml::PushBack(parent_yaml, parent_size_str, current_yaml);

			m_yaml_vec.push_back(current_yaml);
		}
	}
	else
	{
		return -10000;
	}

	m_last_line_tab_count = tab_count;
	return 0;
}

int yaml::PushValue(yaml* parent_yaml, const std::string& key, const std::string& str)
{
	if (nullptr == parent_yaml)
	{
		return -100;
	}
	switch (parent_yaml->type())
	{
	case YAML_TYPE_ENUM_HASH:
	{
		auto* hash = parent_yaml->GetHash();
		if (nullptr == hash)
		{
			return -200;
		}
		int ret = ReadHash(hash, key, str);
		if (ret < 0)
		{
			return -300 + ret;
		}
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		auto* vec = parent_yaml->GetVector();
		if (nullptr == vec)
		{
			return -400;
		}
		int ret = ReadList(vec, str);
		if (ret < 0)
		{
			return -500 + ret;
		}
	}
	break;
	}
	return 0;
}

bool yaml::PushBack(yaml* parent_yaml, const std::string& key, yaml* current_yaml)
{
	if (nullptr == parent_yaml || nullptr == current_yaml)
	{
		return false;
	}

	switch (parent_yaml->type())
	{
	case YAML_TYPE_ENUM_HASH:
	{
		std::unordered_map<std::string, yaml*>* hash = parent_yaml->GetHash();
		if (nullptr == hash)
		{
			return false;
		}
		auto it = hash->find(key);
		if (it != hash->end())
		{
			delete it->second;
			it->second = current_yaml;
		}
		else
		{
			hash->emplace(key, current_yaml);
		}
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		std::vector<yaml*>* vec = parent_yaml->GetVector();
		if (nullptr == vec)
		{
			return false;
		}
		vec->push_back(current_yaml);
	}
	break;
	default:
	{
		return false;
	}
	break;
	}

	return true;
}

int yaml::ReadHash(std::unordered_map<std::string, yaml*>* hash, const std::string& key, const std::string& str)
{
	if (nullptr == hash || key.empty() || str.empty())
	{
		return -10;
	}
	yaml* current_yaml = new yaml;
	if (nullptr == current_yaml)
	{
		return -20;
	}
	int ret = ReadValue(current_yaml, str);
	if (ret != 0)
	{
		delete current_yaml;
		current_yaml = nullptr;
		return -30 + ret;
	}
	hash->emplace(key, current_yaml);
	return 0;
}

int yaml::ReadList(std::vector<yaml*>* vec, const std::string& str)
{
	if (nullptr == vec || str.empty())
	{
		return -40;
	}
	yaml* current_yaml = new yaml;
	if (nullptr == current_yaml)
	{
		return -50;
	}
	int ret = ReadValue(current_yaml, str);
	if (ret != 0)
	{
		delete current_yaml;
		current_yaml = nullptr;
		return -60 + ret;
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
		return -2;
	}
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
	int depth = 0;
	int ret = yaml::WriteFileStr(ofs, _yaml, depth);
	ofs.close();
	return ret;
}

int yaml::WriteFileStr(std::ofstream& ofs, yaml* _yaml, int& depth)
{
	if (nullptr == _yaml)
	{
		return -1;
	}
	std::string tab;
	for (int i = 0; i < depth; ++i)
	{
		tab += "\t";
	}

	static std::string next_line("\n");
	switch (_yaml->type())
	{
	case YAML_TYPE_ENUM_HASH:
	{
		std::unordered_map<std::string, yaml*>* hash = _yaml->GetHash();
		if (nullptr == hash)
		{
			break;
		}

		for (const auto& pair : *hash)
		{
			const std::string& key = pair.first;
			yaml* sub_yaml = pair.second;
			if (nullptr == sub_yaml)
			{
				continue;
			}

			int type = 0;
			switch (sub_yaml->type())
			{
			case YAML_TYPE_ENUM_HASH: { type = WRITE_DATA_TYPE_CONTAINER; } break;
			case YAML_TYPE_ENUM_LIST: { type = WRITE_DATA_TYPE_CONTAINER; } break;
			case YAML_TYPE_ENUM_INTEGER: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_LONG_LONG: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_DOUBLE: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_STRING: { type = WRITE_DATA_TYPE_VALUE; } break;
			}
			switch (type)
			{
			case WRITE_DATA_TYPE_CONTAINER:
			{
				std::string write_str = tab + key + ": " + next_line;
				ofs.write(write_str.c_str(), write_str.size());
				WriteFileStr(ofs, sub_yaml, ++depth);
			}
			break;
			case WRITE_DATA_TYPE_VALUE:
			{
				std::string write_str = tab + key + ": ";
				ofs.write(write_str.c_str(), write_str.size());
				int temp_depth = 0;
				WriteFileStr(ofs, sub_yaml, temp_depth);
				ofs.write(next_line.c_str(), next_line.size());
			}
			break;
			}
		}
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		std::vector<yaml*>* vec = _yaml->GetVector();
		if (nullptr == vec)
		{
			break;
		}
		for (size_t i = 0; i < vec->size(); ++i)
		{
			std::string key = std::to_string(i);
			yaml* sub_yaml = (*vec)[i];
			if (nullptr == sub_yaml)
			{
				continue;
			}
			int type = 0;
			switch (sub_yaml->type())
			{
			case YAML_TYPE_ENUM_HASH: { type = WRITE_DATA_TYPE_CONTAINER; } break;
			case YAML_TYPE_ENUM_LIST: { type = WRITE_DATA_TYPE_CONTAINER; } break;
			case YAML_TYPE_ENUM_INTEGER: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_LONG_LONG: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_DOUBLE: { type = WRITE_DATA_TYPE_VALUE; } break;
			case YAML_TYPE_ENUM_STRING: { type = WRITE_DATA_TYPE_VALUE; } break;
			}
			switch (type)
			{
			case WRITE_DATA_TYPE_CONTAINER:
			{
				std::string write_str = tab + "- " + next_line;
				ofs.write(write_str.c_str(), write_str.size());
				WriteFileStr(ofs, sub_yaml, ++depth);
			}
			break;
			case WRITE_DATA_TYPE_VALUE:
			{
				std::string write_str = tab + "- ";
				ofs.write(write_str.c_str(), write_str.size());
				int temp_depth = 0;
				WriteFileStr(ofs, sub_yaml, temp_depth);
				ofs.write(next_line.c_str(), next_line.size());
			}
			break;
			}
		}
	}
	break;
	case YAML_TYPE_ENUM_INTEGER:
	{
		int* value_ptr = _yaml->GetInt();
		if (nullptr == value_ptr)
		{
			break;
		}
		std::string write_str = std::to_string(*value_ptr);
		ofs.write(write_str.c_str(), write_str.size());
	}
	break;
	case YAML_TYPE_ENUM_LONG_LONG:
	{
		long long* value_ptr = _yaml->GetLongLong();
		if (nullptr == value_ptr)
		{
			break;
		}
		std::string write_str = std::to_string(*value_ptr);
		ofs.write(write_str.c_str(), write_str.size());
	}
	break;
	case YAML_TYPE_ENUM_DOUBLE:
	{
		double* value_ptr = _yaml->GetDouble();
		if (nullptr == value_ptr)
		{
			break;
		}
		std::string write_str = std::to_string(*value_ptr);
		ofs.write(write_str.c_str(), write_str.size());
	}
	break;
	case YAML_TYPE_ENUM_STRING:
	{
		std::string* value_ptr = _yaml->GetStr();
		if (nullptr == value_ptr)
		{
			break;
		}
		std::string write_str = *value_ptr;
		ofs.write(write_str.c_str(), write_str.size());
	}
	break;
	}

	--depth;
	return 0;
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
std::string* yaml::GetStr()
{
	if (YAML_TYPE_ENUM_STRING != m_type)
	{
		return nullptr;
	}
	return m_value._string;
}

size_t yaml::Size()
{
	switch (m_type)
	{
	case YAML_TYPE_ENUM_HASH:
	{
		auto* container = this->GetHash();
		if (nullptr == container)
		{
			break;
		}
		return container->size();
	}
	break;
	case YAML_TYPE_ENUM_LIST:
	{
		auto* container = this->GetVector();
		if (nullptr == container)
		{
			break;
		}
		return container->size();
	}
	break;
	}
	return 0;
}

yaml* yaml::at(const std::string& key)
{
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
		return _yaml->at(other_key);
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
		return _yaml->at(other_key);
	}
	break;
	}
	return nullptr;
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

void yaml::CheckResize(size_t tab_count)
{
	if (tab_count > m_last_line_tab_count)
	{
		return;
	}
	const size_t cur_size = tab_count + 1;
	if (m_yaml_vec.size() > cur_size)
	{
		m_yaml_vec.resize(cur_size);
	}
}

