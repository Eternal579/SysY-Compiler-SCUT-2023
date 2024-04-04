#pragma once
#include <map>
#include <set>
#include <string>

namespace FloatPool
{
	//记录被变量名使用的label，避免变量名和常量label重复
	extern std::set<std::string> label_used;

	extern std::map<float, std::string> pool;

	extern int pool_cnt;

	void Insert(float value);

	std::string GetLabel(float value);

	void GenerateAllFloatLiterals();
}
