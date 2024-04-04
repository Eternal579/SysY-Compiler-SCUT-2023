#include "AST.h"
#include "Symbol_Table.h"
#include "Utility.h"
#include <functional>

#include "LiteralPool.h"

/* ****************************************
Analyze实现了符号表的插入，语义分析，部分优化工作
**************************************** */

bool IfAST::is_If = false;

bool WhileAST::is_While = false;

bool WhileAST::is_While_cond = false;

vector<pair<string, Value>>if_else_changed; // lrx写的，只有我自己清楚他的用途

vector<string>while_changed; // 把while语句中外部定义的变量，并且在内部中有所修改的变量统计下来

void CompUnitAST::Analyze() {
    Symbol_Table::NewScope();

    Symbol_Table::InsertPredefinedFunction();

    for (const shared_ptr<DefAST>& node : nodes)
    {
        if (node)
        {
            node->Analyze();
        }
    }
}

void VarDefAST::Analyze()
{
    bool IsGlobal = is_global; bool IsConst = is_const;
    for (size_t i = 0; i < var_defs.size(); ++i)
    {
        if(!Symbol_Table::Insert_Use(var_defs[i]->shared_from_base<VarDef>()))
            error_sys("Error in Insert_Use()");

        if (var_defs[i]->IsSingle())
        {
            auto singleVar = dynamic_pointer_cast<SingleVarDef>(var_defs[i]);

            if (!singleVar->init_value) {
                if (!Symbol_Table::Insert(*singleVar, this->type, IsGlobal, IsConst)) // 因为我把插入符号表的操作放到后面了，所以在这里写的目的是为了考虑只声明不初始化的情况
                    error_sys("redefinition of variable (VarDefAST::Analyze() )");
                continue;
            }

            singleVar->init_value->Analyze(); // 对于一个变量，它Analyze之后可能会变成canEvaluate

            bool flag = false;
            if (singleVar->init_value->GetType() == ValueType::IDENT) { // flag是用来判断初始值是不是个常量，如果是变量才需要检查const类型
                flag = true;
            }
            // 不是标识符的话GetType就可以直接说明他是整型还是浮点型，但如果是标识符的话则需要查表
            ValueType TMP_Type = singleVar->init_value->GetType() == ValueType::IDENT ?
                Symbol_Table::GetType(singleVar->init_value->GetName()) : singleVar->init_value->GetType();
            if (type == ValueType::INT && TMP_Type != ValueType::INT && TMP_Type != ValueType::FLOAT) {
                error_sys("Type checking fail (int <- wrongType)");
            }
            else if (type == ValueType::FLOAT && TMP_Type != ValueType::FLOAT && TMP_Type != ValueType::INT)
                error_sys("Type checking fail (float <- wrongType)");

            if (IsConst && flag && Symbol_Table::GetIsConst(singleVar->init_value->GetName()) == false) {
                error_sys("Type checking fail (const <- notConst)");
            }

            auto res = singleVar->init_value->Evaluate(); // InitVal部分Analyze完之后Evaluate部分才有效，比如对于一个funccall，我必须在他在查完funcDef本身的代码是否可算，以及实参是否可算之后才可以确定这个funccall是否可算
            if (res.first == EvaluateType::canNot) {  // 如果Initval部分不可计算，那不需要向符号表中插入此变量的值
                if (!Symbol_Table::Insert(*singleVar, this->type, IsGlobal, IsConst))
                    error_sys("redefinition of variable (VarDefAST::Analyze() )");
                Origin rs = singleVar->init_value->addToMul();
                if (rs.cnt != -1 && rs.cnt != 1) {
                    if (rs.dim.size() == 0) {
                        singleVar->init_value = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(rs.cnt),
                            make_shared<ValueAST>(make_unique<SingleVarDef>(make_unique<string>(rs.name)), rs.type));
                    }
                    else {
                        vector<shared_ptr<DeclAST>>dimension;
                        for (int index = 0; index < rs.dim.size(); index++) {
                            dimension.push_back(make_shared<ValueAST>(rs.dim[index]));
                        }
                        singleVar->init_value = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(rs.cnt),
                            make_shared<ValueAST>(make_unique<ArrayVarDef>((make_unique<string>(rs.name)), dimension), rs.type));
                    }
                }
            }
            else { // 可以计算                                                 // 因为如果是float类型的赋值给了int类型
                if (type == ValueType::INT && TMP_Type == ValueType::FLOAT) { // 我必须修改res，如果把不修改的res存到符号表
                    res.second.int_val = (int)res.second.float_val;           // 那就会出现一个问题，在生成汇编的时候，当我要显式拿一个int类型的变量的时候                            
                }                                                             // 但实际上ValueAST的union类型的value是个float，就会出现错误
                else if (type == ValueType::FLOAT && TMP_Type == ValueType::INT) {
                    res.second.float_val = (float)res.second.int_val;
                }

                if (!Symbol_Table::Insert(*singleVar, this->type, res.second, IsGlobal, IsConst))
                    error_sys("redefinition of variable (VarDefAST::Analyze() )");
                VarDefAST::Simplify(singleVar->shared_from_base<SingleVarDef>(), res.second, type); // 优化
            }
        }
        else
        {           
            // 数组
            auto arrayVar = dynamic_pointer_cast<ArrayVarDef>(var_defs[i]);
            ValueType TYPE = type;
            for (int i = 0; i < arrayVar->dimension.size(); i++) {
                arrayVar->dimension[i]->Analyze();
                auto rs = arrayVar->dimension[i]->Evaluate(); 
                ValueType TMP_Type = arrayVar->dimension[i]->GetType() == ValueType::IDENT ?
                    Symbol_Table::GetType(arrayVar->dimension[i]->GetName()) : arrayVar->dimension[i]->GetType();
                
                if (arrayVar->dimension[i]->GetType() == ValueType::IDENT && Symbol_Table::GetIsConst(arrayVar->dimension[i]->GetName()) == false) {
                    error_sys("array's dimension must be initialized with const value(VarDefAST::Analyze()) ");
                }
                if (rs.first == EvaluateType::can) {
                    arrayVar->dimension[i] = make_shared<ValueAST>(rs.second, TMP_Type);
                }
            }
            const auto& var_def_ptr = arrayVar;
            auto& dimension = var_def_ptr->dimension;
            int n = dimension.size();
            vector<int> dimensions(n);
            // cout << "dimension: " << n << endl;
            for (int j = 0; j < n; ++j) {
                if (j) {
                    auto rs = dimension.at(j)->Evaluate();        
                    ValueType TMP_Type = dimension.at(j)->GetType() == ValueType::IDENT ?
                        Symbol_Table::GetType(dimension.at(j)->GetName()) : dimension.at(j)->GetType();
                    ValueAST* valueAST = dynamic_cast<ValueAST*>((dimension)[n - 1 - j].get());
                    int tmp = 0;
                    if (TMP_Type == ValueType::FLOAT) {
                        // TODO
                        error_sys("dimension could not be float");
                        // tmp = (int)(valueAST->value.float_val);
                        // cout << valueAST->value.float_val << endl;
                    }
                    else if (TMP_Type == ValueType::INT) {
                        tmp = valueAST->value.int_val;
                        // cout << valueAST->value.int_val << endl;
                    }
                    else {
                        cout << "error type error" << endl;
                        exit(-1);
                    }
                    dimensions[j] = dimensions[j - 1] * tmp;
                    if (tmp < 0) {
                        error_sys("dimension should be positive");
                    }
                    if (rs.first == EvaluateType::can) {}
                    else {
                        error_sys("dimension can not be calculated");
                    }
                }
                else {
                    auto rs = dimension.at(j)->Evaluate();        
                    ValueType TMP_Type = dimension.at(j)->GetType() == ValueType::IDENT ?
                        Symbol_Table::GetType(dimension.at(j)->GetName()) : dimension.at(j)->GetType();
                    ValueAST* valueAST = dynamic_cast<ValueAST*>((dimension)[n - 1].get());
                    int tmp = 0;
                    if (TMP_Type == ValueType::FLOAT) {
                        error_sys("dimension could not be float");
                        // TODO
                        // tmp = (int)(valueAST->value.float_val);
                        // cout << valueAST->value.float_val << endl;
                    }
                    else if (TMP_Type == ValueType::INT) {
                        tmp = valueAST->value.int_val;
                        // cout << valueAST->value.int_val << endl;
                    }
                    else {
                        cout << "error type error" << endl;
                        exit(-1);
                    }
                    dimensions[0] = tmp;
                    if (tmp < 0) {
                        error_sys("dimension should be positive");
                    }
                    if (rs.first == EvaluateType::can) {}
                    else {
                        error_sys("dimension can not be calculated");
                    }
                }
            }


            // for (int i = 0; i < n; ++i) {
            //     cout << dimensions[i] << endl;
            // }
            auto& list = var_def_ptr->list;

            /*
            if (list) {
                preorder(list, 0);
                int szz = var_def_ptr->init_values.size();
                for (int u = 0; u < dimensions.back() - szz; ++u) {
                    shared_ptr<ValueAST> valueAST_ = make_shared<ValueAST>(type);
                    var_def_ptr->init_values.push_back(valueAST_);
                }
            }
            else {
                // TODO: 全局变量需要默认初始化
                for (int u = 0; u < dimensions.back(); ++u) {
                    shared_ptr<ValueAST> valueAST_ = make_shared<ValueAST>(type);
                    var_def_ptr->init_values.push_back(valueAST_);
                }
            }
            */

            //std::function<void(shared_ptr<ListAST>, int)> preorder = [&TYPE, &preorder, &last_bracket, &prev_bracket, &func, &counter, &var_def_ptr, &n, &dimensions](std::shared_ptr<ListAST> root, int bracketNumber) -> void {
            
            
            int last_bracket = 0;
            int prev_bracket = 0;
            // int prev_id = 0;
            int _prev = 0;
            int id = 0;
            int counter = 0;
            int max_counter = 0;
            std::function<void(const int&,const int&,const int&,shared_ptr<DeclAST>, bool)> func = [&TYPE, &dimensions, &id, &var_def_ptr, &n, &counter, &_prev, &max_counter](const int& last, const int& prev, const int& curr, shared_ptr<DeclAST> val, bool flag) -> void {
                // cout << boolalpha << flag << endl;
                /* TODO: 数组为空 */
                if (curr > n + 1) {
                    cout << "\n数组非法操作\n";
                    exit(-1);
                }
                if (last == 0) {
                    counter = 1;
                    _prev = curr;
                    if (_prev != 1) {
                        if (curr == n + 1) max_counter = 1;
                        else max_counter = dimensions[n - curr];
                    }
                    var_def_ptr->init_values.push_back(val);
                    return;
                }
                if (curr == last && curr == prev) {
                    int sz = var_def_ptr->init_values.size();
                    while (id < n && dimensions[id] <= sz) {
                        ++id;
                    }
                    if (id >= n) {
                        error_sys("Array overflow: the initial size of the array should be lower than the capacity");
                    }
                    ++counter;
                    if (_prev != 1 && counter > max_counter) {
                        error_sys("Array overflow: counter error");
                    }
                    var_def_ptr->init_values.push_back(val);
                } else {
                    int sz = var_def_ptr->init_values.size();
                    if (_prev != 1 && curr <= last) {
                        // resize();
                        for (int u = 0; u < max_counter - counter; ++u) {
                            shared_ptr<ValueAST> valueAST = make_shared<ValueAST>(TYPE);
                            var_def_ptr->init_values.push_back(valueAST);
                        }
                    }
                    sz = var_def_ptr->init_values.size();
                    while (id < n && dimensions[id] <= sz) {
                        ++id;
                    }
                    //if (n != 1 && (id >= n || sz % dimensions[0])) {
                    //    error_sys("Array overflow or Array not filled");
                    //}
                    _prev = curr;
                    counter = 1;
                    // 设置max_counter TODO
                    if (curr == n + 1) max_counter = 1;
                    else max_counter = min(dimensions[n - curr], dimensions[id - 1]);
                    var_def_ptr->init_values.push_back(val);
                }
            };
            std::function<void(shared_ptr<ListAST>, int)> preorder = [&TYPE, &preorder, &last_bracket, &prev_bracket, &func, &counter, &var_def_ptr, &n, &dimensions](std::shared_ptr<ListAST> root, int bracketNumber) -> void {
                if (root->isBracket) {
                    // cout << " {";
                }
                int curr_bracket = bracketNumber;
                if (root->values) {
                    if (root->isBracket) {
                        ++curr_bracket;
                        func(last_bracket, prev_bracket, curr_bracket, root->values, root->isFinished);
                        // cout << " " << dynamic_cast<ValueAST*>(root->values.get())->value.int_val << "<last bracket: " << last_bracket << ", prev bracket:" << prev_bracket << ", bracket:" << bracketNumber + 1 << "> ";
                        last_bracket = curr_bracket;
                        prev_bracket = last_bracket;
                    }
                    else {
                        func(last_bracket, prev_bracket, curr_bracket, root->values, root->isFinished);
                        // cout << " " << dynamic_cast<ValueAST*>(root->values.get())->value.int_val << "<last bracket: " << last_bracket << ", prev bracket:" << prev_bracket << ", bracket:" << bracketNumber << "> ";
                        last_bracket = curr_bracket;
                        prev_bracket = last_bracket;
                    }
                } 
                const auto& children = root->children;
                for (int i = 0; i < children.size(); ++i) {
                    if (children[i]) {
                        if (root->isBracket) preorder(children[i], bracketNumber + 1); 
                        else preorder(children[i], bracketNumber);
                    }
                }
                if (root->isBracket && children.empty()) {
                    // cout << " 0 " << "<last bracket: " << last_bracket << ", prev bracket:" << prev_bracket << ", bracket:" << bracketNumber + 1 << "> ";
                    // cout << " 0 ";
                    ++curr_bracket;
                    shared_ptr<ValueAST> valueAST_ = make_shared<ValueAST>(TYPE);
                    func(last_bracket, prev_bracket, curr_bracket, valueAST_, root->isFinished);
                    last_bracket = curr_bracket;
                    prev_bracket = last_bracket;
                }
                if (root->isBracket) {
                    // if (counter < dimensions[n - curr_bracket]) {
                    //     int sz = var_def_ptr->init_values.size();
                    //     var_def_ptr->init_values.resize(sz + dimensions[n - curr_bracket] - counter);
                    // }
                    --prev_bracket;
                    // cout << "} ";
                }
            };
            if (list) {
                preorder(list, 0);
                int szz = var_def_ptr->init_values.size();
                //for (int u = 0; u < dimensions.back() - szz; ++u) {
                //    shared_ptr<ValueAST> valueAST_ = make_shared<ValueAST>(type);
                //    var_def_ptr->init_values.push_back(valueAST_);
                //}
                if (IsGlobal)
                {
                    var_def_ptr->init_values.resize(szz);
                    var_def_ptr->zero_cnt = dimensions.back() - szz;
                } else
                {
                    var_def_ptr->init_values.resize(dimensions.back());
                    var_def_ptr->zero_cnt = dimensions.back() - szz;
                }

            }
            else {
                // TODO: 全局变量需要默认初始化
                if (IsGlobal) {
                    var_def_ptr->is_def = true;
                    var_def_ptr->zero_cnt = dimensions.back();
                }
            	else
            	{
                    var_def_ptr->init_values.resize(dimensions.back());
                    var_def_ptr->zero_cnt = dimensions.back();
                }
                //for (int u = 0; u < dimensions.back(); ++u) {
                //    shared_ptr<ValueAST> valueAST_ = make_shared<ValueAST>(type);
                //    var_def_ptr->init_values.push_back(valueAST_);
                //}

            }
            // arrayVar->sz = (int)dimensions.back();
            if (!Symbol_Table::Insert(*arrayVar, this->type, IsGlobal, IsConst)) // 因为我把插入符号表的操作放到后面了，所以在这里写的目的是为了考虑只声明不初始化的情况
                error_sys("redefinition of variable (VarDefAST::Analyze() )");
            // insert dimension
            // initial_values的构造
            for (int i = 0; i < arrayVar->init_values.size(); i++) {
                if (arrayVar->init_values[i]) {
                    arrayVar->init_values[i]->Analyze();
                    bool flag = false;
                    if (arrayVar->init_values[i]->GetType() == ValueType::IDENT) { // flag是用来判断初始值是不是个常量，如果是变量才需要检查const类型
                        flag = true;
                    }
                    // 不是标识符的话GetType就可以直接说明他是整型还是浮点型，但如果是标识符的话则需要查表
                    ValueType TMP_Type = arrayVar->init_values[i]->GetType() == ValueType::IDENT ?
                        Symbol_Table::GetType(arrayVar->init_values[i]->GetName()) : arrayVar->init_values[i]->GetType();
                    if (type == ValueType::INT && TMP_Type != ValueType::INT && TMP_Type != ValueType::FLOAT) {
                        error_sys("Type checking fail (int <- wrongType)");
                    }
                    else if (type == ValueType::FLOAT && TMP_Type != ValueType::FLOAT && TMP_Type != ValueType::INT)
                        error_sys("Type checking fail (float <- wrongType)");
                    if (IsConst && flag && Symbol_Table::GetIsConst(arrayVar->init_values[i]->GetName()) == false) {
                        error_sys("Type checking fail (const <- notConst)");
                    }
                    auto rs = arrayVar->init_values[i]->Evaluate();

                    auto lval = dynamic_pointer_cast<ValueAST>(arrayVar->init_values[i]);
                    if (!lval || lval->var_def && lval->var_def->IsSingle()) { // 数组初始化列表中，只有可以计算的变量，或者表达式，甚至函数调用，可以进行简化
                        if (rs.first == EvaluateType::can) {
                            if (type == ValueType::INT && TMP_Type == ValueType::FLOAT) { // 我必须修改res，如果把不修改的res存到符号表
                                rs.second.int_val = (int)rs.second.float_val;           // 那就会出现一个问题，在生成汇编的时候，当我要显式拿一个int类型的变量的时候                            
                            }                                                             // 但实际上ValueAST的union类型的value是个float，就会出现错误
                            else if (type == ValueType::FLOAT && TMP_Type == ValueType::INT) {
                                rs.second.float_val = (float)rs.second.int_val;
                            }
                            arrayVar->init_values[i] = make_shared<ValueAST>(rs.second, type);
                        }
                        // else {
                        //     error_sys("Array can not be initialized with a num that can not be calculated(VarDefAST::Analyze() )");
                        // }
                    }
                }
            }
            vector<int> dim(n);
            for (int j = 0; j < n; ++j) {
                auto rs = dimension.at(j)->Evaluate();        
                ValueType _TMP_Type = dimension.at(j)->GetType();
                int tmp_;
                if (rs.first == EvaluateType::can) {
                    if (_TMP_Type == ValueType::INT) {
                        tmp_ = rs.second.int_val;
                    }
                    else {
                        tmp_ = rs.second.float_val;
                    }
                    dim[j] = tmp_;
                }
                else {
                    error_sys("dimension can not be calculated");
                }
            }
            Symbol_Table::ChangeDimension(arrayVar->GetName(), dim);
            // Symbol_Table::Insert(*arrayVar, TYPE, (int)dimensions.back(), IsGlobal, IsConst);
            // cout << "dimension.back()" << (int)dimensions.back() << endl;
        }
    }
}

