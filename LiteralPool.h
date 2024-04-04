#pragma once
#include <map>
#include <set>
#include <string>

namespace FloatPool
{
	//��¼��������ʹ�õ�label������������ͳ���label�ظ�
	extern std::set<std::string> label_used;

	extern std::map<float, std::string> pool;

	extern int pool_cnt;

	void Insert(float value);

	std::string GetLabel(float value);

	void GenerateAllFloatLiterals();
}
