#include "ASMGenerate.h"

BasicBlock::BasicBlock(shared_ptr<BasicBlock> _pre)
{
	pres.push_back(_pre);
}

BasicBlock::~BasicBlock()
{
}

void BasicBlock::GenerateASM()
{
}

CompUnitInstruction::CompUnitInstruction(shared_ptr<CompUnitAST> ast)
{

}

void CompUnitInstruction::GenerateASM()
{
	for (auto& i : instructions)
		i->GenerateASM();
}

void CompUnitInstruction::GenerateGlobalVarASM()
{
}

Instruction::Instruction(shared_ptr<BasicBlock> _pre): BasicBlock(_pre)
{
}

FuncDefInstruction::FuncDefInstruction(shared_ptr<BasicBlock> _pre, shared_ptr<FuncDefAST> _func_def) : Instruction(_pre), func_def(_func_def)
{

}

VarDefInstruction::VarDefInstruction(shared_ptr<BasicBlock> _pre, shared_ptr<VarDefAST> _var_def) : Instruction(_pre), var_def(_var_def)
{

}

void VarDefInstruction::GenerateASM()
{

}