void AssignmentAST::Analyze()
{
    Symbol_Table::AddUse(lval_ref->var_def->GetName());

    if (lval_ref->var_def->IsSingle()){
        auto singleVar = dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def);
        singleVar->init_value->Analyze();
        if (!Symbol_Table::GetExist(*singleVar->name))
            error_sys("Lval not declared (AssignmentAST::Analyze() )");
        if (Symbol_Table::GetSize(*singleVar->name) != 0) {
            error_sys("type error [] should not be single var");
        }
        ValueType ltype = Symbol_Table::GetType(*singleVar->name); lval_type = ltype;
        ValueType rtype = singleVar->init_value->GetType() == ValueType::IDENT ?
            Symbol_Table::GetType(singleVar->init_value->GetName()) : singleVar->init_value->GetType();
        if (ltype == ValueType::INT && rtype == ValueType::INT && rtype == ValueType::FLOAT) {
            error_sys("Assign fault(AssignmentAST::Analyze() )");
        }
        else if (ltype == ValueType::FLOAT && rtype == ValueType::INT && rtype == ValueType::FLOAT) {
            error_sys("Assign fault(AssignmentAST::Analyze() )");
        }

        auto rs = singleVar->init_value->Evaluate();
        if (rs.first == EvaluateType::can) {                            // 因为如果是float类型的赋值给了int类型
            if (ltype == ValueType::INT && rtype == ValueType::FLOAT) { // 我必须修改res，如果把不修改的res存到符号表
                rs.second.int_val = (int)rs.second.float_val;           // 那就会出现一个问题，在生成汇编的时候，当我要显式拿一个int类型的变量的时候
            }                                                           // 但实际上ValueAST的union类型的value是个float，就会出现错误
            else if (ltype == ValueType::FLOAT && rtype == ValueType::INT) {
                rs.second.float_val = (float)rs.second.int_val;
            }

            if (IfAST::is_If && !Symbol_Table::isInCurrentScope(*singleVar->name)) {
                auto info = Symbol_Table::GetInfo(*singleVar->name);
                // cout << "just " << *singleVar->name << info.Info_value.int_val << endl;
                if_else_changed.push_back({ *singleVar->name,info.Info_value });
                Simplify(shared_from_base<AssignmentAST>(), rs.second, ltype);
                Symbol_Table::ChangeVal(*singleVar->name, rs.second);
            }
            else{
                Simplify(shared_from_base<AssignmentAST>(), rs.second, ltype);
                Symbol_Table::ChangeVal(*singleVar->name, rs.second); // 当右值编译期可计算，则修改在符号表中的值
            }
        }
        else {
            Symbol_Table::ChangeEvaluate(*singleVar->name); // 当右值编译期不可计算，则把其在符号表中的canEvaluate置为canNot
        }
    }
    else {
        // TODO: 数组维度检查 TODO
        auto arrayVar = dynamic_pointer_cast<ArrayVarDef>(lval_ref->var_def);
        
        for (int i = 0; i < arrayVar->dimension.size(); i++) {
            arrayVar->dimension[i]->Analyze();
            ValueType dim_type = arrayVar->dimension[i]->GetType() == ValueType::IDENT ?
                Symbol_Table::GetType(arrayVar->dimension[i]->GetName()) : arrayVar->dimension[i]->GetType();
            auto rs = arrayVar->dimension[i]->Evaluate();
            if (rs.first == EvaluateType::can) {
                arrayVar->dimension[i] = make_shared<ValueAST>(rs.second, dim_type);
            }
        }

        arrayVar->init_value->Analyze();
        if (!Symbol_Table::GetExist(*arrayVar->name))
            error_sys("Lval not declared (AssignmentAST::Analyze() )");
        ValueType ltype = Symbol_Table::GetType(*arrayVar->name); lval_type = ltype;
        ValueType rtype = arrayVar->init_value->GetType() == ValueType::IDENT ?
            Symbol_Table::GetType(arrayVar->init_value->GetName()) : arrayVar->init_value->GetType();
        if (ltype == ValueType::INT && rtype == ValueType::INT && rtype == ValueType::FLOAT) {
            error_sys("Assign fault(AssignmentAST::Analyze() )");
        }
        else if (ltype == ValueType::FLOAT && rtype == ValueType::INT && rtype == ValueType::FLOAT) {
            error_sys("Assign fault(AssignmentAST::Analyze() )");
        }
        // 
        if (arrayVar->dimension.size() != Symbol_Table::GetSize(*arrayVar->name)) {
            error_sys("type error");
        }
    }
}

