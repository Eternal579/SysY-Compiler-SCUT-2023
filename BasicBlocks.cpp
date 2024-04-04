#include "BasicBlocks.h"

shared_ptr<BasicBlocks> BasicBlocks::break_block, BasicBlocks::continue_block;

size_t BasicBlocks::block_id = 0;

#define VISIT_ONCE if (is_visited) return; else is_visited = true;

BasicBlocks::BasicBlocks():is_visited(false), id(++block_id){}

BasicBlocks::BasicBlocks(shared_ptr<StmtAST> stmt):BasicBlocks()
{
	stmts.push_back(stmt);
}

void BasicBlocks::Dump()
{
	VISIT_ONCE;
	cout << "Block id: " << id << ", pre: ";
	for (auto& i : pre)
		cout << i->id << ", ";
	cout << ", nxt: ";
	for (auto& i : nxt)
		cout << i->id << ", ";
	cout << endl;
	for(auto&i :stmts)
		cout << ToString(i->GetNodeType()) << endl;
	cout << "------------------------\n";
	for (auto& i : nxt)
		i->Dump();
}

void BasicBlocks::ClearVisitTag()
{
	if (is_visited == 0)
		return;
	is_visited = 0;
	for (auto& i : nxt)
		i->ClearVisitTag();
}

bool BasicBlocks::try_concat_block(shared_ptr<BasicBlocks> other)
{
	if (other->stmts.empty())
		return false;
	if (other->stmts[0]->GetNodeType() == ASTNodeType::WHILE ||
		other->stmts[0]->GetNodeType() == ASTNodeType::IF)
		return false;

	ASTNodeType back_type = stmts.back()->GetNodeType();
	if (back_type == ASTNodeType::RETURN || back_type == ASTNodeType::BREAK || back_type == ASTNodeType::CONTINUE)
		return false;

	ConcatVector(stmts, other->stmts);
	ConcatVector(pre, other->pre);
	ConcatVector(nxt, other->nxt);
	return true;
}

void BasicBlocks::AddPrev(shared_ptr<BasicBlocks> _pre)
{
	pre.push_back(_pre);
}

void BasicBlocks::AddNext(shared_ptr<BasicBlocks> _nxt)
{
	nxt.push_back(_nxt);
}

void BasicBlocks::LinkTwoBlock(shared_ptr<BasicBlocks>& pre, shared_ptr<BasicBlocks>& nxt)
{
	pre->AddNext(nxt);
	nxt->AddPrev(pre);
}

