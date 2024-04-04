#include"Symbol_Table.h"
#include<iostream>
#include<memory>
#include<vector>
#include<map>
#include "AST.h"
#include "Utility.h"

using namespace std;

int cnt = 0;

//current available register，即当前可用于存储临时变量的寄存器
set<RegisterInfo> avail_register_int;

set<RegisterInfo> avail_register_float;

set<RegisterInfo> extra_register;

vector<map<string, Info>> Symbol_Table::infos;

vector<vector<int>> Symbol_Table::avail_memory;

vector<map<string, shared_ptr<VarDef>>> Symbol_Table::use_count;

map<string, vector<shared_ptr<VarDef>>> in_func_var_use;

vector<map<string, int>> is_var_push;

// 考虑到通过语法树直接生成汇编，所以我是打算为func专门搞一个符号表叫FuncTables
map<string, shared_ptr<FuncDefAST>> Symbol_Table::FuncTables;

void Symbol_Table::NewScope() {
	infos.emplace_back();
	use_count.emplace_back();
	is_var_push.emplace_back();
}

void Symbol_Table::InsertPredefinedFunction()
{
	Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::INT, make_unique<string>("getint"), make_unique<BlockAST>()));
	Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::INT, make_unique<string>("getch"), make_unique<BlockAST>()));
	Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::FLOAT, make_unique<string>("getfloat"), make_unique<BlockAST>()));

	//Tag: 数组！！！！

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putint"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>()));
	}

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putch"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>()));
	}

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::FLOAT;
		temp_params->push_back(param);
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putfloat"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>()));
	}

	{
		// getarray
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		auto func = make_shared<FuncDefAST>(ValueType::INT, make_unique<string>("getarray"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		Symbol_Table::Insert(func);
	}
	{
		// getfarray
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::FLOAT;
		temp_params->push_back(param);
		auto func = make_shared<FuncDefAST>(ValueType::INT, make_unique<string>("getfarray"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		Symbol_Table::Insert(func);
	}
	//Tag: 数组！！！！
	{
		// putarray
		 vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		 {
			 auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
			 param->type = ValueType::INT;
			 temp_params->push_back(param);
		 }
		 {
			 auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_2")));
			 param->type = ValueType::INT;
			 temp_params->push_back(param);
		 }
		 auto func = make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putarray"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		 Symbol_Table::Insert(func);
	}
	{
		// putfarray
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		{
			auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
			param->type = ValueType::INT;
			temp_params->push_back(param);
		}
		{
			auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_2")));
			param->type = ValueType::FLOAT;
			temp_params->push_back(param);
		}
		auto func = make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putfarray"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		Symbol_Table::Insert(func);
	}

	//Tag: 格式化输出
	{
		/*vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>(); // 我（LRX）觉得这没用，插入个函数名就行
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::STRING;
		temp_params->push_back(param);*/
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putf"), make_unique<BlockAST>()));
	}

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("line_no")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("_sysy_starttime"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>()));
	}
	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("line_no")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		Symbol_Table::Insert(make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("_sysy_stoptime"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>()));
	}

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("n")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		auto func = make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("scut_memset_int"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		FuncDefAST::memset_int = func;
		Symbol_Table::Insert(func);
	}

	{
		vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
		auto param = make_shared<VarDefAST>(make_shared<ArrayVarDef>(make_unique<string>("param_1")));
		param->type = ValueType::FLOAT;
		temp_params->push_back(param);
		param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("n")));
		param->type = ValueType::INT;
		temp_params->push_back(param);
		auto func = make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("scut_memset_float"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
		FuncDefAST::memset_float = func;
		Symbol_Table::Insert(func);
	}

}

bool Symbol_Table::ExitScope() {
	if (infos.size() <= 1) return false;
	infos.pop_back();
	if (use_count.size() <= 1) return false;
	use_count.pop_back();
	if (is_var_push.size() <= 1) return false;
	is_var_push.pop_back();
	return true;
}