int trick = 0;
void ValueAST::Analyze()
{
    if (GetType() == ValueType::IDENT) {
        value_type = Symbol_Table::GetType(GetName());
        if (var_def) { 
            Symbol_Table::AddUse(GetName());
        }
        auto rs = Symbol_Table::GetInfo(GetName());
        if (rs.canEvaluate != EvaluateType::canNot) { // 符号表中是根据canEvaluate是否为空来判断是否可编译求值
            canEvaluate = EvaluateType::can;
            value = rs.Info_value;
        }
        if (!var_def->IsSingle()) {
            auto array = dynamic_pointer_cast<ArrayVarDef>(var_def);
            for (int i = 0; i < array->dimension.size(); i++) {
                array->dimension[i]->Analyze();
                ValueType dim_type = array->dimension[i]->GetType() == ValueType::IDENT ?
                    Symbol_Table::GetType(array->dimension[i]->GetName()) : array->dimension[i]->GetType();
                auto rs = array->dimension[i]->Evaluate();
                if (rs.first == EvaluateType::can) {
                    array->dimension[i]= make_shared<ValueAST>(rs.second, dim_type);
                }
            }
        }
    }
}

void ExprAST::Analyze()
{
    if (IsUnaryOperator()) // 单目运算
    {
        roperand->Analyze();
        auto rs = roperand->Evaluate();
        ValueType rtype = roperand->GetType() == ValueType::IDENT ?
            Symbol_Table::GetType(roperand->GetName()) : roperand->GetType();

        if (op == OpType::NOT) { // 非运算结果为整型
            value_type = ValueType::INT;
        }
        else {
            value_type = rtype;
        }

        if (rs.first == EvaluateType::can) // 编译期可计算
        {
            Simplify(rtype, rs.second); // ExprAST的Analyze只能在编译期算一下值，缩成ValuesAST的步骤得在其他地方写，比如VarDefAST的Simplify中就包括把ExprAST缩成ValueAST
        }
        else // 编译期不可计算
        {
            canEvaluate = EvaluateType::canNot;
        }
    }
    else 
    {
        loperand->Analyze();
        roperand->Analyze();
        ValueType ltype = loperand->GetType() == ValueType::IDENT ? // 本质上还是GetType的问题，对于一个变量，我们有的时候要的是IDENT，有的时候要的是INT或FLOAT
            Symbol_Table::GetType(loperand->GetName()) : loperand->GetType();

        ValueType rtype = roperand->GetType() == ValueType::IDENT ?
            Symbol_Table::GetType(roperand->GetName()) : roperand->GetType();

        if (op == OpType::LE || op == OpType::LEQ || op == OpType::GE || op == OpType::GEQ || op == OpType::EQ || op == OpType::NEQ
            || op == OpType::AND || op == OpType::OR) {  // 所有关系运算的结果都是整型
            value_type = ValueType::INT;
        }
        else if (ltype == ValueType::FLOAT || rtype == ValueType::FLOAT) { // 这里处理的是之前ExprAST刚构造时不能处理的情况
            value_type = ValueType::FLOAT;
        }
        else {
            value_type = ValueType::INT;
        }

        auto l_rs = loperand->Evaluate(), r_rs = roperand->Evaluate();

        if (!WhileAST::is_While) { // 为什么在while语句中就不要执行下面这段话捏，如果是假的，整个while都被删了那无妨，如果是真的，那你不能把expr两边的表达式缩了，因为他肯定在接下来的while语句中有所改变，（如果不变，那就是死循环了）
            // 虽然整个Expr不能缩成一个ValueAST，但左子树，右子树能缩还是得缩的
            if (l_rs.first == EvaluateType::can) {
                Simplify(l_rs.second, 0, ltype);
            }
            else if (r_rs.first == EvaluateType::can) {
                Simplify(r_rs.second, 1, rtype);
            }
        }

        if (l_rs.first == EvaluateType::can && r_rs.first == EvaluateType::can) {
            if(op == OpType::MOD){
                if(ltype == ValueType::FLOAT || rtype == ValueType::FLOAT){
                    error_sys("FLOAT can not MOD");
                }
                else{
                    value.int_val = l_rs.second.int_val % r_rs.second.int_val;
                }
            }
            else{
                if (ltype == ValueType::INT && rtype == ValueType::INT) {
                    Simplify(op, ltype, l_rs.second.int_val, rtype, r_rs.second.int_val);
                }
                else if (ltype == ValueType::INT && rtype == ValueType::FLOAT) {
                    Simplify(op, ltype, l_rs.second.int_val, rtype, r_rs.second.float_val);
                }
                else if (ltype == ValueType::FLOAT && rtype == ValueType::INT) {
                    Simplify(op, ltype, l_rs.second.float_val, rtype, r_rs.second.int_val);
                }
                else if (ltype == ValueType::FLOAT && rtype == ValueType::FLOAT) {
                    Simplify(op, ltype, l_rs.second.float_val, rtype, r_rs.second.float_val);
                }
            }
        }
        else {
            canEvaluate = EvaluateType::canNot;
        }
    }
}