void BasicBlocks::simplify() {
	VISIT_ONCE;
	// while (j < n) {
	// 		a[i][j] = 0;
	//      j = j + 1;
	// }
	// 
	// if (j < n) {
	//      memset(a[i][j], n - j);
	//	}
	// for (auto& p: prev) {

	// }
	

	// 查找数组维度最后一维
	// 观察数组维度最后一维是否只加1


	// mp 
	// y.first -> 数组赋值的位置 y.second -> +1的位置
	// 
	// 
	
	// for (auto& [x, y]: mp) {
	// 	if (y.second > 0) {
	// 		shared_ptr<AssignmentAST> assign = dynamic_pointer_cast<AssignmentAST>(stmts[y.first]);
	// 		auto var = dynamic_pointer_cast<ArrayVarDef>(assign->lval_ref->var_def);


	// 		{
	// 			auto temp_params = new vector<shared_ptr<DeclAST>>;
	// 			auto param = make_shared<ValueAST>(make_shared<ArrayVarDef>(make_unique<string>(var->GetName())));
	// 			param->value_type = ltype;
	// 			temp_params->push_back(param);

	// 			// param = make_shared<ValueAST>(int(var->init_values.size()));
	// 			// temp_params->push_back(param);
	// 			if (ltype == ValueType::INT)
	// 			{
	// 				auto memset_call = FuncCallAST(make_unique<string>("scut_memset_int"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
	// 				memset_call.func_def = FuncDefAST::memset_int;
	// 				RestoreRegister(memset_call.GenerateRSICV());
	// 			}
	// 			else
	// 			{
	// 				auto memset_call = FuncCallAST(make_unique<string>("scut_memset_float"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
	// 				memset_call.func_def = FuncDefAST::memset_float;
	// 				RestoreRegister(memset_call.GenerateRSICV());
	// 			}
	// 		}
	// 	}
	// }
	// if (prev.size() == 1 && prev->stmts.size() == 1) {
	// 	shared_ptr<WhileAST> whileStmt = prev->stmts[0];
	// 	auto cond = whileStmt->cond;
	// 	BlockAST blockAST(stmts);
	// 	shared_ptr<IfAST> ifStmt = make_shared<IfAST>(cond, );	

	// } else {
	// 	return;
	// }

	// for (auto& i: nxt) {
	// 	i->simplify();
	// }
	// for (int i = 0; i < stmts.size(); ++i) {
	// 	switch(stmts[i]->GetNodeType()) {
	// 		case ASTNodeType::ASSIGN: {
	// 			shared_ptr<AssignmentAST> assign = dynamic_pointer_cast<AssignmentAST>(stmts[i]);
	// 			if (i == 0 && assign->lval_ref->var_def->IsSingle()) {
	// 				shared_ptr<DeclAST> val = dynamic_pointer_cast<SingleVarDef>(assign->lval_ref->var_def)->init_value;
	// 				// values[assign->lval_ref->var_def->GetName()] = {i, *(dynamic_pointer_cast<SingleVarDef>(assign->lval_ref->var_def)->init_value), true};
	// 			}
	// 		}
	// 			break;
	// 		case ASTNodeType::BLOCK:
	// 			break;
	// 		case ASTNodeType::IF:
	// 			break;
	// 			break;
	// 		case ASTNodeType::CONTINUE: 
	// 			break;
	// 		case ASTNodeType::BREAK:
	// 			break;
	// 		case ASTNodeType::RETURN:
	// 			break;
	// 		case ASTNodeType::FUNCCALL: 
	// 			break;
	// 		case ASTNodeType::GLOBAL_VARDEF: {

	// 		}
	// 			break;
	// 		case ASTNodeType::FUNCDEF:
	// 			break;
	// 	}
	// }
}

vector<shared_ptr<BasicBlocks>> ConstructRootBlocks(shared_ptr<CompUnitAST>& ast)
{
	vector<shared_ptr<BasicBlocks>> result_blocks;
	for(auto& node: ast->nodes)
	{
		if (node->GetNodeType() == ASTNodeType::FUNCDEF)
		{
			auto ptr = dynamic_pointer_cast<FuncDefAST>(node);
			shared_ptr<BasicBlocks> pre_block_tmp = make_shared<BasicBlocks>();
			shared_ptr<BasicBlocks> func_end = make_shared<BasicBlocks>();
			ConstructBasicBlocks(ptr->block, pre_block_tmp, func_end);
			result_blocks.push_back(pre_block_tmp);
		}
		else if (node->GetNodeType() == ASTNodeType::VARDEF)
		{
			//auto ptr = dynamic_pointer_cast<VarDefAST>(node);
			//result_blocks.push_back(make_shared<GlobalBasicBlocks>(ptr));
		}
	}
	return result_blocks;
}

