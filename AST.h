#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Constants.h"
#include "Utility.h"
#include <set>

class VarDef;
using namespace std;

extern bool O1_optimize;

//记录当前的寄存器变量个数
extern int cnt;
extern set<RegisterInfo> extra_register;
extern set<RegisterInfo> avail_register_int;
extern set<RegisterInfo> avail_register_float;
extern map<string, vector<shared_ptr<VarDef>>> in_func_var_use;

bool CheckAndSetRegister_EXTRA(string& reg);
string CheckAndSetRegister_INT(string& reg);
string CheckAndSetRegister_FLOAT(string& reg);
ValueType GetRegisterType(string reg);
void RestoreRegister(string reg, int freq = 0);

//RAII写法，防止标签忘记输出
class LabelGuard
{
public:
	LabelGuard(string* _start_label, string* _end_jump, bool* _should_restore);
	~LabelGuard();
	string* start_label = nullptr, * end_jump;
	bool *should_restore;
};

class SymbolTableGuard
{
public:
	SymbolTableGuard();
	~SymbolTableGuard();
};

class BaseAST : public enable_shared_from_this<BaseAST>
{
public:
	virtual ~BaseAST() {}

	virtual void Dump() const {}
	
	virtual void Analyze() {}

	virtual void Delete_Useless() {}

	virtual void Unroll_Loop() {}

	//获取当前语句的大小，用于计算栈帧大小
	virtual int GetSize() const { return 0; }

	//生成RSIC-V汇编，并将结果存储到target_reg之中,target_reg必须存在于avail_register中，否则会异常
	//如果target_reg为空，会从avail_register中选择一个寄存器，函数的返回值就是结果的位置
	//对于没有结果的语句，返回值为空("")
	virtual string GenerateRSICV(string target_reg = "") { return ""; }

	template <typename Derived> std::shared_ptr<Derived> shared_from_base() {
		return std::static_pointer_cast<Derived>(shared_from_this());
	}

};

class StmtAST : public BaseAST
{
public:
	string start_label = "", end_jump_label = "";
	static string continue_label, break_label;

	bool should_restore = false; // 如果在If语句中，需要还原符号表修改的内容，那么就设置为true
	
	virtual ASTNodeType GetNodeType() const;

	virtual void whilePioneer() const {};

};

class NullStmtAST: public StmtAST{};

class DefAST : public StmtAST
{
public:
	virtual bool IsFuncDef() const { return false; }
	virtual bool IsMainFuncDef() const { return false; }
};

class DeclAST : public StmtAST
{
public:
	virtual ValueType GetType() const { return ValueType::VOID; }
	virtual std::string GetName() const { return ""; }
	virtual pair<EvaluateType, Value> Evaluate() { return { EvaluateType::canNot, Value()}; }
	virtual Origin addToMul() { return { -1, ValueType::UNKNOWN, "", vector<int>() }; }

	bool in_extra_register = false;
};

class VarDef;

//LValue AST
class ValueAST :public DeclAST
{
public:
	ValueAST(ValueType _value_type);
	ValueAST(int _value);
	ValueAST(float _value);
	ValueAST(unique_ptr<string> _value);
	ValueAST(Value _value, ValueType _type);
	ValueAST(shared_ptr<VarDef> _var_def);
	ValueAST(shared_ptr<VarDef> _var_def, ValueType _type);

	void Dump() const override;

	pair<EvaluateType, Value> Evaluate() override;

	Origin addToMul() override;

	ValueType GetType() const override;

	void Analyze() override;

	std::string GetName() const override;
	string GenerateRSICV(string target_reg = "") override;

	ValueType value_type = ValueType::UNKNOWN;

	// var_def只在赋值语句中的上
	shared_ptr<VarDef> var_def;//如果是变量则存储变量信息，如果是常量则为空
	unique_ptr<string> str;
	//vector<shared_ptr<DeclAST>> dimension;
	Value value;
	EvaluateType canEvaluate; // 常量一开始就是can，变量一开始就是canNot，并且得在这个ValueAST结点Analyze了之后，才可能变成can
};

class ExprAST : public DeclAST
{
public:
	ExprAST(OpType _op, unique_ptr<DeclAST> _roperand);
	ExprAST(OpType _op, unique_ptr<DeclAST> _loperand, unique_ptr<DeclAST> _roperand);
	ExprAST(OpType _op, shared_ptr<DeclAST> _loperand, shared_ptr<DeclAST> _roperand);