void FuncDefAST::Analyze()
{
    ValueType prev_type = FuncDefAST::func_type; string prev_name = FuncDefAST::func_name;
    FuncDefAST::func_type = type; FuncDefAST::func_name = *name;
    if(!Symbol_Table::Insert(this->shared_from_base<FuncDefAST>()))
        error_sys("redefinition of Function (FuncDefAST::Analyze() )");

    Symbol_Table::NewScope();
    if (params)
    {
        for (size_t i = 0; i < params->size(); ++i)
        {
            if(!Symbol_Table::Insert_Use(params->at(i)->var_defs[0]->shared_from_base<VarDef>()))
                error_sys("Error in Insert_Use()");

            if (params->at(i)->var_defs[0]->IsSingle()) {
                auto singleVar = dynamic_pointer_cast<SingleVarDef>(params->at(i)->var_defs[0]);
                if (!Symbol_Table::Insert(*singleVar, params->at(i)->type, false, false)) // 函数参数不允许const
                    error_sys("redefinition of parameter (FuncDefAST::Analyze() )");
            }
            else {
                // TODO: 数组参数插入符号表
                auto arrayVar = dynamic_pointer_cast<ArrayVarDef>(params->at(i)->var_defs[0]);
                if (!Symbol_Table::Insert(*arrayVar, params->at(i)->type, false, false)) // 函数参数不允许const
                    error_sys("redefinition of parameter (FuncDefAST::Analyze() )");
            }
        }
    }
    block->Analyze();

    ////尾递归消除
    //auto stmt = block->stmts->back();
    //if (stmt->GetNodeType() == ASTNodeType::RETURN)
    //{
    //    shared_ptr<ReturnAST> return_ast = dynamic_pointer_cast<ReturnAST>(stmt);
    //    if (return_ast->ret_value->GetNodeType() == ASTNodeType::FUNCCALL)
    //    {
    //        auto ret_value = dynamic_pointer_cast<FuncCallAST>(return_ast->ret_value);
    //        block->stmts->pop_back();
    //        block->end_jump_label = block->start_label = ++cnt; //将尾递归移至函数开头
    //        //检查函数定义与函数调用参数个数是否一致
    //        if (ret_value->param_vals->size() != params->size())
    //            error_sys("Tail recursion fail (NUM(FParams) != NUM(RParams))");
    //        for (size_t i = 0; i < params->size(); ++i)
    //        {
    //            auto& param = params->at(i)->var_defs[0];
    //            if (param->IsSingle())
    //            {
    //                block->stmts->push_back(make_shared<AssignmentAST>
    //                    (make_unique<ValueAST>(make_unique<SingleVarDef>(make_unique<string>(param->GetName()))),
    //                        move(ret_value->param_vals->at(i))));
    //            }
    //            else
    //            {
    //                //数组！！！！
    //            }
    //        }
    //        ret_value->param_vals->clear();
    //    }
    //}

    Symbol_Table::ExitScope();

    FuncDefAST::func_type = prev_type; FuncDefAST::func_name = prev_name;

}

