#pragma once
#include "AST.h"
#include "Constants.h"
#include <map>
#include <vector>
#include <unordered_map>
using namespace std;

class BasicBlocks
{
public:
	BasicBlocks();
	BasicBlocks(shared_ptr<StmtAST> stmt);

	void Dump();
	void ClearVisitTag();

	bool try_concat_block(shared_ptr<BasicBlocks> other);

	void AddPrev(shared_ptr<BasicBlocks> _pre);
	void AddNext(shared_ptr<BasicBlocks> _nxt);
	static void LinkTwoBlock(shared_ptr<BasicBlocks>& pre, shared_ptr<BasicBlocks>& nxt);
	vector<shared_ptr<StmtAST>> stmts;
	vector<shared_ptr<BasicBlocks>> pre, nxt;
	bool is_visited;
	size_t id;

	static shared_ptr<BasicBlocks> break_block, continue_block;
	static size_t block_id;
	void simplify();

};

vector<shared_ptr<BasicBlocks>> ConstructRootBlocks(shared_ptr<CompUnitAST>& ast);

//����ֵ�����һ��basicblock
shared_ptr<BasicBlocks> ConstructBasicBlocks(shared_ptr<BlockAST>block_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block);
void ConstructBasicBlocks(shared_ptr<IfAST> if_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block);
void ConstructBasicBlocks(shared_ptr<WhileAST> while_ast, shared_ptr<BasicBlocks>& prev_block, shared_ptr<BasicBlocks>& nxt_block);
