#include"AST.h"
#include<math.h>

int in_while = 0; 
int unroll_sum = 0; int unroll_now = 0;
bool should_unroll = true;
void CompUnitAST::Unroll_Loop() {

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i])
        {
            nodes[i]->Unroll_Loop();
        }
    }
}

void FuncDefAST::Unroll_Loop() {
    block->Unroll_Loop();
}

void BlockAST::Unroll_Loop() {
    if (stmts)
    {
        for (size_t i = 0; i < stmts->size(); ++i)
        {
            auto while_stmt = dynamic_pointer_cast<WhileAST>(stmts->at(i));
            if (should_unroll && while_stmt) {
                in_while++;
                int flag = 0; int from_value = 0; string from_name = "";
                if (i != 0) {
                    auto VarDef = dynamic_pointer_cast<VarDefAST>(stmts->at(i - 1));
                    if (VarDef && VarDef->var_defs.size() == 1 && VarDef->var_defs[0]->IsSingle() 
                        && VarDef->type == ValueType::INT) {
                        auto single = dynamic_pointer_cast<SingleVarDef>(VarDef->var_defs[0]);
                        if (single->init_value) {
                            auto rs = single->init_value->Evaluate();
                            if (rs.first == EvaluateType::can) {
                                flag = 1;
                                from_value = rs.second.int_val; from_name = *single->name;
                            }
                        }
                    }
                }
                while_stmt->Unroll_Loop(*stmts, i, flag, from_value, from_name);
                in_while--;
                if (in_while == 0) {
                    unroll_sum += unroll_now;
                    unroll_now = 0;
                }
            }
            else {
                stmts->at(i)->Unroll_Loop();
            }
        }
    }
}

void IfAST::Unroll_Loop() {
    if (true_block) {
        true_block->Unroll_Loop();
    }
    if (false_block) {
        false_block->Unroll_Loop();
    }
}

void WhileAST::Unroll_Loop(vector<shared_ptr<StmtAST>>& targetStmts, int index, int flag, int from_value, string from_name) {
    if (block) {
        block->Unroll_Loop();
    }
    if (flag) {
        auto exp = dynamic_pointer_cast<ExprAST>(cond);
        if (exp && exp->loperand->GetName() == from_name && exp->op == OpType::LE && exp->roperand->Evaluate().first == EvaluateType::can) {
            int to_value = exp->roperand->Evaluate().second.int_val;
            auto m_block = dynamic_pointer_cast<BlockAST>(block);
            if (!m_block) error_sys("Something is wrong in (WhileAST::Unroll_Loop)");
            int sz = m_block->stmts->size();
            if ((sz == 2 || sz == 3) && m_block->stmts->at(sz - 1)->GetNodeType() == ASTNodeType::ASSIGN) {
                bool guard = false;
                for (int index = 0; index <= sz - 1; index++) {
                    if (m_block->stmts->at(index)->GetNodeType() == ASTNodeType::ASSIGN) {
                        auto assign = dynamic_pointer_cast<AssignmentAST>(m_block->stmts->at(index));
                        if (assign->lval_ref->var_def->GetName() == from_name && index < sz - 1) {
                            guard = false;
                            break;
                        }
                        if (assign->lval_ref->var_def->GetName() == from_name && index == sz - 1)
                            guard = true;
                    }
                }
                if (guard) {
                    auto assign = dynamic_pointer_cast<AssignmentAST>(m_block->stmts->at(sz - 1));
                    auto single = dynamic_pointer_cast<SingleVarDef>(assign->lval_ref->var_def);
                    auto exp = dynamic_pointer_cast<ExprAST>(single->init_value);
                    if (exp->loperand->GetName() == from_name && exp->roperand->Evaluate().first == EvaluateType::can) {
                        int offset = exp->roperand->Evaluate().second.int_val;
                        int times = ceil((to_value - from_value) / offset);
                        if (unroll_now == 0) unroll_now = times;
                        else unroll_now *= times;
                        if (unroll_now > 100) {
                            should_unroll = false;
                        }
                        // cout << unroll_now << "hahahaha" << endl;
                        if (should_unroll) {
                            shared_ptr<BlockAST>exchange = make_shared<BlockAST>();
                            for (int i = 1; i <= times; i++) {
                                for (int j = 0; j < sz; j++) {
                                    if (m_block->stmts->at(j)->GetNodeType() == ASTNodeType::FUNCCALL)
                                        FuncCallAST::funccall_cnt++;
                                    exchange->stmts->push_back(m_block->stmts->at(j));
                                }
                            }
                            targetStmts[index] = exchange;
                        }
                    }
                }
            }
        }
    }
}