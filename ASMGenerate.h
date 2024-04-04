#pragma once
#include <vector>
#include <memory>

#include "ASMGenerate.h"
#include "ASMGenerate.h"
#include "AST.h"
#include "Constants.h"
using namespace std;

#define PRE shared_ptr<BasicBlock> _pre

extern int cnt;

class Instruction;

class BasicBlock: public enable_shared_from_this<BasicBlock>
{
public:
	BasicBlock(shared_ptr<BasicBlock> _pre = nullptr);

	virtual ~BasicBlock();

	template <typename T>
	shared_ptr<T> derived_shared_from_this()
	{
		return static_pointer_cast<T>(shared_from_this());
	}

	virtual void GenerateASM();

	// predecessor of this block
	vector<shared_ptr<BasicBlock>> pres;

	// successor of this block
	shared_ptr<BasicBlock> nxt;

	vector<shared_ptr<Instruction>> instructions;

};

class CompUnitInstruction : public BasicBlock
{
public:
	CompUnitInstruction(shared_ptr<CompUnitAST> ast);

	void GenerateASM() override;

	void GenerateGlobalVarASM();
};

class Instruction : public BasicBlock
{
public:
	Instruction(shared_ptr<BasicBlock> _pre);
};

class FuncDefInstruction : public Instruction
{
public:
	FuncDefInstruction(shared_ptr<BasicBlock> _pre, shared_ptr<FuncDefAST> _func_def);

	shared_ptr<FuncDefAST> func_def;
};

class VarDefInstruction : public Instruction
{
public:
	VarDefInstruction(shared_ptr<BasicBlock> _pre, shared_ptr<VarDefAST> _var_def);

	void GenerateASM() override;

	shared_ptr<VarDefAST> var_def;
	std::string name;
	ValueType type;
	bool is_const;
};