void BlockAST::Analyze()
{
    if (stmts)
    {
        for (size_t i = 0; i < stmts->size(); ++i)
        {   
            int pre_size = stmts->size();
            if (stmts->at(i)->GetNodeType() == ASTNodeType::RETURN || stmts->at(i)->GetNodeType() == ASTNodeType::BREAK || stmts->at(i)->GetNodeType() == ASTNodeType::CONTINUE){
                stmts->at(i)->Analyze();
                if (i != stmts->size() - 1) {
                    stmts->erase(stmts->begin() + i + 1, stmts->end());  //去除return,continue,break后面的无用语句
                }
                break;
            }
            else if (stmts->at(i)->GetNodeType() == ASTNodeType::WHILE) {
                auto whileNode = dynamic_pointer_cast<WhileAST>(stmts->at(i));
                whileNode->Analyze(*stmts, i);
            }
            else if (stmts->at(i)->GetNodeType() == ASTNodeType::IF) {
                auto ifNode = dynamic_pointer_cast<IfAST>(stmts->at(i));
                ifNode->Analyze(*stmts, i);
            }
            else if (stmts->at(i)->GetNodeType() == ASTNodeType::BLOCK) {
                auto blockNode = dynamic_pointer_cast<BlockAST>(stmts->at(i));
                Symbol_Table::NewScope();
                blockNode->Analyze();
                Symbol_Table::ExitScope();
            }
            else {
                stmts->at(i)->Analyze();
            }
            if (stmts->size() == pre_size - 1)
                i--;
        }
    }
    //simplify_memset();
}