shared_ptr<BasicBlocks> ConstructBasicBlocks(shared_ptr<BlockAST> block_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block)
{
	shared_ptr<BasicBlocks> pre_block_tmp = prev_block;
	shared_ptr<BasicBlocks> nxt_block_temp = make_shared<BasicBlocks>(); // ���ڴ洢��һ��block��nxt��preָ��
	for (int i = 0; i < block_ast->stmts->size(); ++i)
	{
		auto& stmt = block_ast->stmts->at(i);

		ASTNodeType stmt_type = stmt->GetNodeType();

		if (stmt_type == ASTNodeType::BLOCK)
		{
			pre_block_tmp = ConstructBasicBlocks(dynamic_pointer_cast<BlockAST>(stmt), pre_block_tmp, nxt_block_temp);
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else if (stmt_type == ASTNodeType::IF)
		{
			ConstructBasicBlocks(dynamic_pointer_cast<IfAST>(stmt), pre_block_tmp, nxt_block_temp);
			pre_block_tmp = nxt_block_temp;
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else if (stmt_type == ASTNodeType::WHILE)
		{
			ConstructBasicBlocks(dynamic_pointer_cast<WhileAST>(stmt), pre_block_tmp, nxt_block_temp);
			pre_block_tmp = nxt_block_temp;
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else if (stmt_type == ASTNodeType::BREAK) // end of stmts
		{
			pre_block_tmp->stmts.push_back(stmt);
			BasicBlocks::LinkTwoBlock(pre_block_tmp, BasicBlocks::break_block);

			pre_block_tmp = nxt_block_temp;
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else if (stmt_type == ASTNodeType::CONTINUE) // end of stmts
		{
			pre_block_tmp->stmts.push_back(stmt);
			BasicBlocks::LinkTwoBlock(pre_block_tmp, BasicBlocks::continue_block);

			pre_block_tmp = nxt_block_temp;
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else if(stmt_type == ASTNodeType::RETURN)
		{
			pre_block_tmp->stmts.push_back(stmt);
			pre_block_tmp->nxt.clear();

			pre_block_tmp = nxt_block_temp;
			nxt_block_temp = make_shared<BasicBlocks>();
		}
		else
		{
			pre_block_tmp->stmts.push_back(stmt);
		}
	}
	return pre_block_tmp;
}

void ConstructBasicBlocks(shared_ptr<IfAST> if_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block)
{
	prev_block->stmts.push_back(if_ast);

	shared_ptr<BasicBlocks> prev_block_tmp = make_shared<BasicBlocks>();
	BasicBlocks::LinkTwoBlock(prev_block, prev_block_tmp);
	prev_block_tmp = ConstructBasicBlocks(dynamic_pointer_cast<BlockAST>(if_ast->true_block), prev_block_tmp, nxt_block);
	if(prev_block_tmp->pre.empty()) // ��β����continue, break
		BasicBlocks::LinkTwoBlock(prev_block_tmp, nxt_block);

	if(if_ast->false_block)
	{
		shared_ptr<BasicBlocks> prev_block_tmp = make_shared<BasicBlocks>();
		BasicBlocks::LinkTwoBlock(prev_block, prev_block_tmp);
		prev_block_tmp = ConstructBasicBlocks(dynamic_pointer_cast<BlockAST>(if_ast->false_block), prev_block_tmp, nxt_block);
		BasicBlocks::LinkTwoBlock(prev_block_tmp, nxt_block);
	}
	else
	{
		BasicBlocks::LinkTwoBlock(prev_block, nxt_block);
	}
}

void ConstructBasicBlocks(shared_ptr<WhileAST> while_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block)
{
	shared_ptr<BasicBlocks> pre_break = BasicBlocks::break_block, pre_continue = BasicBlocks::continue_block;

	shared_ptr<BasicBlocks> while_head = make_shared<BasicBlocks>();
	while_head->stmts.push_back(while_ast);
	BasicBlocks::LinkTwoBlock(prev_block, while_head);
	BasicBlocks::LinkTwoBlock(while_head, nxt_block);

	BasicBlocks::continue_block = while_head;
	BasicBlocks::break_block = nxt_block;

	auto block = dynamic_pointer_cast<BlockAST>(while_ast->block);
	shared_ptr<BasicBlocks> prev_block_tmp = make_shared<BasicBlocks>();
	BasicBlocks::LinkTwoBlock(while_head, prev_block_tmp);
	prev_block_tmp = ConstructBasicBlocks(block, prev_block_tmp, nxt_block);
	if (!block->stmts->empty() && (block->stmts->back()->GetNodeType() == ASTNodeType::BREAK || block->stmts->back()->GetNodeType() == ASTNodeType::CONTINUE))
	{
		// do nothing
	}
	else
	{
		BasicBlocks::LinkTwoBlock(prev_block_tmp, while_head);
	}

	BasicBlocks::continue_block = pre_continue;
	BasicBlocks::break_block = pre_break;
}