	void Dump() const override;

	pair<EvaluateType, Value> Evaluate() override;

	Origin addToMul() override;

	bool IsUnaryOperator() const;

	ValueType GetType() const override;

	void Analyze() override;

	void Simplify(ValueType rtype, Value _value);

	void Simplify(Value _value, int flag, ValueType _type);

	template<typename T1, typename T2>
	void Simplify(OpType op, ValueType ltype, T1 _lval, ValueType rtype, T2 _rval) {
		canEvaluate = EvaluateType::can;
		int flag = 0;
		if (ltype == ValueType::FLOAT || rtype == ValueType::FLOAT)
			flag = 1;
		switch(op){
		case OpType::PLUS: if (flag) value.float_val = _lval + _rval; else value.int_val = _lval + _rval; break;
		case OpType::MINUS: if (flag) value.float_val = _lval - _rval; else value.int_val = _lval - _rval; break;
		case OpType::MUL: if (flag) value.float_val = _lval * _rval; else value.int_val = _lval * _rval; break;
		case OpType::DIV: if (flag) value.float_val = _lval / _rval; else value.int_val = _lval / _rval; break;
		// MOD 就不能写在这里，不然编译不过
		case OpType::LE: value.int_val = (_lval < _rval); break;
		case OpType::LEQ: value.int_val = (_lval <= _rval); break;
		case OpType::GE: value.int_val = (_lval > _rval); break;
		case OpType::GEQ: value.int_val = (_lval >= _rval); break;
		case OpType::EQ: value.int_val = (_lval == _rval); break;
		case OpType::NEQ: value.int_val = (_lval != _rval); break;
		case OpType::AND: value.int_val = (_lval && _rval); break;
		case OpType::OR: value.int_val = (_lval || _rval); break;
		default: error_sys("ExprAST::Simplify");
		}
	}

	string GenerateRSICV(string target_reg = "") override;

	OpType op;
	shared_ptr<DeclAST> loperand, roperand;
	ValueType value_type;

	Value value;
	EvaluateType canEvaluate;

};

class VarDef : public BaseAST
{
public:
	//获取变量名
	virtual string GetName() const = 0;

	//判断变量是否为数组，如果不是则返回false
	virtual bool IsSingle() const = 0;

	//判断当前是否为VarDefAST
	bool is_def = false;

	//判断当前变量是否存放在s寄存器中
	bool in_extra_register = false;

	//用于记录变量的使用次数
	int use_cnt = 0;

	bool is_used_in_cond = false;

	int rank = 0;
};

class SingleVarDef : public VarDef
{
public:
	SingleVarDef(unique_ptr<string> _name);
	SingleVarDef(unique_ptr<string> _name, unique_ptr<DeclAST> _init_value);

	void Dump() const override;
	string GetName() const override;
	bool IsSingle() const override;
	string GenerateRSICV(string target_reg) override;
	int GetSize() const override;

	//存储变量名以及初始值，如果没有初始值则为空，可以使用if(var_single[i].second) 来判断.
	unique_ptr<string> name;
	shared_ptr<DeclAST> init_value;
};

class ListAST {
public:
	bool isBracket = false;
    bool isFinished = false;
	ListAST() : values(nullptr) {}
	ListAST(shared_ptr<DeclAST> value) : values(value) {}
	shared_ptr<DeclAST> values;
	vector<shared_ptr<ListAST>> children;
};
class ArrayVarDef : public VarDef
{
public:
	void Rval(string& target_reg, vector<int>& dim);
	void GlobalParamenterInitialize(string& target_reg);
	void LocalParameterInitialize(string& target_reg);
	void Assignment(string& target_reg);
	void FuncCallParameter(string& target_reg, vector<int>& dim);
	ArrayVarDef(unique_ptr<string> _name);
	ArrayVarDef(unique_ptr<string> _name, vector<shared_ptr<DeclAST>> _dimension);
	ArrayVarDef(unique_ptr<string> _name, vector<shared_ptr<DeclAST>> _dimension, unique_ptr<DeclAST> init_value);
	string GenerateRSICV(string target_reg) override;
	int GetSize() const override;
	void Dump() const override;
	string GetName() const override;
	bool IsSingle() const override;
	string LoadShiftToRegister(vector<int>& dim, string shift_reg = "");
	