void Symbol_Table::ShowTable() {
	for (int i = infos.size() - 1; i >= 0; i--)
	{
		auto& table = infos[i];
		for (auto i : table) {
			cout << i.first << ' ' << ToString(i.second.address_sp) << "\t";
		}
		cout << endl;
	}
}

bool Symbol_Table::Insert(shared_ptr<FuncDefAST> ready_to_table)
{
	ValueType ltype = ready_to_table->type;
	if (infos.back().find(*ready_to_table->name) != infos.back().end()) return false;
	infos.back().insert({ *ready_to_table->name , {-1, "", EvaluateType::canNot, Value(), ltype}});
	if (FuncTables.find(*ready_to_table->name) != FuncTables.end()) return false;
	FuncTables[*ready_to_table->name] = ready_to_table; // 所有函数在被插入FuncTables时都默认不可编译期间计算
	FuncTables[*ready_to_table->name]->canEvaluate = EvaluateType::canNot; 
	return true;
}
void Symbol_Table::ClearTable()
{
	infos.clear();
	avail_memory.clear();
	FuncTables.clear();
}

bool Symbol_Table::Insert(const VarDefAST& ready_to_table)
{
	ValueType ltype = ready_to_table.type;
	for (auto& i : ready_to_table.var_defs) { // 这里可没有写完！！！没有写数组的！
		if(i->IsSingle()) {
			const auto& Single_Var_TMP = dynamic_pointer_cast<SingleVarDef>(i);
			if (infos.back().find(*Single_Var_TMP->name) != infos.back().end()) return false;

			if(ready_to_table.is_global) {
				infos.back().insert({ *Single_Var_TMP->name , {-1, "", EvaluateType::canNot, Value(), ltype,
				ready_to_table.is_global, ready_to_table.is_const} });
			}
			else {
				infos.back().insert({ *Single_Var_TMP->name , {avail_memory.back().back(), "", EvaluateType::canNot, Value(), ltype,
				ready_to_table.is_global, ready_to_table.is_const} });
				avail_memory.back().pop_back();
			}
			infos.back()[*Single_Var_TMP->name].freq = Single_Var_TMP->use_cnt;
		}
		else {
			// TODO: 数组
			const auto& Single_Var_TMP = dynamic_pointer_cast<ArrayVarDef>(i);
			if (ready_to_table.is_global) {
				infos.back().insert({ *Single_Var_TMP->name , {-1, "", EvaluateType::canNot, Value(), ltype,
				ready_to_table.is_global, ready_to_table.is_const} });
			}
			else {
				int mem_address;
				if (Single_Var_TMP->init_values.size()) {
					// mem_address.resize((Single_Var_TMP->init_values.size()));
					avail_memory.back().erase(avail_memory.back().end() - (Single_Var_TMP->init_values.size() - 1), avail_memory.back().end());
					mem_address = avail_memory.back().back();
					avail_memory.back().pop_back();
				}
				else {
					avail_memory.back().pop_back();
					mem_address = avail_memory.back().back();
					avail_memory.back().pop_back();
				}
				infos.back().insert({ *Single_Var_TMP->name , {mem_address, "", EvaluateType::canNot, Value(), ltype,
					ready_to_table.is_global, ready_to_table.is_const}});
			}
			int n = Single_Var_TMP->dimension.size();
			vector<int> dim(n);
            for (int j = 0; j < n; ++j) {
				if (Single_Var_TMP->dimension.at(j)) {
					Single_Var_TMP->dimension.at(j)->Analyze();
					auto rs = Single_Var_TMP->dimension.at(j)->Evaluate();        
					ValueType _TMP_Type = Single_Var_TMP->dimension.at(j)->GetType();
					int tmp_;
					if (rs.first == EvaluateType::can) {
						if (_TMP_Type == ValueType::INT) { tmp_ = rs.second.int_val; }
						else { tmp_ = rs.second.float_val; }
						dim[j] = tmp_;
					}
					else { error_sys("dimension can not be calculated"); }
				}
				else { dim[j] = -1; }
            }
			int index = infos.size() - 1;
			for (; index >= 0; index--) {
				if (infos[index].find(Single_Var_TMP->GetName()) != infos[index].end()){
					infos[index][Single_Var_TMP->GetName()].dimensions = dim;
					break;
				}
			}
            Symbol_Table::ChangeDimension(Single_Var_TMP->GetName(), dim);
		}
	}
	return true;
}
bool Symbol_Table::Insert(const SingleVarDef& ready_to_table, ValueType type, bool IsGlobal, bool IsConst)
{
	if (infos.back().find(*ready_to_table.name) != infos.back().end()) return false;
	infos.back().insert({ *ready_to_table.name , {-1, "", EvaluateType::canNot, Value(), type, IsGlobal, IsConst}});
	return true;
}
bool Symbol_Table::Insert(const ArrayVarDef& ready_to_table, ValueType type, bool IsGlobal, bool IsConst)
{
	if (infos.back().find(*ready_to_table.name) != infos.back().end()) return false;
	infos.back().insert({ *ready_to_table.name , {-1, "", EvaluateType::canNot, Value(), type, IsGlobal, IsConst}});
	int n = ready_to_table.dimension.size();
	vector<int> dim(n);
	for (int j = 0; j < n; ++j) {
		if (ready_to_table.dimension.at(j)) {
			ready_to_table.dimension.at(j)->Analyze();
			auto rs = ready_to_table.dimension.at(j)->Evaluate();        
			ValueType _TMP_Type = ready_to_table.dimension.at(j)->GetType();
			int tmp_;
			if (rs.first == EvaluateType::can) {
				if (_TMP_Type == ValueType::INT) {
					tmp_ = rs.second.int_val;
				}
				else {
					tmp_ = rs.second.float_val;
				}
				dim[j] = tmp_;
			}
			else {
				error_sys("dimension can not be calculated");
			}
		}
		else {
			dim[j] = -1;
		}
	}
	// DEBUG
	// for (int j = 0; j < n; ++j) {
	// 	cout << dim[j] << endl;
	// }
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(ready_to_table.GetName()) != infos[index].end()){
			infos[index][ready_to_table.GetName()].dimensions = dim;
			// for (int j = 0; j < n; ++j) {
			// 	cout << "size:" << infos[index][ready_to_table.GetName()].dimensions[j] << endl;
			// }
			break;
		}
	}
	return true;
}
bool Symbol_Table::Insert(const SingleVarDef& ready_to_table, ValueType type, Value _val, bool IsGlobal, bool IsConst)
{
	if (infos.back().find(*ready_to_table.name) != infos.back().end()) return false;
	EvaluateType TMP = EvaluateType::can;
	if (IsGlobal && !IsConst) TMP = EvaluateType::canNot; // 非const的全局变量不能计算
	infos.back().insert({ *ready_to_table.name , {-1, "", TMP, _val, type, IsGlobal, IsConst}});
	return true;
}
bool Symbol_Table::Insert(const ArrayVarDef& ready_to_table, ValueType type, Value _val, bool IsGlobal, bool IsConst)
{
	if (infos.back().find(*ready_to_table.name) != infos.back().end()) return false;
	infos.back().insert({ *ready_to_table.name , {-1, "", EvaluateType::canNot, Value(), type, IsGlobal, IsConst}});
	int n = ready_to_table.dimension.size();
	vector<int> dim(n);
	for (int j = 0; j < n; ++j) {
		if (ready_to_table.dimension.at(j)) {
			ready_to_table.dimension.at(j)->Analyze();
			auto rs = ready_to_table.dimension.at(j)->Evaluate();        
			ValueType _TMP_Type = ready_to_table.dimension.at(j)->GetType();
			int tmp_;
			if (rs.first == EvaluateType::can) {
				if (_TMP_Type == ValueType::INT) {
					tmp_ = rs.second.int_val;
				}
				else {
					tmp_ = rs.second.float_val;
				}
				dim[j] = tmp_;
			}
			else {
				error_sys("dimension can not be calculated");
			}
		}
		else {
			dim[j] = -1;
		}
	}
	// for (int j = 0; j < n; ++j) {
	// 	cout << dim[j] << endl;
	// }
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(ready_to_table.GetName()) != infos[index].end()){
			infos[index][ready_to_table.GetName()].dimensions = dim;
			break;
		}
	}
	return true;
}
bool Symbol_Table::Insert_Use(shared_ptr<VarDef> VarAst) {
	VarAst->use_cnt = 0;
	if (use_count.back().find(VarAst->GetName()) != use_count.back().end()) return false;
	use_count.back().insert({ VarAst->GetName(), VarAst });
	return true;
}
bool Symbol_Table::GetExist(const string& name)
{
	for (int index = infos.size() - 1; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
			return true;
	}
	return false;
}

