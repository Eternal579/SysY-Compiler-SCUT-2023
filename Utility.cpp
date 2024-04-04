#include "Utility.h"

#include <random>
#include "AST.h"
#include <cstring>
#include <string>
#include "Constants.h"
ofstream output;
default_random_engine e(0);
string ToString(ASTNodeType type)
{
	switch (type)
	{
	case ASTNodeType::None: return "None";
	case ASTNodeType::ASSIGN: return "Assign";
	case ASTNodeType::BLOCK: return "Blobkc";
	case ASTNodeType::IF: return "If";
	case ASTNodeType::WHILE: return "While";
	case ASTNodeType::CONTINUE: return "Continue";
	case ASTNodeType::BREAK: return "Break";
	case ASTNodeType::RETURN: return "Return";
	case ASTNodeType::FUNCCALL: return "Funcall";
	case ASTNodeType::VARDEF: return "Vardef";
	case ASTNodeType::FUNCDEF: return "Funcdef";
	default: return "Unknown Type";
	}
}

void InitOutputStream(const char* path)
{
	output.open(path);
	if (!output.is_open())
		error_sys("In InitOutputStream: output stream open failed");
}

void CloseOutputStream()
{
	output.close();
}

string GetLabel(int cnt)
{
	return ".mylabel" + to_string(cnt);
}

int GetRandomInt(int l, int r)
{
	/*auto t = time(0);
	cout << "time is :" << t << endl;*/
	//static default_random_engine e(0);
	uniform_int_distribution<int> u(l, r);
	//int ran = u(e);//为什么会获取到ran=r的情况？
	//while (ran > r)
	//	ran -= (r - l + 1);
	return u(e);
}

string RandomFromSet(const set<RegisterInfo>& s)
{
	if (s.empty())
		error_sys("In RandomFromSet: set is empty");
	vector<string>names;
	vector<int>freqs;
	int max_freq = 1;
	for(auto i = s.begin(); i != s.end(); ++i)
	{
		names.push_back((*i).name);
		freqs.push_back((*i).freq);
		max_freq = max(max_freq, freqs.back());
	}
	for (auto& i : freqs)
		i = max_freq - i + 1;

	discrete_distribution<> d(freqs.begin(), freqs.end());
	return names[d(e)];
}

string TypeConvert(ValueType src_type, string src_reg, ValueType res_type)
{
	string target_reg;
	if (res_type == ValueType::INT && src_type == ValueType::FLOAT)
	{
		target_reg = CheckAndSetRegister_INT(target_reg);
		output << "\tfcvt.w.s " << target_reg << ", " << src_reg << ", rtz" << endl;
		RestoreRegister(src_reg);
	}
	else if (res_type == ValueType::FLOAT && src_type == ValueType::INT)
	{
		target_reg = CheckAndSetRegister_FLOAT(target_reg);
		output << "\tfcvt.s.w " << target_reg << ", " << src_reg << endl;
		RestoreRegister(src_reg);
	}
	else
		return src_reg;
	return target_reg;
}

string DecimalFloatToHex(float num) {
	unsigned int* hvalue;
	hvalue = (unsigned int*)&num;
	char ptr[32] = {};
	snprintf(ptr, 32, "%X", *hvalue);
	return ptr;
}

void PrintStoreVariableToMemory(const string& name, Info& info)
{
	if ((info.address_sp == -1 && !info.is_global) || info.reg.empty())
		return;
	if (!info.dimensions.empty() && info.dimensions[0] == -1)
	{
		PrintCommandRelativeToSP("sd", info.reg, info.address_sp);
	}
	else if (info.is_global)
	{
		if (info.type == ValueType::FLOAT)
		{
			output << "\tlla s0, " << name << "\n";
			output << "\tfsw " << info.reg << ", 0(s0)\n";
		}
		else
		{
			output << "\tlla s0, " << name << "\n";
			output << "\tsw " << info.reg << ", 0(s0)\n";
		}
	}
	else
	{
		if (info.type == ValueType::FLOAT)
		{
			PrintCommandRelativeToSP("fsw", info.reg, info.address_sp);
		}
		else
		{
			PrintCommandRelativeToSP("sw", info.reg, info.address_sp);
		}
	}
	info.reg = "";
}

void PrintCommandRelativeToSP(string command, string reg, int offset)
{
	if (offset > 2047 || offset < -2048)
	{
		if (reg == "s0")
			error_sys("In PrintCommandRelativeToSP: the target_reg is equal to s0");
		output << "\tli s0, " << offset << endl;
		output << "\tadd s0, s0, sp\n";
		output << '\t' << command << " " << reg << ", 0(s0)\n";
	}
	else
	{
		output << '\t' << command << " " << reg << ", " << offset << "(sp)\n";
	}
}