	string GetIdentifier(int index);
	int zero_cnt = 0;	
	//存储数组声明
	shared_ptr<DeclAST> init_value;
	unique_ptr<string> name;
	vector<shared_ptr<DeclAST>> dimension;
	vector<shared_ptr<DeclAST>> init_values;
	shared_ptr<ListAST> list;
	
};

class VarDefAST : public DefAST
{
public:
	VarDefAST(unique_ptr<string> _name);

	VarDefAST(shared_ptr<VarDef> var);

	void Append(shared_ptr<VarDefAST> other);

	void Dump() const override;
	
	void Simplify(shared_ptr<SingleVarDef> targetAst, Value _value, ValueType _type);

	void Analyze() override;

	void Delete_Useless(vector<shared_ptr<StmtAST>>& targetStmts, int index);

	void Delete_Useless(vector<shared_ptr<DefAST>>& nodes, int index);

	string GenerateRSICV(string target_reg = "") override;

	int GetSize() const override;
	ASTNodeType GetNodeType() const override;

	bool is_global = false;
	bool is_const = false;

	ValueType type = ValueType::UNKNOWN;
	vector<shared_ptr<VarDef>> var_defs; 

	static bool is_current_global; // 用于判断当前变量是否为全局变量
	static ValueType current_var_type;
};

class AssignmentAST : public StmtAST
{
public:
	AssignmentAST(unique_ptr<ValueAST> _lval_ref, unique_ptr<DeclAST> _init_value);
	
	void Dump() const override;

	ASTNodeType GetNodeType() const override;

	void Simplify(shared_ptr<AssignmentAST> targetAst, Value _value, ValueType _type);

	void Analyze() override;

	void whilePioneer() const override;

	ValueType GetLType();
	
	string GenerateRSICV(string target_reg) override;

	ValueType lval_type;
	//存储变量名以及赋值表达式
	shared_ptr<ValueAST> lval_ref;
};


class BlockAST : public StmtAST
{
public:
	BlockAST();
	BlockAST(shared_ptr<StmtAST> stmt);
	BlockAST(unique_ptr<vector<shared_ptr<StmtAST>>> _stmts);
	void simplify_memset();
	void simplify_one_array(int i);
	void Dump() const override;

	ASTNodeType GetNodeType() const override;
	
	string GenerateRSICV(string target_reg = "") override;

	int GetSize() const override;

	void Analyze() override;

	void Delete_Useless() override;

	void Unroll_Loop() override;

	void whilePioneer() const override;

	unique_ptr<vector<shared_ptr<StmtAST>>> stmts;
};

class ReturnAST : public StmtAST
{
public:
	ReturnAST() = default;
	ReturnAST(shared_ptr<DeclAST> _ret_value);

	void Dump() const override;

	void Analyze() override;

	void Simplify(shared_ptr<ReturnAST> targetAst, Value _value, ValueType _type);
	
	ASTNodeType GetNodeType() const override;

	string GenerateRSICV(string target_reg = "") override;

	shared_ptr<DeclAST> ret_value;
};

class ContinueAST : public StmtAST
{
public:
	void Dump() const override;
	string GenerateRSICV(string target_reg) override;

	ASTNodeType GetNodeType() const override;
};

class BreakAST :public StmtAST
{
public:
	void Dump() const override;

	string GenerateRSICV(string target_reg) override;

	ASTNodeType GetNodeType() const override;
};

class WhileAST :public StmtAST
{
public:
	WhileAST(unique_ptr<DeclAST> _cond, shared_ptr<StmtAST> _block);

	void Dump() const override;

	void Analyze(vector<shared_ptr<StmtAST>>& targetStmts, int index);

	void Delete_Useless() override;

	void Unroll_Loop(vector<shared_ptr<StmtAST>>& targetStmts, int index, int flag, int from_value, string from_name);

	void whilePioneer() const override;

	void Simplify(shared_ptr<WhileAST> targetAst, Value _value, ValueType _type);

	ASTNodeType GetNodeType() const override;

	int GetSize() const override;
	
	string GenerateRSICV(string target_reg) override;

	
	shared_ptr<DeclAST> cond;
	shared_ptr<StmtAST> block;