Info Symbol_Table::GetInfo(const string& name)
{
	for (int index = infos.size() - 1; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			return infos[index][name];
		}
	}
	error_sys("In Symbol_Table::GetInfo: name not exist in the symbol table");
	return {-1, "", EvaluateType::canNot, Value(), ValueType::UNKNOWN};
}

bool Symbol_Table::GetIsGlobal(const string& name) {
	for (int index = infos.size() - 1; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			return infos[index][name].is_global;
		}
	}
	error_sys("In Symbol_Table::GetIsGlobal: name not exist in the symbol table");
	return false;
}

bool Symbol_Table::GetIsConst(const string& name) {
	for (int index = infos.size() - 1; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			return infos[index][name].is_const;
		}
	}
	error_sys("In Symbol_Table::GetIsConst: name not exist in the symbol table");
	return false;
}

bool Symbol_Table::isInCurrentScope(const string& name) {
	return (infos.back().find(name) != infos.back().end());
}

void Symbol_Table::clearChanged(vector<pair<string, Value>>& changed) {
	for(int i = 0; i < changed.size(); i++){
		for (int index = infos.size() - 1; index >= 0; index--) {
			if (infos[index].find(changed[i].first) != infos[index].end()){
				infos[index][changed[i].first].Info_value = changed[i].second;
				break;
			}
		}
	}
}

