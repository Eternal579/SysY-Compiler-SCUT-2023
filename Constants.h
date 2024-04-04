#pragma once
#include <string>
#include <memory>
#include <vector>
using namespace std;

//函数内联时的最大函数体长度
#define INLINE_FUNCTION_LIMIT 10

enum class OpType {

	// Arithmetic binary operators
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD,

	// relational binary operators
	LE,
	LEQ,
	GE,
	GEQ,
	EQ,
	NEQ,

	// Logical binary operators
	AND,
	OR,

	// Logical unary operators
	NOT,

	// Special indicator
	CONST_INT,
	CONST_FLOAT,
	LVAL,
	FUNC_CALL,
};

union Value
{
	int int_val;
	float float_val;
};

enum class ValueType
{
	UNKNOWN, //δ֪���ͣ���ʾ����
	VOID, //VOID
	INT,
	FLOAT,
	IDENT,
	STRING,
};

enum class ASTNodeType
{
	None,
	ASSIGN,
	BLOCK,
	IF,
	WHILE,
	CONTINUE,
	BREAK,
	RETURN,
	FUNCCALL,
	VARDEF,
	FUNCDEF,
};

enum class EvaluateType
{
	/*Fail,
	IsValue,
	EvalSuccess*/
	can,
	canNot,
};

//Used in Symbol table.

struct Info
{
	//address of variable in memory relative to pointer sp.
	int address_sp;
	//Used to generate RSIC-V ASM, mapping variable name to a register
	std::string reg;

	EvaluateType canEvaluate; // 用于在符号表中查找时判断此变量是否可算
	Value Info_value; // NULL表示存在符号表的项是不可以编译期间计算的

	//type of this variable.
	ValueType type;
	bool is_global;
	bool is_const;
	vector<int> dimensions;
	int freq;
};

//用于输出浮点数
union HEX {
	float num;
	unsigned char hex_num[4];
};

struct Origin { // 用来判断加号运算两边是否可以化简为乘法运算
	int cnt;
	ValueType type;
	string name;
	vector<int>dim;
	bool operator== (const Origin b) const {
		if (this->cnt == -1 || b.cnt == -1)return false;
		if (this->type != b.type) return false;
		if (this->name != b.name) return false;
		for (int index = 0; index < dim.size(); index++) if (this->dim[index] != b.dim[index]) return false;
		return true;
	}
};

//用于启发式寄存器分配优化
struct RegisterInfo
{
	RegisterInfo(const char* _name) :name(_name), freq(0) {}
	RegisterInfo(string _name) :name(move(_name)), freq(0) {}
	RegisterInfo(string _name, int _freq): name(move(_name)), freq(_freq){}

	string name;//变量名
	int freq;//变量使用频率

	bool operator==(const RegisterInfo& o)const
	{
		return name == o.name;
	}

	bool operator<(const RegisterInfo& o)const
	{
		return name < o.name;
	}
};