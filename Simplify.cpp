#include "AST.h"
#include"Utility.h"
// 为什么要加一个_type参数，因为不然缩成的ValueAST结点的value_type不知道是什么，会导致后续的汇编代码出错 
void VarDefAST::Simplify(shared_ptr<SingleVarDef> targetAst, Value _value, ValueType _type) {
	targetAst->init_value = make_unique<ValueAST>(_value, _type);
}

void ExprAST::Simplify(ValueType rtype, Value _value) {
	canEvaluate = EvaluateType::can;
	if (op == OpType::PLUS) {
		if (rtype == ValueType::INT) value.int_val = _value.int_val;
		else value.float_val = _value.float_val;
		return;
	}
	else if (op == OpType::MINUS) {
		if (rtype == ValueType::INT) value.int_val = -(_value.int_val);
		else value.float_val = -(_value.float_val);
		return;
	}
	else {
		if (rtype == ValueType::INT) value.int_val = !(_value.int_val);
		else value.int_val = !(_value.float_val);
		return;
	}
}

void ExprAST::Simplify(Value _value, int flag, ValueType _type) {  // flag为0表示优化的是loperand
	if(flag == 0) loperand = make_unique<ValueAST>(_value, _type);
	else roperand = make_unique<ValueAST>(_value, _type);
}

void ReturnAST::Simplify(shared_ptr<ReturnAST> targetAst, Value _value, ValueType _type) {
	targetAst->ret_value = make_unique<ValueAST>(_value, _type);
}

void WhileAST::Simplify(shared_ptr<WhileAST> targetAst, Value _value, ValueType _type) {
	targetAst->cond = make_unique<ValueAST>(_value, _type);
}

void IfAST::Simplify(shared_ptr<IfAST>targetAst, Value _value, ValueType _type) {
	targetAst->cond = make_unique<ValueAST>(_value, _type);
}

void AssignmentAST::Simplify(shared_ptr<AssignmentAST> targetAst, Value _value, ValueType _type) {
	shared_ptr<SingleVarDef> targetSingleVar = dynamic_pointer_cast<SingleVarDef>(targetAst->lval_ref->var_def)->shared_from_base<SingleVarDef>();
	targetSingleVar->init_value = make_unique<ValueAST>(_value, _type);
}