ValueType Symbol_Table::GetType(const string& name)
{
	return GetInfo(name).type;
}

shared_ptr<FuncDefAST> Symbol_Table::GetFuncDef(const string& name)
{
	return FuncTables[name];
}

int Symbol_Table::GetAddress(const string& name)
{
	return GetInfo(name).address_sp;
}
bool Symbol_Table::SetAddress(const string& name, int target_address)
{
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			auto& i = infos[index][name];
			i.address_sp = target_address;
			i.reg = "";
			return true;
		}
	}
	return false;
}

void Symbol_Table::ChangeEvaluate(const string& name) 
{
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end()) {
			infos[index][name].canEvaluate = EvaluateType::canNot;
			infos[index][name].Info_value = Value();
			break;
		}
	}
}

void Symbol_Table::ChangeVal(const string& name, Value _value)
{
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end()){
			if (infos[index][name].is_global == false) {
				infos[index][name].canEvaluate = EvaluateType::can;
				infos[index][name].Info_value = _value;
			}
			break;
		}
	}
}
void Symbol_Table::ChangeDimension(const string& name, const vector<int>& dim) {
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end()){
			infos[index][name].dimensions = dim;
			break;
		}
	}
}
vector<int> Symbol_Table::GetDimension(const string& name) {
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end()){
			return infos[index][name].dimensions;
		}
	}
	error_sys("error could not find array");
	return {};
}
size_t Symbol_Table::GetSize(const string& name) {
	for (int index = infos.size() - 1; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			return infos[index][name].dimensions.size();
		}
	}
	error_sys("In Symbol_Table::GetIsConst: name not exist in the symbol table");
	return 0;
}
bool Symbol_Table::SetRegister(const string& name, const string& target_reg)
{
	int index = infos.size() - 1;
	for (; index >= 0; index--) {
		if (infos[index].find(name) != infos[index].end())
		{
			auto& i = infos[index][name];
			i.reg = target_reg;
			return true;
		}
	}
	return false;
}
// bool Symbol_Table::SetRegisterArray(const string& name, const string& target_reg) {
// 	int index = infos.size() - 1;
// 	for (; index >= 0; index--) {
// 		if (infos[index].find(name) != infos[index].end()) {
// 			auto& i = infos[index][name];
// 			i.reg = target_reg;
// 			i.address_sp = -1;
// 		}
// 	}
// }
string Symbol_Table::GetRegister(const string& name)
{
	return GetInfo(name).reg;
}