void AssignmentAST::whilePioneer() const {
    auto singleVar = dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def);
    if (singleVar)
        while_changed.push_back(*singleVar->name);
    else {
        auto arrayVar = dynamic_pointer_cast<ArrayVarDef>(lval_ref->var_def);
        while_changed.push_back(*arrayVar->name);
    }
}

void BlockAST::whilePioneer() const{
    if (stmts){
        for (size_t i = 0; i < stmts->size(); ++i){
            stmts->at(i)->whilePioneer();
        }
    }
}

void WhileAST::whilePioneer() const {
    block->whilePioneer();
}

void IfAST::whilePioneer() const {
    if (true_block) true_block->whilePioneer();
    if (false_block) false_block->whilePioneer();
}

void ReturnAST::Analyze()
{
    if (FuncDefAST::func_type == ValueType::UNKNOWN)
        error_sys("return statement not in function.");
    if (!ret_value)
        return;

    ret_value->Analyze();

    const auto ret_type = ret_value->GetType() == ValueType::IDENT ? Symbol_Table::GetType(ret_value->GetName()) : ret_value->GetType();
    if (FuncDefAST::func_type == ValueType::INT && ret_type != ValueType::INT && ret_type != ValueType::FLOAT)
        error_sys("Type checking failed. return type not matched.");
    else if (FuncDefAST::func_type == ValueType::FLOAT && ret_type != ValueType::INT && ret_type != ValueType::FLOAT)
        error_sys("Type checking failed. return type not matched.");

    auto rs = ret_value->Evaluate();
    if (rs.first == EvaluateType::can) {                                               // 因为如果是float类型的赋值给了int类型
        if (FuncDefAST::func_type == ValueType::INT && ret_type == ValueType::FLOAT) { // 我必须修改res，如果把不修改的res存到符号表 
            rs.second.int_val = (int)rs.second.float_val;                              // 那就会出现一个问题，在生成汇编的时候，当我要显式拿一个int类型的变量的时候
        }                                                                              // 但实际上ValueAST的union类型的value是个float，就会出现错误
        else if (FuncDefAST::func_type == ValueType::FLOAT && ret_type == ValueType::INT) {
            rs.second.float_val = (float)rs.second.int_val;
        }
        Simplify(shared_from_base<ReturnAST>(), rs.second, FuncDefAST::func_type);
    }
    //Symbol_Table::FuncTables[FuncDefAST::func_name]->canEvaluate = rs.first; // 一个函数本身可不可以被计算只取决于他return的东西
    //Symbol_Table::FuncTables[FuncDefAST::func_name]->value = rs.second;
    {
        Origin rs = ret_value->addToMul();
        if (rs.cnt != -1 && rs.cnt != 1) {
            if (rs.dim.size() == 0) {
                ret_value = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(rs.cnt),
                    make_shared<ValueAST>(make_unique<SingleVarDef>(make_unique<string>(rs.name)), rs.type));
            }
            else {
                vector<shared_ptr<DeclAST>>dimension;
                for (int index = 0; index < rs.dim.size(); index++) {
                    dimension.push_back(make_shared<ValueAST>(rs.dim[index]));
                }
                ret_value = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(rs.cnt),
                    make_shared<ValueAST>(make_unique<ArrayVarDef>((make_unique<string>(rs.name)), dimension), rs.type));
            }
        }
    }
}

