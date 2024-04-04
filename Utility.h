#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <exception>
#include "Constants.h"
#include <vector>
#include <fstream>
#include <set>

using namespace std;

template<typename T>
void error_sys(T message)
{
	std::cout << message << std::endl;
	//throw exception(message);
	exit(1);
}

inline void Print(string msg, bool new_line = true)
{
	cout << msg;
	if (new_line)
		cout << std::endl;
}

inline void Print(const char* msg, bool new_line = true)
{
	cout << msg;
	if (new_line)
		cout << std::endl;
}

inline void LRXPrint(ValueType tmp) {
	if (tmp == ValueType::FLOAT) {
		cout << "FLOAT&&&&&&&&\n";
	}
	else if (tmp == ValueType::INT) {
		cout << "INT&&&&&&&&&&\n";
	}
	else if (tmp == ValueType::VOID) {
		cout << "VOID&&&&&&&&&&\n";
	}
	else if (tmp == ValueType::IDENT) {
		cout << "IDENT&&&&&&&&&&\n";
	}
	else if (tmp == ValueType::UNKNOWN) {
		cout << "UNKNOWN&&&&&&&&&&\n";
	}
	else {
		cout << "WTF&&&&&&&&&&&&&&\n";
	}
}

//ToString()??????????????????????
//ToIR()???????????IR????

inline std::string ToString(OpType op)
{
	switch (op) {
	case OpType::PLUS: return "PLUS";
	case OpType::MINUS: return "MINUS";
	case OpType::MUL: return "MUL";
	case OpType::DIV: return "DIV";
	case OpType::MOD: return "MOD";
	case OpType::LE: return "LE";
	case OpType::LEQ: return "LEQ";
	case OpType::GE: return "GE";
	case OpType::GEQ: return "GEQ";
	case OpType::EQ: return "EQ";
	case OpType::NEQ: return "NEQ";
	case OpType::AND: return "AND";
	case OpType::OR: return "OR";
	case OpType::NOT: return "NOT";
	case OpType::CONST_INT: return "CONST_INT";
	case OpType::CONST_FLOAT: return "CONST_FLOAT";
	case OpType::LVAL: return "LVAL";
	case OpType::FUNC_CALL: return "FUNC_CALL";
	default: return "UNKNOWN";
	}
}

inline std::string ToIR(OpType op)
{
	switch (op) {
	case OpType::PLUS: return "add";
	case OpType::MINUS: return "sub";
	case OpType::MUL: return "mul";
	case OpType::DIV: return "div";
	case OpType::MOD: return "mod";
	case OpType::LE: return "lt";
	case OpType::LEQ: return "le";
	case OpType::GE: return "gt";
	case OpType::GEQ: return "ge";
	case OpType::EQ: return "eq";
	case OpType::NEQ: return "neq";
	case OpType::AND: return "AND";
	case OpType::OR: return "OR";
	case OpType::NOT: return "NOT";
	case OpType::CONST_INT: return "CONST_INT";
	case OpType::CONST_FLOAT: return "CONST_FLOAT";
	case OpType::LVAL: return "LVAL";
	case OpType::FUNC_CALL: return "FUNC_CALL";
	default: return "UNKNOWN";
	}
}

inline std::string ToString(ValueType val)
{
	switch (val) {
	case ValueType::VOID: return "EMPTY";
	case ValueType::INT: return "INT";
	case ValueType::FLOAT: return "FLOAT";
	case ValueType::IDENT: return "IDENT";
	case ValueType::STRING: return "STRING";
	default: return "UNKNOWN";
	}
}
inline std::string ToIR(ValueType val)
{
	switch (val) {
	case ValueType::VOID: return "void";
	case ValueType::INT: return "i32";
	case ValueType::FLOAT: return "float";
	case ValueType::IDENT:
		error_sys("std::string ToIR(ValueType val): variable type is IDENT");
		return "IDENT";
	default:
		error_sys("std::string ToIR(ValueType val): variable type is UNKNOWN");
		return "UNKNOWN";
	}
}

inline std::string ToString(bool val)
{
	if (val)return "true";
	return "false";
}

inline std::string ToIR(bool val)
{
	if (val)return "true";
	return "false";
}

inline std::string ToString(ValueType value_type, Value* value)
{
	switch (value_type)
	{
	case ValueType::INT:
		return std::to_string(value->int_val);
	case ValueType::FLOAT:
		return std::to_string(value->float_val);
	case ValueType::IDENT:
		return "IDENT";
	case ValueType::VOID:
		return "VOID";
	case ValueType::UNKNOWN:
		return "UNKNOWN";
	}
	return "ERROR";
}

string ToString(ASTNodeType type);

extern ofstream output;

void InitOutputStream(const char* path);

void CloseOutputStream();

// Get jump label using cnt. For example, if cnt is 1, return "label1"
string GetLabel(int cnt);

// Return an integer in range [l, r]
int GetRandomInt(int l, int r);

// 从set中获取随机元素，复杂度最高为O(n)
//template<typename T>
string RandomFromSet(const set<RegisterInfo>& s);

// 类型转换，返回转换后的寄存器
string TypeConvert(ValueType src_type, string src_reg, ValueType res_type);

// 将十进制浮点数转换为十六进制，用于FloatPool
string DecimalFloatToHex(float num);

void PrintStoreVariableToMemory(const string& name, Info& info);

void PrintCommandRelativeToSP(string command, string reg, int offset);

//将b中所有元素移动到a中，并清空b
template<typename T>
void ConcatVector(vector<T>& a, vector<T>& b)
{
	a.insert(a.end(), std::make_move_iterator(b.begin()), std::make_move_iterator(b.end()));
	b.clear();
}