int Symbol_Table::GetFreq(const string& name)
{
	return GetInfo(name).freq;
}

void Symbol_Table::ShiftStack(int shift)
{
	if (shift == 0)
		return;

	bool tags[12] = {};
	for (int i = 0; i < 12; ++i)
	{
		tags[i] = (avail_register_int.find("s" + to_string(i)) != avail_register_int.end());
		if (tags[i])
			avail_register_int.erase("s" + to_string(i));
	}

	if (shift > 2047 || shift < -2048)
	{
		ValueAST shift_ast(shift);
		string shift_reg = shift_ast.GenerateRSICV();
		output << "\tadd sp, sp, " << shift_reg << endl;
		RestoreRegister(shift_reg);
	}
	else
	{
		output << "\taddi sp, sp, " << shift << endl;
	}


	if (shift < 0)
	{
		vector<int> temp;
		for (int i = FuncDefAST::max_param_length * 8; i < -shift; i += 4)
			temp.push_back(i);
		avail_memory.emplace_back(move(temp));

		auto& start = infos[0];
		for(auto& i:start)
		{
			if(i.second.address_sp == -1)
				continue;
			i.second.address_sp -= shift;
		}
	}
	else
	{
		avail_memory.pop_back();

		auto& start = infos[0];
		for (auto i : start)
			i.second.address_sp += shift;
	}

	for (int i = 0; i < 12; ++i)
	{
		if (tags[i])
			avail_register_int.insert("s" + to_string(i));
	}
}

string Symbol_Table::PutRegisterToMemory(string target_reg, bool is_float)
{
	if (target_reg.empty())
	{
		if (avail_register_int.empty())
		{
			if (is_float)
				target_reg = "ft" + to_string(GetRandomInt(0, 6));
			else
				target_reg = "t" + to_string(GetRandomInt(0, 6));
		}
		else
		{
			if(is_float)
				target_reg = RandomFromSet(avail_register_float);
			else
				target_reg = RandomFromSet(avail_register_int);
			return target_reg;
		}
	}

	for (size_t i = 0; i < infos.size(); ++i)
	{
		for (auto& j: infos[i])
		{
			if (j.second.reg == target_reg)
			{
				PrintStoreVariableToMemory(j.first, j.second);
			}
		}
	}
	
	return target_reg;
}