void WhileAST::Analyze(vector<shared_ptr<StmtAST>>& targetStmts, int index)
{
    if (while_changed.size() != 0) while_changed.clear();
    block->whilePioneer();
    for (int i = 0; i < while_changed.size(); i++) {
        Symbol_Table::ChangeEvaluate(while_changed[i]);
    }

    WhileAST::is_While = true;
    WhileAST::is_While_cond = true;
    cond->Analyze();
    WhileAST::is_While_cond = false;
    auto rs = cond->Evaluate();
    ValueType condtype = cond->GetType() == ValueType::IDENT ?
        Symbol_Table::GetType(cond->GetName()) : cond->GetType();

    if (rs.first == EvaluateType::can) {
        // Simplify(shared_from_base<WhileAST>(), rs.second, condtype); // 这句话注释的原因，cond为真不能缩，cond为假不用缩

        // 要是while语句的条件编译期间可算并且条件不成立，则直接在原有的vector类型的stmts中删去while语句
        if (condtype == ValueType::INT) {
            if (rs.second.int_val <= 0) {
                targetStmts.erase(targetStmts.begin() + index);
            }
        }
        else if (condtype == ValueType::FLOAT) {
            if (rs.second.float_val <= 0) {
                targetStmts.erase(targetStmts.begin() + index);
            }
        }
    }

    Symbol_Table::NewScope();
    auto next_if = dynamic_pointer_cast<IfAST>(block);
    if (next_if)
        next_if->Analyze(targetStmts, index);
    else {
        auto next_while = dynamic_pointer_cast<WhileAST>(block);
        if (next_while) next_while->Analyze(targetStmts, index);
        else block->Analyze();
    }

    WhileAST::is_While = false;
    Symbol_Table::ExitScope();
}

void IfAST::Analyze(vector<shared_ptr<StmtAST>>& targetStmts, int index)
{
    cond->Analyze();
    auto rs = cond->Evaluate();
    ValueType condtype = cond->GetType() == ValueType::IDENT ?
        Symbol_Table::GetType(cond->GetName()) : cond->GetType();

    if (rs.first == EvaluateType::can) {
        Simplify(shared_from_base<IfAST>(), rs.second, condtype);

        // 优化If语句
        if (condtype == ValueType::INT) {
            if (rs.second.int_val <= 0) {
                if (false_block) { // 条件不成立，有else语句，那把else语句的block直接接在targetStmts上即可
                    targetStmts[index] = false_block;
                    Symbol_Table::NewScope();
                    auto next_if = dynamic_pointer_cast<IfAST>(false_block);
                    if (next_if) 
                        next_if->Analyze(targetStmts, index);
                    else {
                        auto next_while = dynamic_pointer_cast<WhileAST>(false_block);
                        if (next_while) next_while->Analyze(targetStmts, index);
                        else false_block->Analyze(); // 记得重新Analyze一下
                    }
                    Symbol_Table::ExitScope();
                }
                else { // 条件不成立，并且没有else语句，那就直接删了
                    targetStmts.erase(targetStmts.begin() + index);
                }
            }
            else {
                if (true_block) {  // 条件恒成立，把if的true_block接在targetStmts上即可
                    targetStmts[index] = true_block;
                    Symbol_Table::NewScope();
                    auto next_if = dynamic_pointer_cast<IfAST>(true_block);
                    if (next_if)
                        next_if->Analyze(targetStmts, index);
                    else {
                        auto next_while = dynamic_pointer_cast<WhileAST>(true_block);
                        if (next_while) next_while->Analyze(targetStmts, index);
                        else true_block->Analyze(); // 记得重新Analyze一下
                    }
                    Symbol_Table::ExitScope();
                }
            }
        }
        else if (condtype == ValueType::FLOAT) {
            if (rs.second.float_val <= 0) {
                if (false_block) { // 条件不成立，有else语句，那把else语句的block直接接在targetStmts上即可
                    targetStmts[index] = false_block;
                    Symbol_Table::NewScope();
                    auto next_if = dynamic_pointer_cast<IfAST>(false_block);
                    if (next_if)
                        next_if->Analyze(targetStmts, index);
                    else {
                        auto next_while = dynamic_pointer_cast<WhileAST>(false_block);
                        if (next_while) next_while->Analyze(targetStmts, index);
                        else false_block->Analyze(); // 记得重新Analyze一下
                    }
                    Symbol_Table::ExitScope();
                }
                else {  // 条件不成立，并且没有else语句，那就直接删了
                    targetStmts.erase(targetStmts.begin() + index);
                }
            }
            else {
                if (true_block) { // 条件恒成立，把if的true_block接在targetStmts上即可
                    targetStmts[index] = true_block;
                    Symbol_Table::NewScope();
                    auto next_if = dynamic_pointer_cast<IfAST>(true_block);
                    if (next_if)
                        next_if->Analyze(targetStmts, index);
                    else {
                        auto next_while = dynamic_pointer_cast<WhileAST>(true_block);
                        if (next_while) next_while->Analyze(targetStmts, index);
                        else true_block->Analyze(); // 记得重新Analyze一下
                    }
                    Symbol_Table::ExitScope();
                }
            }
        }
    }
    else { // 不可以编译期间计算的话，记得Analyze
        if (if_else_changed.size() != 0) if_else_changed.clear();
        Symbol_Table::NewScope();
        IfAST::is_If = true;
        auto next_if = dynamic_pointer_cast<IfAST>(true_block);
        if (next_if)
            next_if->Analyze(targetStmts, index);
        else {
            auto next_while = dynamic_pointer_cast<WhileAST>(true_block);
            if (next_while) next_while->Analyze(targetStmts, index);
            else if (true_block) true_block->Analyze(); // 记得重新Analyze一下
        }
        Symbol_Table::ExitScope();
        Symbol_Table::clearChanged(if_else_changed);
        if (false_block) {
            Symbol_Table::NewScope();
            auto TMP_block = dynamic_pointer_cast<IfAST>(false_block);
            if (TMP_block) TMP_block->Analyze(targetStmts, index);
            else false_block->Analyze(); // 记得重新Analyze一下
            Symbol_Table::ExitScope();
        }
        IfAST::is_If = false;
        for (int i = 0; i < if_else_changed.size(); i++) {
            Symbol_Table::ChangeEvaluate(if_else_changed[i].first);
        }
    }
}