	Value value;
	EvaluateType canEvaluate;
	static bool is_While; // 用于判断是否在while语句中
	static bool is_While_cond;
};

class IfAST :public StmtAST
{
public:
	IfAST(shared_ptr<DeclAST> _cond, shared_ptr<StmtAST> _true);
	IfAST(shared_ptr<DeclAST> _cond, shared_ptr<StmtAST> _true, shared_ptr<StmtAST> _false);

	void Dump() const override;

	void Analyze(vector<shared_ptr<StmtAST>>& targetStmts, int index);

	void Delete_Useless() override;

	void Unroll_Loop() override;

	void whilePioneer() const override;

	void Simplify(shared_ptr<IfAST> targetAst, Value _value, ValueType _type);

	ASTNodeType GetNodeType() const override;
	
	string GenerateRSICV(string target_reg) override;

	int GetSize() const override;

	shared_ptr<DeclAST> cond;
	shared_ptr<StmtAST> true_block, false_block;

	Value value;
	EvaluateType canEvaluate;
	static bool is_If; // 用于判断赋值语句是否在if-else语句中
};

class FuncDefAST :public DefAST
{
public:
	FuncDefAST(ValueType _type, unique_ptr<string> _name, unique_ptr<BlockAST> _block);
	FuncDefAST(ValueType _type, unique_ptr<string> _name, unique_ptr<vector<shared_ptr<VarDefAST>>> _params, unique_ptr<BlockAST> _block);
	FuncDefAST(const FuncDefAST& o) = delete;
	FuncDefAST(FuncDefAST&& o) = delete;

	void Dump() const override;
	
	//尾递归消除
	void Simplify();

	void Analyze() override;

	void Delete_Useless() override;

	void Unroll_Loop() override;

	string GenerateRSICV(string target_reg = "") override;
	ASTNodeType GetNodeType() const override;

	//当前函数的返回值类型
	ValueType type;
	unique_ptr<string> name;
	//每个VarDefAST都只包含有一个参数
	unique_ptr<vector<shared_ptr<VarDefAST>>> params;
	shared_ptr<BlockAST> block;

	Value value;
	EvaluateType canEvaluate; // 我只在符号表的funcdef中修改了canEvaluate，也就是说原本的FuncDefAST的value和canEvaluate都是没有赋值过的

	int s_register_used; // 用于记录当前函数中已经使用的s寄存器的个数

	//可用于返回值类型检查，储存的是当前语句所在函数的返回值类型
	static ValueType func_type;

	// 可用于判断函数是否可以编译计算，储存的是当前语句所在函数的函数名
	static string func_name;

	//记录当前所在函数最后一个语句ret的label
	static string current_return_label;

	//存储最长的参数个数
	static int max_param_length;

	static shared_ptr<FuncDefAST> memset_int, memset_float;
};

class FuncCallAST :public DeclAST
{
public:
	FuncCallAST(unique_ptr<string> _name);
	FuncCallAST(unique_ptr<string> _name, unique_ptr<vector<shared_ptr<DeclAST>>> _params);

	void Dump() const override;

	ValueType GetType() const override;

	ASTNodeType GetNodeType() const override;

	void Analyze() override;
	
	//尝试内联函数
	void Simplify();

	pair<EvaluateType, Value> Evaluate() override;
	string GenerateRSICV(string target_reg = "") override;
	int GetSize() const override;

	//Function name
	unique_ptr<string> name;

	//Function parameters，每一个DeclAST都只包含有一个参数的值
	unique_ptr<vector<shared_ptr<DeclAST>>> param_vals;
	shared_ptr<FuncDefAST> func_def; // 直到Analyze以后这个funcDef才会被赋值

	ValueType value_type;

	Value value;
	EvaluateType canEvaluate;

	//当函数内联成功时会被赋值
	BlockAST inline_block;

	static int funccall_cnt;

	static int starttime_cnt, stoptime_cnt;
};

class CompUnitAST : public BaseAST
{
public:
	void Dump() const override;
	
	void Append(shared_ptr<DefAST> node);

	void Analyze() override;

	void Delete_Useless() override;

	void Unroll_Loop() override;

	string GenerateRSICV(string target_reg = "") override;

	/*void Simplify() override;*/

	vector<shared_ptr<DefAST>> nodes;
};
