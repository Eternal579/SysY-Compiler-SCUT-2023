#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#endif

#include<iostream>
#include"AST.h"
#include<vector>
#include<map>
#include<memory>
#include "Constants.h"

using namespace std;

extern int cnt;
extern set<RegisterInfo> extra_register;
extern set<RegisterInfo> avail_register_int;
extern set<RegisterInfo> avail_register_float;
extern map<string, vector<shared_ptr<VarDef>>> in_func_var_use;
extern vector<map<string, int>> is_var_push;
class Symbol_Table {
public:
	static vector<map<string, Info>> infos;
	static vector<vector<int>> avail_memory;
	static vector<map<string, shared_ptr<VarDef>>> use_count;
public:
	static vector<int> GetDimension(const string& name);
	static map<string, shared_ptr<FuncDefAST>> FuncTables;
	static void ChangeDimension(const string& name, const vector<int>& dim);
	static size_t GetSize(const string& name);
	static bool Insert(const VarDefAST& ready_to_table); // 执行具体插入操作
	static bool Insert(const SingleVarDef& ready_to_table, ValueType type, bool IsConst, bool IsGlobal); // 执行具体插入操作
	static bool Insert(const ArrayVarDef& ready_to_table, ValueType type, bool IsConst, bool IsGlobal); // 执行具体插入操作
	static bool Insert(const SingleVarDef& ready_to_table, ValueType type, Value _value, bool IsConst, bool IsGlobal); // 执行具体插入操作，并且顺带插入值
	static bool Insert(const ArrayVarDef& ready_to_table, ValueType type, Value _value, bool IsConst, bool IsGlobal); // 执行具体插入操作，并且顺带插入值
	static bool Insert(shared_ptr<FuncDefAST> ready_to_table); // 执行具体插入操作
	static bool Insert_Use(shared_ptr<VarDef> singleAst);
	//static bool Insert(const shared_ptr<DefAST>&& ready_to_table); // 执行具体插入操作
	//static bool Insert(const unique_ptr<vector<shared_ptr<VarDefAST>>>& params); // 执行具体插入操作

	static void NewScope(); // 创建一个新的局部作用域的符号表

	static void InsertPredefinedFunction(); // 插入预定义函数，例如getint()

	static bool ExitScope(); // 退出当前局部作用域

	static void ShowTable();

	static void ClearTable();

	static void clearChanged(vector<pair<string, Value>>& changed);

	//static shared_ptr<DefAST> GetDecl(const string& name);
	static bool GetExist(const string& name);
	static Info GetInfo(const string& name);

	static bool GetIsGlobal(const string& name);
	static bool GetIsConst(const string& name);
	static bool isInCurrentScope(const string& name); // 用于判断当前变量是否属于本作用域下的

	static ValueType GetType(const string& name);

	static shared_ptr<FuncDefAST> GetFuncDef(const string& name);

	static int GetAddress(const string& name);
	static bool SetAddress(const string& name, int target_address);
	static void ChangeEvaluate(const string& name);
	static void ChangeVal(const string& name, Value _value);
	static bool AddUse(const string& name);

	static bool SetRegister(const string& name, const string& target_reg);
	static string GetRegister(const string& name);
	static int GetFreq(const string& name);

	static void ShiftStack(int shift);

	//如果目标寄存器被占用，则将其内容存入内存
	//如果传入空字符串，则会选择一个变量放入内存，并返回一个可用的寄存器
	static string PutRegisterToMemory(string target_reg = "", bool is_float = false);

	//将变量的值加载到目标寄存器中，如果变量已经在另一个寄存器中，那么变量的值会移动到target_reg中
	static string LoadVariableToRegister(const string& name, bool is_float = false, string target_reg = "");
	
	static void PutAllToMemory(bool only_put_global, bool should_put_extra_register = false);

};