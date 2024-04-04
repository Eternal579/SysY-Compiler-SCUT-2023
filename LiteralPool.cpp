#include "LiteralPool.h"

#include "Utility.h"

std::set<std::string> FloatPool::label_used;

std::map<float, std::string> FloatPool::pool;

int FloatPool::pool_cnt = 0;

void FloatPool::Insert(float value)
{
	if (pool.find(value) != pool.end())
		return;

	std::string label = ".float" + std::to_string(++pool_cnt);
	while (label_used.find(label) != label_used.end())
		label = ".float" + std::to_string(++pool_cnt);
	pool.insert({ value, move(label) });
}

std::string FloatPool::GetLabel(float value)
{
	auto i = pool.find(value);
	if (i == pool.end())
		error_sys("In FloatPool::GetLabel: float literal not found.");
	return i->second;
}

void FloatPool::GenerateAllFloatLiterals()
{
	Insert(0.0f);

	output << "\t.data" << endl;
	for(auto i = pool.begin(); i != pool.end(); ++i)
		output << '\t' << i->second << ": .word 0x" << DecimalFloatToHex(i->first) << endl;
}