string Symbol_Table::LoadVariableToRegister(const string& name, bool is_float, string target_reg)
{
	//已经在寄存器中
	string reg = GetRegister(name);
	if (!reg.empty())
	{
		if (target_reg.empty())
			return reg;

		if (reg != target_reg)
		{
			PutRegisterToMemory(target_reg, is_float);//将其他变量从当前寄存器移除
			if(is_float)
				output << "\tfmv.s " << target_reg << ", " << reg << endl;
			else
				output << "\tmv " << target_reg << ", " << reg << endl;
			RestoreRegister(reg);
			SetRegister(name, target_reg);
			if(target_reg[0] == 's')
			{
				extra_register.erase(target_reg);
			}
			else
			{
				if (is_float)
					avail_register_float.erase(target_reg);
				else
					avail_register_int.erase(target_reg);
			}
		}
		return target_reg;
	}

	//变量在内存中
	Info info = GetInfo(name);
	if(info.address_sp == -1 && !info.is_global)
		return "";
	//获取目标寄存器位置
	if (target_reg.empty())
	{
		if(!is_float && avail_register_int.empty())
		{
			target_reg = "t" + to_string(GetRandomInt(0, 6));
			PutRegisterToMemory(target_reg, is_float);
		}
		else if(avail_register_float.empty())
		{
			target_reg = "ft" + to_string(GetRandomInt(0, 6));
			PutRegisterToMemory(target_reg, is_float);
		}
		else
		{
			if (is_float)
				CheckAndSetRegister_FLOAT(target_reg);
			else
				CheckAndSetRegister_INT(target_reg);
		}
	}

	info = GetInfo(name);
	if (!info.dimensions.empty() && info.dimensions[0] == -1) // 函数内部使用数组参数
	{
		PrintCommandRelativeToSP("ld", target_reg, info.address_sp);
		//output << "\tld " << target_reg << ", " << info.address_sp << "(sp)" << endl;
		SetRegister(name, target_reg);
		avail_register_int.erase(target_reg);
	}
	else if(info.is_global)//全局变量
	{
		if (is_float)
		{
			output << "\tlla s0, " << name << "\n";
			output << "\tflw " << target_reg << ", 0(s0)\n";
			SetRegister(name, target_reg);
			avail_register_float.erase(target_reg);
		}
		else
		{
			output << "\tlla s0, " << name << "\n";
			output << "\tlw " << target_reg << ", 0(s0)\n";
			SetRegister(name, target_reg);
			if (target_reg[0] == 's')
			{
				extra_register.erase(target_reg);
			}
			else
			{
				avail_register_int.erase(target_reg);
			}
		}
	}
	else//局部变量
	{
		if (is_float)
		{
			PrintCommandRelativeToSP("flw", target_reg, info.address_sp);
			//output << "\tflw " << target_reg << ", " << info.address_sp << "(sp)" << endl;
			SetRegister(name, target_reg);
			avail_register_float.erase(target_reg);
		}
		else
		{
			PrintCommandRelativeToSP("lw", target_reg, info.address_sp);
			//output << "\tlw " << target_reg << ", " << info.address_sp << "(sp)" << endl;
			SetRegister(name, target_reg);
			if (target_reg[0] == 's')
			{
				extra_register.erase(target_reg);
			}
			else
			{
				avail_register_int.erase(target_reg);
			}
		}
	}
	return target_reg;
}

void Symbol_Table::PutAllToMemory(bool only_put_global, bool should_put_extra_register)
{
	if(only_put_global)
	{
		for (auto& j : infos[0])
		{
			if (!should_put_extra_register && j.second.reg[0] == 's')
				continue;
			RestoreRegister(j.second.reg, j.second.freq);
			PrintStoreVariableToMemory(j.first, j.second);
		}
	}
	else
	{
		for(auto& i: infos)
		{
			for(auto& j:i)
			{
				if(!should_put_extra_register && j.second.reg[0] == 's')
					continue;
				RestoreRegister(j.second.reg, j.second.freq);
				PrintStoreVariableToMemory(j.first, j.second);
			}
		}
	}
}

bool Symbol_Table::AddUse(const string& name) {
	for (int index = use_count.size() - 1; index >= 0; index--) {
		if (use_count[index].find(name) != use_count[index].end()) {
			if (is_var_push[index][name] == 0) {
				if(use_count[index][name]->IsSingle())
					in_func_var_use[FuncDefAST::func_name].push_back(use_count[index][name]);
				else
					in_func_var_use[FuncDefAST::func_name].push_back(use_count[index][name]);
				is_var_push[index][name] = 1;
			}
			use_count[index][name]->use_cnt++;
			if (WhileAST::is_While_cond) {
				use_count[index][name]->is_used_in_cond = true;
			}
			return true;
		}
	}
	error_sys("In Symbol_Table::GetInfo: name not exist in the symbol table");
	return false;
}
