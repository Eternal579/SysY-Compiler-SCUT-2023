#pragma warning(disable:4996)
#include <cassert>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <memory>
#include <random>
#include <string>
#include "BasicBlocks.h"
#include "AST.h"
#include "Symbol_Table.h" 
#include "Utility.h"
using namespace std;
#pragma warning(disable:4996)

bool O1_optimize = false;
//#define DEBUG


void calc_use()
{
	for (auto& func : in_func_var_use) {
		/*cout << "\n------------------------------------\n";
		cout << "In the func name: " << func.first << endl;*/
		vector<shared_ptr<VarDef>>tmp;
		for (auto var : func.second) {
			tmp.push_back(var);
		}
		sort(tmp.begin(), tmp.end(), [](const shared_ptr<VarDef>& a, const shared_ptr<VarDef>& b)
		{
			if(a->use_cnt == b->use_cnt)
			{
				if(a->is_used_in_cond == b->is_used_in_cond)
				{
					return a->GetName() > b->GetName();
				}
				else
					return a->is_used_in_cond > b->is_used_in_cond;
			}
			else
				return a->use_cnt > b->use_cnt;
		});
		for(size_t i = 0;i < tmp.size();i++)
			tmp[i]->rank = i + 1;
		/*for (auto& var : func.second) {
			var->rank = lower_bound(tmp.begin(), tmp.end(), var->use_cnt, greater<int>()) - tmp.begin() + 1;
		}*/
		/*for (auto& var : func.second) {
			tmp.push_back(var->use_cnt);
			cout << "VarName: " << var->GetName() << " UseCnt: " << var->use_cnt << " UsedRank: " << var->rank << " IsUsedInCond: " << var->is_used_in_cond << endl;
		}*/
	}
}

#ifndef __linux__
#include "sysy.tab.h"
#else
#include "y.tab.h"
extern FILE* yyin;
extern int yyparse(shared_ptr<CompUnitAST>& ast);

#endif

#include "AST.h"
int main(int argc, const char* argv[]) {
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	string input_path, output_path;

	if (argc >= 6 || argc == 3)
		O1_optimize = true;

	if(argc >= 5)
	{
		input_path = string(argv[4]);
		output_path = string(argv[3]);
	}
	else
	{  
		input_path = string(argv[1]);
		output_path = "./output.s";
	}

	// 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件

	freopen(input_path.c_str(), "r", stdin);
	
	// 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
	shared_ptr<CompUnitAST> ast;
	auto ret = yyparse(ast);
	assert(!ret);

	fstream input_file(input_path);
	size_t file_size = input_file.seekg(0, ios::end).tellg();
	file_size/= 1024; // 获得文件大小(单位: KB)
	input_file.close();

#ifdef DEBUG
	InitOutputStream(output_path.c_str());
	string file_name(input_path);
	const int backslash_index = max(int(file_name.find_last_of('\\')), int(file_name.find_last_of('/')));
	file_name = file_name.substr(backslash_index + 1);
	output << "\t.file \"" << file_name << "\"\n";

	//O1_optimize = true;
	cout << "\nAnalyze -----------------------------\n";
	ast->Analyze();
	ast->Delete_Useless();
	ast->Unroll_Loop();
	ast->Dump();
	for (auto& func : in_func_var_use) {
		cout << "\n------------------------------------\n";
		cout << "In the func name: " << func.first << endl;
		vector<int>tmp;
		for (auto& var : func.second) {
			tmp.push_back(var->use_cnt);
		}
		sort(tmp.begin(), tmp.end(), greater<int>());
		for (auto& var : func.second) {
			var->rank = lower_bound(tmp.begin(), tmp.end(), var->use_cnt, greater<int>()) - tmp.begin() + 1;
		}
		for (auto& var : func.second) {
			tmp.push_back(var->use_cnt);
			cout << "VarName: " << var->GetName() << " UseCnt: " << var->use_cnt << " UsedRank: " << var->rank << " IsUsedInCond: " << var->is_used_in_cond << endl;
		}
	}

	/*cout << "\n Basic_Blocks --------------------------------\n";
	auto basic_blocks = ConstructRootBlocks(ast);
	for (auto& i : basic_blocks) {
		i->simplify();
	}
	for (auto& i : basic_blocks)
		i->Dump();*/

	cout << "\n RISC-V --------------------------------\n";
	ast->GenerateRSICV();
	CloseOutputStream();
	cout << "\n END --------------------------------\n";
#else

	//O1_optimize = false;



	InitOutputStream(output_path.c_str());
	string file_name(input_path);
	const int backslash_index = max(int(file_name.find_last_of('\\')), int(file_name.find_last_of('/')));
	file_name = file_name.substr(backslash_index + 1);
	output << "\t.file \"" << file_name << "\"\n";
	
	ast->Analyze();

	if(O1_optimize) // 对于大于 1MB 的文件, 进行死代码优化
		ast->Delete_Useless();
	//if(O1_optimize)
	//{
	//	ast->Delete_Useless();
	//	//ast->Unroll_Loop();
	//	calc_use();
	//}
	ast->GenerateRSICV();
	CloseOutputStream();
#endif

}
