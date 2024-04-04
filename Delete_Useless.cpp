#include"AST.h"
#include"Symbol_Table.h"

void CompUnitAST::Delete_Useless() {

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i])
        {
            int pre_size = nodes.size();
            auto VarDef = dynamic_pointer_cast<VarDefAST>(nodes[i]);
            if (VarDef) {
                VarDef->Delete_Useless(nodes, i);
            }
            else {
                nodes[i]->Delete_Useless();
            }
            if (nodes.size() == pre_size - 1)
                i--;
        }
    }
}

void FuncDefAST::Delete_Useless() {
    block->Delete_Useless();
}

void BlockAST::Delete_Useless() {
    if (stmts)
    {
        for (size_t i = 0; i < stmts->size(); ++i)
        {
            int pre_size = stmts->size();
            auto VarDef = dynamic_pointer_cast<VarDefAST>(stmts->at(i));
            if (VarDef) {
                VarDef->Delete_Useless(*stmts, i);
            }
            else {
                stmts->at(i)->Delete_Useless();
            }
            if (stmts->size() == pre_size - 1)
                i--;
        }
    }
}

void IfAST::Delete_Useless() {
    if (true_block) {
        true_block->Delete_Useless();
    }
    if (false_block) {
        false_block->Delete_Useless();
    }
}

void WhileAST::Delete_Useless() {
    if (block) {
        block->Delete_Useless();
    }
}

void VarDefAST::Delete_Useless(vector<shared_ptr<DefAST>>& nodes, int index) {
    for (size_t i = 0; i < var_defs.size(); ++i) {
        auto single = dynamic_pointer_cast<SingleVarDef>(var_defs[i]);
        if (single) {
            if (single->use_cnt == 0) {
                var_defs.erase(var_defs.begin() + i);
                i--;
            }
        }
    }
    if (var_defs.size() == 0)
        nodes.erase(nodes.begin() + index);
}

void VarDefAST::Delete_Useless(vector<shared_ptr<StmtAST>>& targetStmts, int index) {
    for (size_t i = 0; i < var_defs.size(); ++i) {
        auto single = dynamic_pointer_cast<SingleVarDef>(var_defs[i]);
        if (single) {
            if (single->use_cnt == 0) {
                var_defs.erase(var_defs.begin() + i);
                i--;
            }
        }
    }
    if (var_defs.size() == 0)
        targetStmts.erase(targetStmts.begin() + index);
}