void FuncCallAST::Analyze()
{
    if (*name == "putf") { // 由于格式化输出函数的特殊性，得单独处理
        value_type == ValueType::VOID;
        vector<shared_ptr<VarDefAST>>* temp_params = new vector<shared_ptr<VarDefAST>>();
        {
            auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
            param->type = ValueType::STRING;
            temp_params->push_back(param);
        }
        for (size_t i = 1; i < param_vals->size(); i++) {
            param_vals->at(i)->Analyze();
            ValueType TMP_Type = param_vals->at(i)->GetType() == ValueType::IDENT ?
                Symbol_Table::GetType(param_vals->at(i)->GetName()) : param_vals->at(i)->GetType();
            string param_name = "param_" + to_string(i + 1);
            auto param = make_shared<VarDefAST>(make_shared<SingleVarDef>(make_unique<string>("param_1")));
            param->type = TMP_Type;
            temp_params->push_back(param);
        }
        func_def = make_shared<FuncDefAST>(ValueType::VOID, make_unique<string>("putf"), unique_ptr<vector<shared_ptr<VarDefAST>>>(temp_params), make_unique<BlockAST>());
        return;
    }

    if ((func_def = Symbol_Table::GetFuncDef(*name)) == NULL)
        error_sys("Function not defined (FuncCallAST::Analyze() )");
    value_type = func_def->type;
    if (func_def->params) {
        if (!param_vals || param_vals->size() != func_def->params->size()) {
            error_sys("Type checking fail (NUM(FParams) != NUM(RParams))");
        }
        for (size_t i = 0; i < param_vals->size(); i++) {
            auto var = dynamic_pointer_cast<ValueAST>(param_vals->at(i));
            if (var && var->var_def) { // 说明实参是个变量
                auto tmp_array = dynamic_pointer_cast<ArrayVarDef>(var->var_def); // 实参如果是个数组，那么tmp_array不为空，是个变量，则为空
                if (!func_def->params->at(i)->var_defs.at(0)->IsSingle()
                    && !tmp_array) {
                    param_vals->at(i) = make_unique<ValueAST>(make_shared<ArrayVarDef>(make_unique<string>(param_vals->at(i)->GetName())));
                }
            }
            param_vals->at(i)->Analyze();
            ValueType TMP_Type = param_vals->at(i)->GetType() == ValueType::IDENT ?
                Symbol_Table::GetType(param_vals->at(i)->GetName()) : param_vals->at(i)->GetType();

            if (TMP_Type == ValueType::INT && func_def->params->at(i)->type != ValueType::INT && func_def->params->at(i)->type != ValueType::FLOAT) {
                error_sys("Type checking fail (Type(FParam) != Type(RParam))");
            }
            else if (TMP_Type == ValueType::FLOAT && func_def->params->at(i)->type != ValueType::INT && func_def->params->at(i)->type != ValueType::FLOAT) {
                error_sys("Type checking fail (Type(FParam) != Type(RParam))");
            }
        }
    }
    
    canEvaluate = EvaluateType::canNot;
    //if (func_def->block->stmts->size() >= INLINE_FUNCTION_LIMIT)
    //    return;
    //inline_block = BlockAST();
    //for (size_t i = 0; i < param_vals->size(); ++i)
    //{
    //    const auto& param_def = func_def->params->at(i);
    //    if (param_def->var_defs[0]->IsSingle())
    //    {
    //        inline_block.stmts->push_back(
    //            make_shared<AssignmentAST>(make_unique<ValueAST>(
    //                make_unique<SingleVarDef>(make_unique<string>(param_def->var_defs[0]->GetName()))),
    //                move(param_vals->at(i))));
    //    }
    //    else
    //    {
    //        //数组！！！！！！
    //    }
    //}
    ////清除当前的参数列表，避免被重复使用，因为已经被move了
    //param_vals->clear();
}
