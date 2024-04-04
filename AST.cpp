#include "AST.h"
#include <functional>
#include<iostream>
#include<memory>
#include "Symbol_Table.h"
#include "Utility.h"
#include "Constants.h"
#include <set>
#include "LiteralPool.h"
using namespace std;

#define LABEL_GUARD LabelGuard label_guard(&start_label, &end_jump_label, &should_restore)
#define SYMBOL_TABLE_GUARD SymbolTableGuard symbol_table_guard
#define BOTH_GUARD LABEL_GUARD; SYMBOL_TABLE_GUARD;
#define NUM_S_REGISTER 12

vector<shared_ptr<DefAST>> global_var_defs;


//记录当前continue与break语句所属的循环语句的标号
string StmtAST::continue_label = "", StmtAST::break_label = "";

//记录当前所在函数的返回值类型
ValueType FuncDefAST::func_type = ValueType::UNKNOWN;

// 记录当前所在函数的函数名
string FuncDefAST::func_name = "";

string FuncDefAST::current_return_label = "";

int FuncDefAST::max_param_length = 2;

shared_ptr<FuncDefAST> FuncDefAST::memset_int, FuncDefAST::memset_float;

int FuncCallAST::funccall_cnt = 0;

int FuncCallAST::starttime_cnt = 0, FuncCallAST::stoptime_cnt = 0;

bool VarDefAST::is_current_global = false;
ValueType VarDefAST::current_var_type = ValueType::UNKNOWN;

//将当前所有可以用于存储临时变量的寄存器记录下来
void InitRegister()
{
    avail_register_int.insert("t0");
    avail_register_int.insert("t1");
    avail_register_int.insert("t2");
    avail_register_int.insert("t3");
    avail_register_int.insert("t4");
    avail_register_int.insert("t5");
    avail_register_int.insert("t6");

    if (O1_optimize)
    {
        extra_register.insert("s1");
        extra_register.insert("s2");
        extra_register.insert("s3");
        extra_register.insert("s4");
        extra_register.insert("s5");
        extra_register.insert("s6");
        extra_register.insert("s7");
        extra_register.insert("s8");
        extra_register.insert("s9");
        extra_register.insert("s10");
        extra_register.insert("s11");
    }

    avail_register_float.insert("ft0");
    avail_register_float.insert("ft1");
    avail_register_float.insert("ft2");
    avail_register_float.insert("ft3");
    avail_register_float.insert("ft4");
    avail_register_float.insert("ft5");
    avail_register_float.insert("ft6");
    avail_register_float.insert("ft7");
    avail_register_float.insert("ft8");
    avail_register_float.insert("ft9");
    avail_register_float.insert("ft10");
    avail_register_float.insert("ft11");
}

//检查并设置reg为一个可用的寄存器，如果没有寄存器可用则抛出异常
string CheckAndSetRegister_INT(string& reg)
{
    if (reg.empty())
    {
        if (avail_register_int.empty())
            error_sys("In CheckRegister: No register available!");
        else
        {
            reg = RandomFromSet(avail_register_int);
        }
    }
    avail_register_int.erase(reg);
    Symbol_Table::PutRegisterToMemory(reg, false);
    return reg;
}

string CheckAndSetRegister_FLOAT(string& reg)
{
    if (reg.empty())
    {
        if (avail_register_float.empty())
            error_sys("In CheckRegister: No register available!");
        else
        {
            reg = RandomFromSet(avail_register_float);
        }
    }
    avail_register_float.erase(reg);
    Symbol_Table::PutRegisterToMemory(reg, true);
    return reg;
}

bool CheckAndSetRegister_EXTRA(string& reg)
{
    if (reg.empty())
    {
        if (extra_register.empty())
            return false;
        else
        {
            reg = RandomFromSet(extra_register);
        }
    }
    extra_register.erase(reg);
    //Symbol_Table::PutRegisterToMemory(reg, false);
    return true;
}

ValueType GetRegisterType(string reg)
{
    if (reg.size() < 2)
        return ValueType::UNKNOWN;

    if (O1_optimize)
    {
        if (reg[0] == 't' || (reg[0] == 's' && reg[1] != '0'))
            return ValueType::INT;
        else if (reg[0] == 'f' && (reg[1] == 't'))
            return ValueType::FLOAT;
        else
            return ValueType::UNKNOWN;
    }
    else
    {
        if (reg[0] == 't')
            return ValueType::INT;
        else if (reg[0] == 'f' && (reg[1] == 't'))
            return ValueType::FLOAT;
        else
            return ValueType::UNKNOWN;
    }
}

//将reg重新变为可以被使用的状态，即重新插入avail_register中
void RestoreRegister(string reg, int freq)
{
    if (!reg.empty())
    {
        if(O1_optimize)
        {
            ValueType reg_type = GetRegisterType(reg);
            if (reg_type == ValueType::INT)
            {
                if (reg[0] == 's')
                {
                    //do nothing
	                //extra_register.insert({ reg, freq });
                }
                else
                    avail_register_int.insert({ reg, freq });
            }
            else if (reg_type == ValueType::FLOAT)
                avail_register_float.insert({ reg, freq });
        }
        else
        {
            ValueType reg_type = GetRegisterType(reg);
            if (reg_type == ValueType::INT)
                avail_register_int.insert({ reg, freq });
            else if (reg_type == ValueType::FLOAT)
                avail_register_float.insert({ reg, freq });
        }
    }
}

LabelGuard::LabelGuard(string* _start_label, string* _end_jump, bool* _should_restore) :
    start_label(_start_label), end_jump(_end_jump), should_restore(_should_restore)
{
    if (start_label != nullptr && *start_label != "")
        output << *start_label << ":\n";
}

LabelGuard::~LabelGuard()
{
    if (should_restore != nullptr && *should_restore)
        Symbol_Table::PutAllToMemory(false);
    if (end_jump != nullptr && *end_jump != "")
    {
        Symbol_Table::PutAllToMemory(false);
        output << "\tj " << *end_jump << "\n";
    }
}

SymbolTableGuard::SymbolTableGuard()
{
    Symbol_Table::NewScope();
}

SymbolTableGuard::~SymbolTableGuard()
{
    Symbol_Table::ExitScope();
}

ASTNodeType StmtAST::GetNodeType() const
{
    return ASTNodeType::None;
}

ValueAST::ValueAST(ValueType _value_type) : value_type(_value_type) {
    if (_value_type == ValueType::INT) {
        value.int_val = 0;
    }
    else if (_value_type == ValueType::FLOAT) {
        value.float_val = 0;
    }
    else {
        cout << "error" << endl;
        exit(-1);
    }
    canEvaluate = EvaluateType::can;
}
ValueAST::ValueAST(int _value) :value_type(ValueType::INT) { value.int_val = _value; canEvaluate = EvaluateType::can; }
ValueAST::ValueAST(float _value) :value_type(ValueType::FLOAT) { value.float_val = _value; canEvaluate = EvaluateType::can; FloatPool::Insert(_value); }

ValueAST::ValueAST(unique_ptr<string> _value) :
    value_type(ValueType::STRING), str(move(_value))
{
    canEvaluate = EvaluateType::canNot;
}

// 这个构造函数是专门为VarDefAST的simplify产生的
ValueAST::ValueAST(Value _value, ValueType _type) :value(_value), value_type(_type)
{
    canEvaluate = EvaluateType::can;
    if (_type == ValueType::FLOAT)
        FloatPool::Insert(_value.float_val);
}

ValueAST::ValueAST(shared_ptr<VarDef> _var_def) :
    value_type(ValueType::IDENT), var_def(move(_var_def))
{
    canEvaluate = EvaluateType::canNot;
}

ValueAST::ValueAST(shared_ptr<VarDef> _var_def, ValueType _type) :
    value_type(_type), var_def(move(_var_def))
{
    canEvaluate = EvaluateType::canNot;
}

void ValueAST::Dump() const
{
    if (!var_def) {
        if (value_type == ValueType::INT)
            cout << "ValueAST { value: " << value.int_val << ", type: " << ToString(value_type) << " }";
        else if (value_type == ValueType::FLOAT)
            cout << "ValueAST { value: " << value.float_val << ", type: " << ToString(value_type) << " }";
        else if (value_type == ValueType::STRING)
            cout << "ValueAST { value: " << (*str) << ", type: " << ToString(value_type) << " }";
    }
    else
        var_def->Dump();
}


pair<EvaluateType, Value> ValueAST::Evaluate()
{
    return { canEvaluate, value };
}

Origin ValueAST::addToMul()
{
    vector<int>tmp_dim;
    if (var_def) {
        int tmp_cnt = 1;
        if (!var_def->IsSingle()) {
            auto array = dynamic_pointer_cast<ArrayVarDef>(var_def);
            for (int index = 0; index < array->dimension.size(); index++) {
                if (array->dimension[index]->Evaluate().first == EvaluateType::canNot)
                    tmp_cnt = -1;
                else tmp_dim.push_back(array->dimension[index]->Evaluate().second.int_val);
            }
        }
        return{ tmp_cnt,value_type,GetName(),tmp_dim };
    }
    return{ 1,value_type,"",tmp_dim };
}

ValueType ValueAST::GetType() const
{
    return value_type;
}

std::string ValueAST::GetName() const
{
    if (var_def)
        return var_def->GetName();
    else
        return "";
}

string ValueAST::GenerateRSICV(string target_reg)
{
    if (!var_def)
    {
        if (value_type == ValueType::INT)
        {
            CheckAndSetRegister_INT(target_reg);
            output << "\tli " << target_reg << ", " << value.int_val << endl;
        }
        else if (value_type == ValueType::FLOAT)
        {
            CheckAndSetRegister_FLOAT(target_reg);
            output << "\tlla s0, " << FloatPool::GetLabel(value.float_val) << endl;
            output << "\tflw " << target_reg << ", 0(s0)" << endl;
        }
        else
            error_sys("In GenerateRSICV: ValueAST is not a value!");
    }
    else {
        target_reg = var_def->GenerateRSICV(target_reg);
    }
    return target_reg;
}

ExprAST::ExprAST(OpType _op, unique_ptr<DeclAST> _roperand) : op(_op), roperand(move(_roperand))
{
    value_type = roperand->GetType(); // 这里只处理了非变量的表达式类型赋值！！！处理不了-a的表达式类型赋值！因为这个时候还没有符号表！
    canEvaluate = EvaluateType::canNot;
}
ExprAST::ExprAST(OpType _op, unique_ptr<DeclAST> _loperand, unique_ptr<DeclAST> _roperand) :
    op(_op), loperand(move(_loperand)), roperand(move(_roperand))
{
    canEvaluate = EvaluateType::canNot;
    ValueType ltype = loperand->GetType(); // 这里只处理了非变量的表达式类型赋值！！！处理不了a+b的表达式类型赋值！因为这个时候还没有符号表！
    ValueType rtype = roperand->GetType();
    //Set the ValueType of current node.
    if (ltype == ValueType::FLOAT || rtype == ValueType::FLOAT) {
        value_type = ValueType::FLOAT;
    }
    else {
        value_type = ValueType::INT;
    }
}

ExprAST::ExprAST(OpType _op, shared_ptr<DeclAST> _loperand, shared_ptr<DeclAST> _roperand) :
    op(_op), loperand(_loperand), roperand(_roperand)
{
    canEvaluate = EvaluateType::canNot;
    ValueType ltype = loperand->GetType(); // 这里只处理了非变量的表达式类型赋值！！！处理不了a+b的表达式类型赋值！因为这个时候还没有符号表！
    ValueType rtype = roperand->GetType();
    //Set the ValueType of current node.
    if (ltype == ValueType::FLOAT || rtype == ValueType::FLOAT) {
        value_type = ValueType::FLOAT;
    }
    else {
        value_type = ValueType::INT;
    }
}

void ExprAST::Dump() const
{
    cout << "ExprAST { op: " << ToString(op) << ", type: " << ToString(GetType()) << ", ";
    if (IsUnaryOperator())
    {
        roperand->Dump();
        cout << " }";
    }
    else
    {
        loperand->Dump();
        cout << ", ";
        roperand->Dump();
        cout << " }";
    }
}

pair<EvaluateType, Value> ExprAST::Evaluate()
{
    return { canEvaluate, value };
}

Origin ExprAST::addToMul()
{
    int ans = -1; ValueType _type = ValueType::UNKNOWN; string _name = ""; vector<int>_dim;
    if (op == OpType::PLUS && loperand) { // loperand是为了防止 +b 的情况
        auto rs1 = loperand->Evaluate(); auto rs2 = roperand->Evaluate();
        if (rs1.first == EvaluateType::canNot || rs2.first == EvaluateType::canNot) {
            Origin ans1 = loperand->addToMul(); Origin ans2 = roperand->addToMul();
            if (ans1.cnt != -1 && ans1.cnt != 1) {
                if (ans1.dim.size() == 0) {
                    loperand = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(ans1.cnt),
                        make_shared<ValueAST>(make_unique<SingleVarDef>(make_unique<string>(ans1.name)), ans1.type));
                }
                else {
                    vector<shared_ptr<DeclAST>>dimension;
                    for (int index = 0; index < ans1.dim.size(); index++) {
                        dimension.push_back(make_shared<ValueAST>(ans1.dim[index]));
                    }
                    loperand = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(ans1.cnt),
                        make_shared<ValueAST>(make_unique<ArrayVarDef>((make_unique<string>(ans1.name)), dimension), ans1.type));
                }
            }
            if (ans2.cnt != -1 && ans2.cnt != 1) {
                if (ans2.dim.size() == 0) {
                    roperand = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(ans2.cnt),
                        make_shared<ValueAST>(make_unique<SingleVarDef>(make_unique<string>(ans2.name)), ans2.type));
                }
                else {
                    vector<shared_ptr<DeclAST>>dimension;
                    for (int index = 0; index < ans2.dim.size(); index++) {
                        dimension.push_back(make_shared<ValueAST>(ans2.dim[index]));
                    }
                    roperand = make_unique<ExprAST>(OpType::MUL, make_shared<ValueAST>(ans2.cnt),
                        make_shared<ValueAST>(make_unique<ArrayVarDef>((make_unique<string>(ans2.name)), dimension), ans2.type));
                }
            }
            if (ans1 == ans2) {
                ans = ans1.cnt + ans2.cnt;
                _type = ans1.type;
                _name = ans1.name;
                _dim = ans1.dim;
            }
        }
    }
    return { ans, _type, _name, _dim };
}

bool ExprAST::IsUnaryOperator() const
{
    return !loperand && roperand;
}

ValueType ExprAST::GetType() const
{
    return value_type;
}

string ExprAST::GenerateRSICV(string target_reg)
{
    //等evaluate完成后对立即数做出优化
    if (IsUnaryOperator())
    {
        string rreg = roperand->GenerateRSICV();

        if (GetType() == ValueType::INT)
            target_reg = CheckAndSetRegister_INT(target_reg);
        else if (GetType() == ValueType::FLOAT)
            target_reg = CheckAndSetRegister_FLOAT(target_reg);
        else
            error_sys("In ExprAST::GenerateRSICV: Unknown ValueType!");

        if (GetType() == ValueType::INT)
        {
            if (op == OpType::NOT)
                output << "\tseqz " << target_reg << ", " << rreg << endl;
            else if (op == OpType::MINUS)
                output << "\tneg " << target_reg << ", " << rreg << endl;
        }
        else if (GetType() == ValueType::FLOAT)
        {
            if (op == OpType::NOT)
            {
                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                string result_reg;
                CheckAndSetRegister_INT(result_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << result_reg << ", " << target_reg << ", " << zero_reg << endl;
                RestoreRegister(zero_reg);
                RestoreRegister(target_reg);
                target_reg = result_reg;
            }
            else if (op == OpType::MINUS)
            {
                output << "\tfneg.s " << target_reg << ", " << rreg << endl;
            }
        }

        if (target_reg == rreg)
            error_sys("In ExprAST::GenerateRSICV: target_reg == rreg");
        RestoreRegister(rreg);
    }
    else if(O1_optimize)
    {
        ValueType ltype = loperand->GetType(), rtype = roperand->GetType();
        if (ltype != ValueType::INT && ltype != ValueType::FLOAT)
            ltype = ValueType::INT;
        if (rtype != ValueType::INT && rtype != ValueType::FLOAT)
            rtype = ValueType::INT;

        string short_circuit_label;
        string lreg = loperand->GenerateRSICV();
        if (op == OpType::AND)
        {
            Symbol_Table::PutAllToMemory(false);
            short_circuit_label = GetLabel(++cnt);

            if (ltype == ValueType::FLOAT)
            {
                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                string result_reg;
                CheckAndSetRegister_INT(result_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << result_reg << ", " << lreg << ", " << zero_reg << endl;
                output << "\tseqz " << result_reg << ", " << result_reg << endl;
                RestoreRegister(zero_reg);
                RestoreRegister(lreg);
                lreg = result_reg;
                ltype = ValueType::INT;
            }
            output << "\tbeqz " << lreg << ", " << short_circuit_label << endl;
        }
        else if (op == OpType::OR)
        {
            Symbol_Table::PutAllToMemory(false);
            short_circuit_label = GetLabel(++cnt);
            if (ltype == ValueType::FLOAT)
            {
                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                string result_reg;
                CheckAndSetRegister_INT(result_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << result_reg << ", " << lreg << ", " << zero_reg << endl;
                output << "\tseqz " << result_reg << ", " << result_reg << endl;
                RestoreRegister(zero_reg);
                RestoreRegister(lreg);
                lreg = result_reg;
                ltype = ValueType::INT;
            }
            output << "\tbnez " << lreg << ", " << short_circuit_label << endl;
        }

        avail_register_int.erase(lreg);

        if (GetType() == ValueType::INT && (op == OpType::PLUS || op == OpType::MINUS))
        {
            auto eval_result = roperand->Evaluate();
            int rvalue;
            if (eval_result.first == EvaluateType::can)
            {
                rvalue = eval_result.second.int_val;
                if (rvalue <= 2047 && rvalue >= -2047)
                {
                    CheckAndSetRegister_INT(target_reg);
                    if (op == OpType::PLUS)
                        output << "\taddiw " << target_reg << ", " << lreg << ", " << rvalue << endl;
                    else if (op == OpType::MINUS)
                        output << "\taddiw " << target_reg << ", " << lreg << ", " << -rvalue << endl;

                    if (target_reg == lreg)
                        error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
                    RestoreRegister(lreg);
                    return target_reg;
                }
            }
        }

        string rreg = roperand->GenerateRSICV();
        if ((op == OpType::AND || op == OpType::OR) && rtype == ValueType::FLOAT)
        {
            string zero_reg;
            CheckAndSetRegister_FLOAT(zero_reg);
            string result_reg;
            CheckAndSetRegister_INT(result_reg);
            output << "\tfmv.s.x " << zero_reg << ",zero\n";
            output << "\tfeq.s " << result_reg << ", " << rreg << ", " << zero_reg << endl;
            output << "\tseqz " << result_reg << ", " << result_reg << endl;
            RestoreRegister(zero_reg);
            RestoreRegister(rreg);
            rreg = result_reg;
            rtype = ValueType::INT;
        }

        if (op == OpType::AND || op == OpType::OR) {
            Symbol_Table::PutAllToMemory(false);
        }

        avail_register_int.erase(lreg);
        avail_register_int.erase(rreg);

        if (GetType() == ValueType::INT)
            target_reg = CheckAndSetRegister_INT(target_reg);
        else if (GetType() == ValueType::FLOAT)
            target_reg = CheckAndSetRegister_FLOAT(target_reg);
        else
            error_sys("In ExprAST::GenerateRSICV: Unknown ValueType!");

        if (ltype == ValueType::INT && rtype == ValueType::INT)
        {
            if (op == OpType::PLUS)
                output << "\taddw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MINUS)
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MUL)
                output << "\tmulw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::DIV)
                output << "\tdivw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MOD)
                output << "\tremw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::EQ)
            {
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tseqz " << target_reg << ", " << target_reg << endl;
            }
            else if (op == OpType::NEQ)
            {
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tsnez " << target_reg << ", " << target_reg << endl;
            }
            else if (op == OpType::LE)
                output << "\tslt " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::GE)
                output << "\tsgt " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::LEQ)
            {
                output << "\tsgt " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\txori " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::GEQ)
            {
                output << "\tslt " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\txori " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::AND)
            {
                /*output << "\tsnez " << lreg << ", " << lreg << endl;
                if (lreg != rreg)
                    output << "\tsnez " << rreg << ", " << rreg << endl;
                output << "\tand " << target_reg << ", " << lreg << ", " << rreg << endl;*/
                output << "\tsnez " << target_reg << ", " << rreg << endl;
            }
            else if (op == OpType::OR)
            {
                //output << "\tor " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tsnez " << target_reg << ", " << rreg << endl;
            }
            else
                error_sys("In ExprAST::GenerateRSICV: Operator not supported");

            //确保target_reg不等于lreg和rreg
            if (target_reg == lreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
            if (target_reg == rreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == rreg");
            RestoreRegister(lreg);
            RestoreRegister(rreg);
        }
        else//有一个是float
        {
            //类型转换
            if (ltype != ValueType::FLOAT || rtype != ValueType::FLOAT)
            {
                if (loperand->GetType() == ValueType::INT)
                {
                    lreg = TypeConvert(ValueType::INT, lreg, ValueType::FLOAT);
                }
                else // roperand是INT
                {
                    rreg = TypeConvert(ValueType::INT, rreg, ValueType::FLOAT);
                }
            }

            if (op == OpType::PLUS)
                output << "\tfadd.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MINUS)
                output << "\tfsub.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MUL)
                output << "\tfmul.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::DIV)
                output << "\tfdiv.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::EQ)
            {
                output << "\tfeq.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            }
            else if (op == OpType::NEQ)
            {
                string temp_reg;
                CheckAndSetRegister_FLOAT(temp_reg);
                output << "\tfsub.s " << temp_reg << ", " << lreg << ", " << rreg << endl;

                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << target_reg << ", " << temp_reg << ", " << zero_reg << endl;
                output << "\tseqz " << target_reg << ", " << target_reg << endl;
                RestoreRegister(temp_reg);
                RestoreRegister(zero_reg);
            }
            else if (op == OpType::LE)
                output << "\tflt.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::GE)
                output << "\tflt.s " << target_reg << ", " << rreg << ", " << lreg << endl;
            else if (op == OpType::LEQ)
            {
                output << "\tfle.s " << target_reg << ", " << lreg << ", " << rreg << endl;
                //output << "\tfxori.s " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::GEQ)
            {
                output << "\tfle.s " << target_reg << ", " << rreg << ", " << lreg << endl;
                //output << "\tfxori.s " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else
                error_sys("In ExprAST::GenerateRSICV: Operator not supported");

            if (target_reg == lreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
            if (target_reg == rreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == rreg");
            RestoreRegister(lreg);
            RestoreRegister(rreg);
        }

        if (!short_circuit_label.empty())
        {
            string skip_short_circuit_label = GetLabel(++cnt);
            output << "\tj " << skip_short_circuit_label << endl;
            output << short_circuit_label << ":\n";
            output << "\tmv " << target_reg << ", " << lreg << endl;
            output << skip_short_circuit_label << ":\n";
        }
		return target_reg;
    }
    else
    {
        ValueType ltype = loperand->GetType(), rtype = roperand->GetType();
        if (ltype != ValueType::INT && ltype != ValueType::FLOAT)
            ltype = ValueType::INT;
        if (rtype != ValueType::INT && rtype != ValueType::FLOAT)
            rtype = ValueType::INT;

        string short_circuit_label;
        string lreg = loperand->GenerateRSICV();
        if (op == OpType::AND)
        {
            Symbol_Table::PutAllToMemory(false);
            short_circuit_label = GetLabel(++cnt);

            if (ltype == ValueType::FLOAT)
            {
                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                string result_reg;
                CheckAndSetRegister_INT(result_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << result_reg << ", " << lreg << ", " << zero_reg << endl;
                output << "\tseqz " << result_reg << ", " << result_reg << endl;
                RestoreRegister(zero_reg);
                RestoreRegister(lreg);
                lreg = result_reg;
                ltype = ValueType::INT;
            }
            output << "\tbeqz " << lreg << ", " << short_circuit_label << endl;
        }
        else if (op == OpType::OR)
        {
            Symbol_Table::PutAllToMemory(false);
            short_circuit_label = GetLabel(++cnt);
            if (ltype == ValueType::FLOAT)
            {
                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                string result_reg;
                CheckAndSetRegister_INT(result_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << result_reg << ", " << lreg << ", " << zero_reg << endl;
                output << "\tseqz " << result_reg << ", " << result_reg << endl;
                RestoreRegister(zero_reg);
                RestoreRegister(lreg);
                lreg = result_reg;
                ltype = ValueType::INT;
            }
            output << "\tbnez " << lreg << ", " << short_circuit_label << endl;
        }

        CheckAndSetRegister_INT(lreg);

        if (GetType() == ValueType::INT && (op == OpType::PLUS || op == OpType::MINUS))
        {
            auto eval_result = roperand->Evaluate();
            int rvalue;
            if (eval_result.first == EvaluateType::can)
            {
                rvalue = eval_result.second.int_val;
                if (rvalue <= 2047 && rvalue >= -2047)
                {
                    CheckAndSetRegister_INT(target_reg);
                    if (op == OpType::PLUS)
                        output << "\taddiw " << target_reg << ", " << lreg << ", " << rvalue << endl;
                    else if (op == OpType::MINUS)
                        output << "\taddiw " << target_reg << ", " << lreg << ", " << -rvalue << endl;

                    if (target_reg == lreg)
                        error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
                    RestoreRegister(lreg);
                    return target_reg;
                }
            }
        }

        string rreg = roperand->GenerateRSICV();
        if ((op == OpType::AND || op == OpType::OR) && rtype == ValueType::FLOAT)
        {
            string zero_reg;
            CheckAndSetRegister_FLOAT(zero_reg);
            string result_reg;
            CheckAndSetRegister_INT(result_reg);
            output << "\tfmv.s.x " << zero_reg << ",zero\n";
            output << "\tfeq.s " << result_reg << ", " << rreg << ", " << zero_reg << endl;
            output << "\tseqz " << result_reg << ", " << result_reg << endl;
            RestoreRegister(zero_reg);
            RestoreRegister(rreg);
            rreg = result_reg;
            rtype = ValueType::INT;
        }

        if (op == OpType::AND || op == OpType::OR) {
            Symbol_Table::PutAllToMemory(false);
        }

        CheckAndSetRegister_INT(rreg);

        if (GetType() == ValueType::INT)
            target_reg = CheckAndSetRegister_INT(target_reg);
        else if (GetType() == ValueType::FLOAT)
            target_reg = CheckAndSetRegister_FLOAT(target_reg);
        else
            error_sys("In ExprAST::GenerateRSICV: Unknown ValueType!");

        if (ltype == ValueType::INT && rtype == ValueType::INT)
        {
            if (op == OpType::PLUS)
                output << "\taddw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MINUS)
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MUL)
                output << "\tmulw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::DIV)
                output << "\tdivw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MOD)
                output << "\tremw " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::EQ)
            {
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tseqz " << target_reg << ", " << target_reg << endl;
            }
            else if (op == OpType::NEQ)
            {
                output << "\tsubw " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tsnez " << target_reg << ", " << target_reg << endl;
            }
            else if (op == OpType::LE)
                output << "\tslt " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::GE)
                output << "\tsgt " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::LEQ)
            {
                output << "\tsgt " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\txori " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::GEQ)
            {
                output << "\tslt " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\txori " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::AND)
            {
                /*output << "\tsnez " << lreg << ", " << lreg << endl;
                if (lreg != rreg)
                    output << "\tsnez " << rreg << ", " << rreg << endl;
                output << "\tand " << target_reg << ", " << lreg << ", " << rreg << endl;*/
                output << "\tsnez " << target_reg << ", " << rreg << endl;
            }
            else if (op == OpType::OR)
            {
                //output << "\tor " << target_reg << ", " << lreg << ", " << rreg << endl;
                output << "\tsnez " << target_reg << ", " << rreg << endl;
            }
            else
                error_sys("In ExprAST::GenerateRSICV: Operator not supported");

            //确保target_reg不等于lreg和rreg
            if (target_reg == lreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
            if (target_reg == rreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == rreg");
            RestoreRegister(lreg);
            RestoreRegister(rreg);
        }
        else//有一个是float
        {
            //类型转换
            if (ltype != ValueType::FLOAT || rtype != ValueType::FLOAT)
            {
                if (loperand->GetType() == ValueType::INT)
                {
                    lreg = TypeConvert(ValueType::INT, lreg, ValueType::FLOAT);
                }
                else // roperand是INT
                {
                    rreg = TypeConvert(ValueType::INT, rreg, ValueType::FLOAT);
                }
            }

            if (op == OpType::PLUS)
                output << "\tfadd.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MINUS)
                output << "\tfsub.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::MUL)
                output << "\tfmul.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::DIV)
                output << "\tfdiv.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::EQ)
            {
                output << "\tfeq.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            }
            else if (op == OpType::NEQ)
            {
                string temp_reg;
                CheckAndSetRegister_FLOAT(temp_reg);
                output << "\tfsub.s " << temp_reg << ", " << lreg << ", " << rreg << endl;

                string zero_reg;
                CheckAndSetRegister_FLOAT(zero_reg);
                output << "\tfmv.s.x " << zero_reg << ",zero\n";
                output << "\tfeq.s " << target_reg << ", " << temp_reg << ", " << zero_reg << endl;
                output << "\tseqz " << target_reg << ", " << target_reg << endl;
                RestoreRegister(temp_reg);
                RestoreRegister(zero_reg);
            }
            else if (op == OpType::LE)
                output << "\tflt.s " << target_reg << ", " << lreg << ", " << rreg << endl;
            else if (op == OpType::GE)
                output << "\tflt.s " << target_reg << ", " << rreg << ", " << lreg << endl;
            else if (op == OpType::LEQ)
            {
                output << "\tfle.s " << target_reg << ", " << lreg << ", " << rreg << endl;
                //output << "\tfxori.s " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else if (op == OpType::GEQ)
            {
                output << "\tfle.s " << target_reg << ", " << rreg << ", " << lreg << endl;
                //output << "\tfxori.s " << target_reg << ", " << target_reg << ", 1" << endl;
            }
            else
                error_sys("In ExprAST::GenerateRSICV: Operator not supported");

            if (target_reg == lreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == lreg");
            if (target_reg == rreg)
                error_sys("In ExprAST::GenerateRSICV: target_reg == rreg");
            RestoreRegister(lreg);
            RestoreRegister(rreg);
        }

        if (!short_circuit_label.empty())
        {
            string skip_short_circuit_label = GetLabel(++cnt);
            output << "\tj " << skip_short_circuit_label << endl;
            output << short_circuit_label << ":\n";
            output << "\tmv " << target_reg << ", " << lreg << endl;
            output << skip_short_circuit_label << ":\n";
        }

    }
    return target_reg;
}


SingleVarDef::SingleVarDef(unique_ptr<string> _name) :name(move(_name)) {}
SingleVarDef::SingleVarDef(unique_ptr<string> _name, unique_ptr<DeclAST> _init_value) :
    name(move(_name)), init_value(move(_init_value)) {}

string SingleVarDef::GetName() const
{
    return *name;
}

bool SingleVarDef::IsSingle() const
{
    return true;
}

void SingleVarDef::Dump() const
{
    cout << "SingleVarDef { name: " << *name << ", value: ";
    if (init_value)
        init_value->Dump();
    cout << ", use_cnt: " << use_cnt;
    cout << " }";
}

string SingleVarDef::GenerateRSICV(string target_reg)
{
    if (is_def)
    {
        if (VarDefAST::is_current_global)//没有初始化的全局变量设为0
        {
            if (!init_value)//int a;
            {
                if (VarDefAST::current_var_type == ValueType::INT)
                    init_value = make_unique<ValueAST>(0);
                else if (VarDefAST::current_var_type == ValueType::FLOAT)
                    init_value = make_unique<ValueAST>(0.0f);
                else
                    error_sys("In SingleVarDef::GenerateRSICV(): Unknown type of global variable.");
            }

            // int a = xxx;
            auto result = init_value->Evaluate();
            if (result.first != EvaluateType::can)
                error_sys("In SingleVarDef::GenerateRSICV(): Global Variable: Cannot evaluate the expression.");
            //output << "\t.globl\n";
            output << *name << ": .word ";
            if (VarDefAST::current_var_type == ValueType::INT)
                output << result.second.int_val << endl;
            else if (VarDefAST::current_var_type == ValueType::FLOAT)
                output << "0x" << DecimalFloatToHex(result.second.float_val) << endl;
            else
                error_sys("In SingleVarDef::GenerateRSICV(): Unknown type of global variable.");

        }
        else
        {
            if (init_value)//int a = xxx;
            {
                target_reg = init_value->GenerateRSICV(target_reg);
                ValueType ltype = Symbol_Table::GetType(*name);
                if (ltype != init_value->GetType())
                    target_reg = TypeConvert(init_value->GetType(), target_reg, ltype);
                /*if (O1_optimize && rank < 8)
                {
                    string extra_reg;
                    if(CheckAndSetRegister_EXTRA(extra_reg))
                    {
                        output << "\tmv " << extra_reg << ", " << target_reg << endl;
                        Symbol_Table::SetRegister(*name, extra_reg);
                        RestoreRegister(target_reg);
                        target_reg = extra_reg;

                    }
                    else
                    {
                        Symbol_Table::SetRegister(*name, target_reg);
                        RestoreRegister(target_reg, Symbol_Table::GetFreq(*name));
                    }
                }
                else*/
                {
                    Symbol_Table::SetRegister(*name, target_reg);
                    RestoreRegister(target_reg, Symbol_Table::GetFreq(*name));
                }
            }
            else//int a;
            {

            }
        }
    }
    else if (init_value)//a = xxx;
    {
        string current_reg = Symbol_Table::GetRegister(*name);
        if(O1_optimize && current_reg[0] == 's')
        {
            target_reg = init_value->GenerateRSICV();
            ValueType ltype = Symbol_Table::GetType(*name);
            if (ltype != init_value->GetType())
                target_reg = TypeConvert(init_value->GetType(), target_reg, ltype);
            output << "\tmv " << current_reg << ", " << target_reg << endl;
            RestoreRegister(target_reg);
        }
        else
        {
            target_reg = init_value->GenerateRSICV(target_reg);
            ValueType ltype = Symbol_Table::GetType(*name);
            if (ltype != init_value->GetType())
                target_reg = TypeConvert(init_value->GetType(), target_reg, ltype);
            Symbol_Table::SetRegister(*name, target_reg);
            RestoreRegister(target_reg, Symbol_Table::GetFreq(*name));
        }
    }
    else// a;
    {
        bool tag = target_reg.empty();
        target_reg = Symbol_Table::LoadVariableToRegister(*name, Symbol_Table::GetType(*name) == ValueType::FLOAT, target_reg);
        if (tag)
        {
            if (target_reg[0] == 'a' || (target_reg[0] == 'f' && target_reg[1] == 'a'))
            {
                string new_reg;
                if (target_reg[0] == 'a')
                {
                    CheckAndSetRegister_INT(new_reg);
                    output << "\tmv " << new_reg << ", " << target_reg << endl;
                }
                else
                {
                    CheckAndSetRegister_FLOAT(new_reg);
                    output << "\tfmv.s " << new_reg << ", " << target_reg << endl;
                }
                RestoreRegister(target_reg);
                target_reg = new_reg;
                Symbol_Table::SetRegister(*name, new_reg);
            }
        }

        if (Symbol_Table::GetType(*name) == ValueType::INT)
            avail_register_int.erase(target_reg);
        else
            avail_register_float.erase(target_reg);
    }

    return target_reg;
}

int SingleVarDef::GetSize() const
{
    return 4;
}
ArrayVarDef::ArrayVarDef(unique_ptr<string> _name) : name(move(_name))
{
    //memset funccall
    FuncCallAST::funccall_cnt += 1;
}
ArrayVarDef::ArrayVarDef(unique_ptr<string> _name, vector<shared_ptr<DeclAST>> _dimension) :
    name(move(_name)), dimension(_dimension)
{
    FuncCallAST::funccall_cnt += 1;
}
ArrayVarDef::ArrayVarDef(unique_ptr<string> _name, vector<shared_ptr<DeclAST>> _dimension, unique_ptr<DeclAST> init_value) :
    name(move(_name)), dimension(_dimension), init_value(move(init_value))
{
    FuncCallAST::funccall_cnt += 1;
}
string ArrayVarDef::GetName() const
{
    return *name;
}

bool ArrayVarDef::IsSingle() const
{
    return false;
}
string ArrayVarDef::LoadShiftToRegister(vector<int>& dim, string shift_reg) {
    // int a[x][y][z]
    // a[m][n][o] (m*y*z)+(n*z)+o
    for (int j = (int)dim.size() - 2; j >= 0; --j) {
        dim[j] *= dim[j + 1];
    }

    shared_ptr<DeclAST> add_dim = dimension.back();
    string temp_reg = add_dim->GenerateRSICV();
    string add_dim_reg;
    CheckAndSetRegister_INT(add_dim_reg);
    output << "\tmv " << add_dim_reg << ", " << temp_reg << endl;
    RestoreRegister(temp_reg);

    int add_dim_address = Symbol_Table::avail_memory.back().back();
    Symbol_Table::avail_memory.back().pop_back();
    Symbol_Table::PutRegisterToMemory(add_dim_reg);

    for (int i = 0; i < dim.size() - 1; ++i)
    {
        //if(O1_optimize)//Tag: 会导致87_many_params错误
        //{
        //    if(avail_register_int.size() >= 3)
        //    {
        //        shared_ptr<ExprAST> mul_dim = make_shared<ExprAST>(OpType::MUL, dimension[i], make_shared<ValueAST>(dim[i + 1]));
        //        mul_dim->value_type = ValueType::INT;
        //        string mul_dim_reg = mul_dim->GenerateRSICV();

        //        output << "\tadd " << add_dim_reg << ", " << add_dim_reg << ", " << mul_dim_reg << endl;

        //        RestoreRegister(mul_dim_reg);
        //    }
        //    else
        //    {
        //        PrintCommandRelativeToSP("sw", add_dim_reg, add_dim_address);
        //        RestoreRegister(add_dim_reg);

        //        shared_ptr<ExprAST> mul_dim = make_shared<ExprAST>(OpType::MUL, dimension[i], make_shared<ValueAST>(dim[i + 1]));
        //        mul_dim->value_type = ValueType::INT;
        //        string mul_dim_reg = mul_dim->GenerateRSICV();

        //        add_dim_reg = "";
        //        CheckAndSetRegister_INT(add_dim_reg);

        //        PrintCommandRelativeToSP("lw", add_dim_reg, add_dim_address);
        //        output << "\tadd " << add_dim_reg << ", " << add_dim_reg << ", " << mul_dim_reg << endl;

        //        RestoreRegister(mul_dim_reg);
        //    }
        //}
        //else
        {
            //cout << O1_optimize << ' ';
            PrintCommandRelativeToSP("sw", add_dim_reg, add_dim_address);
            RestoreRegister(add_dim_reg);

            shared_ptr<ExprAST> mul_dim = make_shared<ExprAST>(OpType::MUL, dimension[i], make_shared<ValueAST>(dim[i + 1]));
            mul_dim->value_type = ValueType::INT;
            string mul_dim_reg = mul_dim->GenerateRSICV();

            add_dim_reg = "";
            CheckAndSetRegister_INT(add_dim_reg);

            PrintCommandRelativeToSP("lw", add_dim_reg, add_dim_address);
            output << "\tadd " << add_dim_reg << ", " << add_dim_reg << ", " << mul_dim_reg << endl;

            RestoreRegister(mul_dim_reg);
        }
    }

    CheckAndSetRegister_INT(shift_reg);
    if (dim[0] > 0) { // 当前函数定义的数组
        output << "\tslli " << shift_reg << ", " << add_dim_reg << ", 2" << endl;
        RestoreRegister(add_dim_reg);
        // Symbol_Table::PutRegisterToMemory(start_reg, false);
        if (Symbol_Table::GetInfo(*name).is_global) {
            output << "\tlla s0, " << *name << endl;
            output << "\tadd " << shift_reg << ", " << shift_reg << ", s0" << endl;
        }
        else {
            int start_address = Symbol_Table::GetAddress(*name);
            output << "\tli s0, " << start_address << endl;
            output << "\tadd " << shift_reg << ", " << shift_reg << ", s0" << endl;
            output << "\tadd " << shift_reg << ", " << shift_reg << ", sp" << endl;
        }
    }
    else { //函数内部使用数组
        output << "\tslli " << shift_reg << ", " << add_dim_reg << ", 2" << endl;
        RestoreRegister(add_dim_reg);
        string start_reg;
        start_reg = Symbol_Table::LoadVariableToRegister(GetName(), false);
        if (start_reg[0] == 'a') {
            string new_reg;
            CheckAndSetRegister_INT(new_reg);
            output << "\tmv " << new_reg << ", " << start_reg << endl;
            RestoreRegister(start_reg);
            start_reg = new_reg;
            Symbol_Table::SetRegister(*name, new_reg);
        }
        avail_register_int.erase(start_reg);
        output << "\tadd " << shift_reg << ", " << shift_reg << ", " << start_reg << endl;
        RestoreRegister(start_reg);
    }

    return shift_reg;

}

string ArrayVarDef::GetIdentifier(int index) {
    return (*name) + "$" + to_string(index);
}

void ArrayVarDef::Dump() const {
    cout << "ArrayVarDef {";
    if (dimension.size()) {
        cout << ", dimension: {";
        int n = dimension.size();
        for (int i = 0; i < n; ++i) {
            dimension[i]->Dump();
        }
        cout << "}";
    }
    if (init_value)
        init_value->Dump();
    else
    {
        for (auto& i : init_values)
            i->Dump();
    }
    cout << "} ";
}
void ArrayVarDef::GlobalParamenterInitialize(string& target_reg) {
    output << *name << ": " << endl;

        for (int i = 0; i < init_values.size(); ++i) {
            auto result = init_values[i]->Evaluate();
            if (result.first != EvaluateType::can)
                error_sys("In SingleVarDef::GenerateRSICV(): Global Variable: Cannot evaluate the expression.");
            if (VarDefAST::current_var_type == ValueType::INT)
                output << "\t.word " << result.second.int_val << endl;
            else if (VarDefAST::current_var_type == ValueType::FLOAT)
                output << "\t.word " << DecimalFloatToHex(result.second.float_val) << endl;
            else
                error_sys("In SingleVarDef::GenerateRSICV(): Unknown type of global variable.");
        }

    if (zero_cnt != 0) output << "\t.zero " << zero_cnt * 4 << endl;
}
void ArrayVarDef::LocalParameterInitialize(string& target_reg) {
    int start_address = Symbol_Table::GetAddress(*name);
    ValueType ltype = Symbol_Table::GetType(*name);
    {
        auto temp_params = new vector<shared_ptr<DeclAST>>;
        auto param = make_shared<ValueAST>(make_shared<ArrayVarDef>(make_unique<string>(*name)));
        param->value_type = ltype;
        temp_params->push_back(param);
        param = make_shared<ValueAST>(int(init_values.size()));
        temp_params->push_back(param);
        if (ltype == ValueType::INT)
        {
            auto memset_call = FuncCallAST(make_unique<string>("scut_memset_int"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
            memset_call.func_def = FuncDefAST::memset_int;
            RestoreRegister(memset_call.GenerateRSICV());
        }
        else
        {
            auto memset_call = FuncCallAST(make_unique<string>("scut_memset_float"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
            memset_call.func_def = FuncDefAST::memset_float;
            RestoreRegister(memset_call.GenerateRSICV());
        }
    }
    for (int i = 0; i < init_values.size() - zero_cnt; ++i) {
        target_reg = init_values[i]->GenerateRSICV();
        if (ltype != init_values[i]->GetType()) {
            target_reg = TypeConvert(init_values[i]->GetType(), target_reg, ltype);
        }

        if (ltype == ValueType::INT) {
            PrintCommandRelativeToSP("sw", target_reg, start_address + 4 * i);
            //output << "\tsw " << target_reg << ", " << start_address + 4 * i << "(sp)" << endl;
        }
        else if (ltype == ValueType::FLOAT) {
            PrintCommandRelativeToSP("fsw", target_reg, start_address + 4 * i);
            //output << "\tfsw " << target_reg << ", " << start_address + 4 * i << "(sp)" << endl;
        }
        RestoreRegister(target_reg);
    }

    /*for (int i = init_values.size() - zero_cnt; i < init_values.size(); ++i) {
        output << "\tsw zero, " << start_address + 4 * i << "(sp)" << endl;
    }*/
}
void ArrayVarDef::Assignment(string& target_reg) {
    vector<int> dim = Symbol_Table::GetDimension(*name);
    target_reg = init_value->GenerateRSICV();
    Symbol_Table::PutRegisterToMemory(target_reg);
    avail_register_int.erase(target_reg);
    ValueType ltype = Symbol_Table::GetType(*name);
    if (ltype != init_value->GetType())
        target_reg = TypeConvert(init_value->GetType(), target_reg, ltype);
    string shift_reg = LoadShiftToRegister(dim);
    if (ltype == ValueType::INT)
        output << "\tsw " << target_reg << ", 0(" << shift_reg << ")\n";
    else if (ltype == ValueType::FLOAT)
        output << "\tfsw " << target_reg << ", 0(" << shift_reg << ")\n";
    RestoreRegister(shift_reg);
    RestoreRegister(target_reg);
}
void ArrayVarDef::FuncCallParameter(string& target_reg, vector<int>& dim) {
    CheckAndSetRegister_INT(target_reg);
    if (dimension.empty()) { // func(a)
        string& start_reg = target_reg;
        if (Symbol_Table::GetInfo(*name).is_global) { // 是全局变量
            output << "\tlla " << start_reg << ", " << *name << endl;
        }
        else {
            if (dim[0] > 0) { // 数组在当前函数内定义
                int start_address = Symbol_Table::GetAddress(*name);
                output << "\tli " << start_reg << ", " << start_address << endl;
                output << "\tadd " << start_reg << ", " << start_reg << ", sp" << endl;
            }
            else { // 函数内再次调用数组，且数组是当前函数的参数
                Symbol_Table::LoadVariableToRegister(*name, false, start_reg);
            }
        }
        //RestoreRegister(start_reg);
    }
    else {
        // Tag: !!!!!!!!!!!!!!!!
        // TODO: !!!!!!!!!!!
        // int a[2][3][4][5]

        // func(a[1][2][3])
        for (int j = (int)dim.size() - 2; j >= 0; --j) {
            dim[j] *= dim[j + 1];
        }
        string& start_reg = target_reg;
        if (Symbol_Table::GetInfo(*name).is_global) { // 是全局变量
            output << "\tlla " << start_reg << ", " << *name << endl;
        }
        else {
            int start_address = Symbol_Table::GetAddress(*name);
            output << "\tli " << start_reg << ", " << start_address << endl;
            if (dim[0] > 0) { // 数组在当前函数内定义
                output << "\tadd " << start_reg << ", " << start_reg << ", sp" << endl;
            }
            else { // 函数内再次调用数组，且数组是当前函数的参数
                Symbol_Table::LoadVariableToRegister(*name, false, start_reg);
            }
        }
        string shift_reg, dim_reg;
        vector<shared_ptr<DeclAST>> mul_dim;
        for (int j = 0; j < dimension.size(); ++j) {
            shared_ptr<ExprAST> temp_mul = make_shared<ExprAST>(OpType::MUL, dimension[j], make_shared<ValueAST>(dim[j + 1]));
            temp_mul->value_type = ValueType::INT;
            mul_dim.push_back(temp_mul);
        }
        if (mul_dim.size() >= 2) {
            shared_ptr<DeclAST> add_dim = mul_dim.at(0);
            for (int i = 1; i < mul_dim.size(); ++i) {
                auto temp_add = make_shared<ExprAST>(OpType::PLUS, add_dim, mul_dim.at(i));
                temp_add->value_type = ValueType::INT;
                add_dim = temp_add;
            }
            dim_reg = add_dim->GenerateRSICV();
        }
        else {
            dim_reg = mul_dim[0]->GenerateRSICV();
        }
        CheckAndSetRegister_INT(shift_reg);
        output << "\tslli " << shift_reg << ", " << dim_reg << ", 2" << endl;
        output << "\tadd " << shift_reg << ", " << shift_reg << ", " << start_reg << endl;
        output << "\tmv " << target_reg << ", " << shift_reg << endl;
        //RestoreRegister(start_reg);
        RestoreRegister(dim_reg);
        RestoreRegister(shift_reg);
    }
}
void ArrayVarDef::Rval(string& target_reg, vector<int>& dim) {
    ValueType type = Symbol_Table::GetType(*name);
    string shift_reg = LoadShiftToRegister(dim);
    if (type == ValueType::INT) CheckAndSetRegister_INT(target_reg);
    else CheckAndSetRegister_FLOAT(target_reg);
    if (type == ValueType::INT)
        output << "\tlw " << target_reg << ", 0(" << shift_reg << ")\n";
    else if (type == ValueType::FLOAT)
        output << "\tflw " << target_reg << ", 0(" << shift_reg << ")\n";
    RestoreRegister(shift_reg);
}
string ArrayVarDef::GenerateRSICV(string target_reg) {
    if (is_def) {
        if (VarDefAST::is_current_global) { //全局数组定义
            GlobalParamenterInitialize(target_reg);
            return target_reg;
        }
        else { //局部数组定义
            if (list) { //列表初始化
                LocalParameterInitialize(target_reg);
                return target_reg;
            }
            else { //未初始化局部变量数组
                return target_reg;
            }
        }
    }
    else {
        if (init_value) { // a[i] = xxx;
            Assignment(target_reg);
            return target_reg;
        }
        else { //a[1]
            vector<int> dim = Symbol_Table::GetDimension(*name);
            if (dim.size() != dimension.size()) {
                FuncCallParameter(target_reg, dim);
            }
            else {
                Rval(target_reg, dim);
            }
            return target_reg;
        }
    }

}
int ArrayVarDef::GetSize() const {
    if (init_values.size()) {
        int sz = 1;
        int n = dimension.size();
        for (int j = 0; j < n; ++j) {
            auto rs = dimension.at(j)->Evaluate();
            if (rs.first == EvaluateType::can) {
                Value value = rs.second;
                ValueType TMP_Type = dimension.at(j)->GetType() == ValueType::IDENT ?
                    Symbol_Table::GetType(dimension.at(j)->GetName()) : dimension.at(j)->GetType();
                // TODO:                     
                if (TMP_Type == ValueType::FLOAT) {
                    sz *= (int)value.float_val;
                }
                else if (TMP_Type == ValueType::INT) {
                    sz *= value.int_val;
                }
                else {

                }
            }
            else {
                error_sys("dimension can not be calculated");
            }
        }
        return sz * 4;
    }
    else {
        return 8;
    }
}
VarDefAST::VarDefAST(unique_ptr<string> _name)
{
    FloatPool::label_used.insert(*_name);
    var_defs.push_back(make_shared<SingleVarDef>(move(_name)));
}
VarDefAST::VarDefAST(shared_ptr<VarDef> var)
{
    FloatPool::label_used.insert(var->GetName());
    var_defs.push_back(var);
}

void VarDefAST::Append(shared_ptr<VarDefAST> other)
{
    var_defs.insert(var_defs.end(), std::make_move_iterator(other->var_defs.begin()), std::make_move_iterator(other->var_defs.end()));
    other->var_defs.clear();
}

void VarDefAST::Dump() const
{
    cout << "VarDefAST { type: " << ToString(type);
    for (size_t i = 0; i < var_defs.size(); ++i)
    {
        cout << "{ name: ";
        cout << var_defs[i]->GetName() << ", ";
        if (var_defs[i]->IsSingle())
        {
            dynamic_pointer_cast<SingleVarDef>(var_defs[i])->Dump();
        }
        else
        {
            cout << ", ArrayVarDef: ";
            auto var_def_ptr = dynamic_pointer_cast<ArrayVarDef>(var_defs[i]);
            auto& dimension = var_def_ptr->dimension;
            cout << "dimension: { ";
            int n = dimension.size();

            for (int j = 0; j < n; ++j) {
                auto rs = dimension.at(j)->Evaluate();
                if (rs.first == EvaluateType::can) {
                    Value value = rs.second;
                    ValueType _TMP_Type = dimension.at(j)->GetType();
                    int tmp_;
                    if (_TMP_Type == ValueType::INT) {
                        tmp_ = value.int_val;
                    }
                    else {
                        tmp_ = value.float_val;
                    }
                    if (j != n - 1)
                        cout << tmp_ << ", ";
                    else
                        cout << tmp_ << " }";
                }
                else {
                    error_sys("dimension can not be calculated");
                }
            }
            cout << ", init_values: ";
            auto& list = var_def_ptr->list;
            if (list) {
                // preorder(list, 0);
                // var_def_ptr->init_values.resize(dimensions.back());
                cout << "{ ";

                // TODO:
                // 变量初始化
                for (auto& v : var_def_ptr->init_values) {
                    if (v)
                        dynamic_cast<ValueAST*>(v.get())->Dump();
                    else
                        cout << 0 << ' ';
                    /*if (v->GetType() == ValueType::INT)
                    cout << dynamic_cast<ValueAST*>(v.get())->value.int_val << " ";
                    if (v->GetType() == ValueType::FLOAT)
                    cout << dynamic_cast<ValueAST*>(v.get())->value.float_val << " ";*/
                }

                cout << "}";
            }
            else {
                // 非全局变量未初始化
                cout << "<NULL>";
            }
        }
        cout << " }";
        if (i != var_defs.size() - 1)
            cout << ", ";
    }

    /*
    cout << "ArrayVarDef: ";

    for (size_t i = 0; i < var_array.size(); ++i)
    {
        cout << "{ name: ";
        cout << var_single[i].name << ", ";
        cout << "init_value: ";
        var_single[i].init_value->Dump();
        cout << "}";
        if (i != var_single.size() - 1)
            cout << ", ";
    }
    */

    cout << ", is_global: " << ToString(is_global);
    cout << ", is_const: " << ToString(is_const);

    cout << " }";
}

string VarDefAST::GenerateRSICV(string target_reg)
{
    LABEL_GUARD;
    bool prev_bool = VarDefAST::is_current_global;
    VarDefAST::is_current_global = is_global;
    VarDefAST::current_var_type = type;

    Symbol_Table::Insert(*this);

    for (size_t i = 0; i < var_defs.size(); ++i)
    {
        var_defs.at(i)->is_def = true;
        RestoreRegister(var_defs.at(i)->GenerateRSICV());
    }

    VarDefAST::is_current_global = prev_bool;
    VarDefAST::current_var_type = ValueType::UNKNOWN;
    return target_reg;
}

int VarDefAST::GetSize() const
{
    int size = 0;
    for (size_t i = 0; i < var_defs.size(); ++i)
        size += var_defs.at(i)->GetSize();
    return size;
}

ASTNodeType VarDefAST::GetNodeType() const
{
    return ASTNodeType::VARDEF;
}

AssignmentAST::AssignmentAST(unique_ptr<ValueAST> _lval_ref, unique_ptr<DeclAST> _init_value) :lval_ref(move(_lval_ref))
{
    if (lval_ref->var_def->IsSingle())
        dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def)->init_value = move(_init_value);
    else
        dynamic_pointer_cast<ArrayVarDef>(lval_ref->var_def)->init_value = move(_init_value);
    /*if(lval_ref->dimension.empty())
    {
        dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def)->init_value = move(_init_value);
    }
    else {
        lval_ref->var_def = make_shared<ArrayVarDef>(make_unique<string>(lval_ref->GetName()), lval_ref->dimension, move(_init_value));
    }*/
}

void AssignmentAST::Dump() const
{
    // if (!dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def)->init_value)
    //     error_sys("In AssignmentAST::Dump(): init_value is null");
    cout << "AssignmentAST { name: ";
    cout << lval_ref->GetName();

    cout << ", value: ";
    if (lval_ref->var_def->IsSingle())
    {
        dynamic_pointer_cast<SingleVarDef>(lval_ref->var_def)->init_value->Dump();
    }
    else
    {
        lval_ref->var_def->Dump();
    }
    cout << " }";
}

ASTNodeType AssignmentAST::GetNodeType() const
{
    return ASTNodeType::ASSIGN;
}

ValueType AssignmentAST::GetLType() {
    return lval_type;
}

string AssignmentAST::GenerateRSICV(string target_reg)
{
    LABEL_GUARD;

    target_reg = lval_ref->GenerateRSICV(target_reg);
    RestoreRegister(target_reg);
    return target_reg;
}

BlockAST::BlockAST()
{
    stmts = make_unique<vector<shared_ptr<StmtAST>>>();
}

BlockAST::BlockAST(shared_ptr<StmtAST> stmt) :stmts(make_unique<vector<shared_ptr<StmtAST>>>())
{
    stmts->push_back(stmt);
}

BlockAST::BlockAST(unique_ptr<vector<shared_ptr<StmtAST>>> _stmts) : stmts(move(_stmts)) {}

void BlockAST::simplify_one_array(int i) {
    auto whileAST = dynamic_pointer_cast<WhileAST>(stmts->at(i));
    auto expr = dynamic_pointer_cast<ExprAST>(whileAST->cond);
    if (!expr) return;
    auto while_block = dynamic_pointer_cast<BlockAST>(whileAST->block);
    if (!while_block) return;
    if (while_block->stmts->at(0)->GetNodeType() != ASTNodeType::ASSIGN
    || while_block->stmts->at(1)->GetNodeType() != ASTNodeType::ASSIGN) return;
    shared_ptr<AssignmentAST> assign = dynamic_pointer_cast<AssignmentAST>(while_block->stmts->at(0));
    if ((assign->lval_ref->var_def->IsSingle())) return;
    string name;
    auto var = dynamic_pointer_cast<ArrayVarDef>(assign->lval_ref->var_def);
    auto& init_value = var->init_value;
    if (!init_value) return;
    if (init_value->GetType() == ValueType::INT) {
        if (dynamic_pointer_cast<ValueAST>(init_value) && !dynamic_pointer_cast<ValueAST>(init_value)->var_def && dynamic_pointer_cast<ValueAST>(init_value)->value.int_val == 0) {
            if (var->dimension.empty()) return;
            auto value = dynamic_pointer_cast<ValueAST>(var->dimension.back());
            if (value) {
                auto item = dynamic_pointer_cast<SingleVarDef>(value->var_def);
                name = item->GetName();
            }
        }
    } else {
        if (dynamic_pointer_cast<ValueAST>(init_value) && !dynamic_pointer_cast<ValueAST>(init_value)->var_def && dynamic_pointer_cast<ValueAST>(init_value)->value.float_val == 0) {
            if (var->dimension.empty()) return;
            auto value = dynamic_pointer_cast<ValueAST>(var->dimension.back());
            if (value) {
                auto item = dynamic_pointer_cast<SingleVarDef>(value->var_def);
                name = item->GetName();
            }
        }
    }
    auto new_assign = dynamic_pointer_cast<AssignmentAST>(while_block->stmts->at(1));
    auto new_var = dynamic_pointer_cast<SingleVarDef>(new_assign->lval_ref->var_def);
    if (!new_var) return;
    if (name != new_var->GetName()) return;
    auto& new_init_value = new_var->init_value;
    if (!new_init_value) return;
    auto new_expr = dynamic_pointer_cast<ExprAST>(new_init_value);
    if (!new_expr) return;
    if (new_expr->op == OpType::PLUS 
    && new_expr->loperand->GetName() == new_var->GetName() 
    && dynamic_pointer_cast<ValueAST>(new_expr->roperand) 
    && dynamic_pointer_cast<ValueAST>(new_expr->roperand)->GetType() == ValueType::INT
    && !dynamic_pointer_cast<ValueAST>(new_expr->roperand)->var_def
    && dynamic_pointer_cast<ValueAST>(new_expr->roperand)->value.int_val == 1) {
        // expr
        if (expr->op == OpType::LE
        && dynamic_pointer_cast<ValueAST>(expr->loperand) 
        && dynamic_pointer_cast<ValueAST>(expr->loperand)->GetName() == name
        && dynamic_pointer_cast<ValueAST>(expr->roperand)) {
            if (stmts->at(i - 1)->GetNodeType() != ASTNodeType::ASSIGN) return;
            {
                shared_ptr<AssignmentAST> assign = dynamic_pointer_cast<AssignmentAST>(stmts->at(i - 1));
                auto var = dynamic_pointer_cast<SingleVarDef>(assign->lval_ref->var_def);
                if (!var) return;
                if (var->GetName() != name) return;
                auto& init_value = var->init_value;
                bool flag = false;
                if (init_value->GetType() == ValueType::INT) {
                    if (dynamic_pointer_cast<ValueAST>(init_value) && !dynamic_pointer_cast<ValueAST>(init_value)->var_def && dynamic_pointer_cast<ValueAST>(init_value)->value.int_val == 0) {
                        flag = true;
                    }
                } else {
                    if (dynamic_pointer_cast<ValueAST>(init_value) && !dynamic_pointer_cast<ValueAST>(init_value)->var_def && dynamic_pointer_cast<ValueAST>(init_value)->value.float_val == 0) {
                        flag = true;
                    }
                }
                if (!flag) return;
            }
            auto temp_params = new vector<shared_ptr<DeclAST>>;
            var->dimension.pop_back();
            ValueType ltype = Symbol_Table::GetType(var->GetName());
            shared_ptr<ArrayVarDef> array_var_def = make_shared<ArrayVarDef>(make_unique<string>(var->GetName()), var->dimension);
            auto param1 = make_shared<ValueAST>(array_var_def);
            param1->value_type = ValueType::INT;
            temp_params->push_back(param1);
            auto param2 = expr->roperand;
            temp_params->push_back(param2);
            if (ltype == ValueType::INT) {
                auto memset_call = make_shared<FuncCallAST>(make_unique<string>("scut_memset_int"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
                memset_call->func_def = FuncDefAST::memset_int;
                shared_ptr<IfAST> ifAST = make_shared<IfAST>(expr, memset_call);
                stmts->at(i) = ifAST;
            } else {
                auto memset_call = make_shared<FuncCallAST>(make_unique<string>("scut_memset_float"), unique_ptr<vector<shared_ptr<DeclAST>>>(temp_params));
                memset_call->func_def = FuncDefAST::memset_float;
                shared_ptr<IfAST> ifAST = make_shared<IfAST>(expr, memset_call);
                stmts->at(i) = ifAST;
            }
        }
    }
    else return;
}
void BlockAST::simplify_memset()
{
    if (stmts->size() < 2) return;
    for (int i = 0; i < stmts->size(); ++i) {
        switch(stmts->at(i)->GetNodeType()) {
            case ASTNodeType::WHILE: {
                auto whileAST = dynamic_pointer_cast<WhileAST>(stmts->at(i));
                auto expr = dynamic_pointer_cast<ExprAST>(whileAST->cond);
                if (!expr) return;
                auto while_block = dynamic_pointer_cast<BlockAST>(whileAST->block);
                if (!while_block) return;
                // 一维数组的 memset 优化
                if (while_block->stmts->size() != 2) return;
                simplify_one_array(i);
                // cout << "Hello" << endl;
            }
            break;
        }
    }
}

void BlockAST::Dump() const {
    cout << "BlockAST { ";

    if (!stmts)
        return;
    for (auto i = stmts->begin(); i != stmts->end(); ++i)
    {
        (*i)->Dump();
        if (i != stmts->end() - 1)
            cout << ", ";
    }
    cout << "}";
}

ASTNodeType BlockAST::GetNodeType() const
{
    return ASTNodeType::BLOCK;
}

string BlockAST::GenerateRSICV(string target_reg)
{
    /*if (!stmts->empty() && stmts->back()->GetNodeType() == ASTNodeType::RETURN)
        end_jump_label = "";*/
    if (!stmts->empty() && should_restore)
    {
        stmts->back()->should_restore = should_restore;
        should_restore = false;
    }

    BOTH_GUARD;

    for (size_t i = 0; i < stmts->size(); ++i)
    {
        if (i == 11 && stmts->size() == 21)
        {
            int a = 2;
        }
        RestoreRegister(stmts->at(i)->GenerateRSICV());
    }

    return target_reg;
}

int BlockAST::GetSize() const
{
    int size = 0;
    for (size_t i = 0; i < stmts->size(); ++i)
        size += stmts->at(i)->GetSize();
    return size;
}

ReturnAST::ReturnAST(shared_ptr<DeclAST> _ret_value) : ret_value(move(_ret_value)) {}

void ReturnAST::Dump() const
{
    cout << "ReturnAST { ret_value: ";
    if (ret_value)
        ret_value->Dump();
    cout << " }";
}

ASTNodeType ReturnAST::GetNodeType() const
{
    return ASTNodeType::RETURN;
}

string ReturnAST::GenerateRSICV(string target_reg)
{
    end_jump_label = FuncDefAST::current_return_label;
    LABEL_GUARD;
    if (ret_value) {
        string ret_reg = ret_value->GenerateRSICV();
        if (FuncDefAST::func_type == ValueType::INT)
        {
            Symbol_Table::PutRegisterToMemory("a0", false);
            if (ret_value->GetType() == ValueType::FLOAT)
                output << "\tfmv.x.w " << "a0" << ", " << ret_reg << endl;
            else if (ret_reg != "a0")
                output << "\tmv a0, " << ret_reg << endl;

        }
        else if (FuncDefAST::func_type == ValueType::FLOAT)
        {
            Symbol_Table::PutRegisterToMemory("fa0", true);
            if (ret_value->GetType() == ValueType::INT)
                output << "\tfmv.w.x " << "fa0" << ", " << ret_reg << endl;
            else if (ret_reg != "fa0")
                output << "\tfmv.s fa0, " << ret_reg << endl;
        }
        else
        {
            error_sys("In ReturnAST::GenerateRSICV(): unknown func_type");
        }

        RestoreRegister(ret_reg);
    }
    return target_reg;
}


void ContinueAST::Dump() const
{
    cout << "ContinueAST {}";
}

string ContinueAST::GenerateRSICV(string target_reg)
{
    end_jump_label = continue_label;
    LABEL_GUARD;
    return target_reg;
}

ASTNodeType ContinueAST::GetNodeType() const
{
    return ASTNodeType::CONTINUE;
}

ASTNodeType BreakAST::GetNodeType() const
{
    return ASTNodeType::BREAK;
}

void BreakAST::Dump() const
{
    cout << "BreakAST {}";
}

string BreakAST::GenerateRSICV(string target_reg)
{
    end_jump_label = break_label;
    LABEL_GUARD;
    return target_reg;
}

WhileAST::WhileAST(unique_ptr<DeclAST> _cond, shared_ptr<StmtAST> _block) :
    cond(move(_cond)), block(_block)
{
    if (block && block->GetNodeType() != ASTNodeType::BLOCK)
        block = make_shared<BlockAST>(block);
}

void WhileAST::Dump() const
{
    cout << "WhileAST { condition: ";
    cond->Dump();
    cout << ", block: ";
    block->Dump();
    cout << " }";
}

ASTNodeType WhileAST::GetNodeType() const
{
    return ASTNodeType::WHILE;
}

int WhileAST::GetSize() const
{
    return block->GetSize();
}

string WhileAST::GenerateRSICV(string target_reg)
{
    string prev_break_label = break_label, prev_continue_label = continue_label;
    Symbol_Table::PutAllToMemory(false);

    if (start_label.empty())
        start_label = GetLabel(++cnt);

    string end_label = GetLabel(++cnt);

    BOTH_GUARD;

    target_reg = cond->GenerateRSICV(target_reg);
    if (cond->GetType() != ValueType::INT)
        target_reg = TypeConvert(cond->GetType(), target_reg, ValueType::INT);

    block->end_jump_label = continue_label = start_label;
    break_label = end_label;

    output << "\tbeqz " << target_reg << ", " << end_label << endl;
    RestoreRegister(target_reg);
    block->should_restore = true;
    RestoreRegister(block->GenerateRSICV());

    output << end_label << ":" << endl;

    break_label = prev_break_label;
    continue_label = prev_continue_label;

    return target_reg;
}

IfAST::IfAST(shared_ptr<DeclAST> _cond, shared_ptr<StmtAST> _true) :
    cond(move(_cond)), true_block(move(_true))
{
    if (true_block && true_block->GetNodeType() != ASTNodeType::BLOCK)
        true_block = make_shared<BlockAST>(true_block);
}
IfAST::IfAST(shared_ptr<DeclAST> _cond, shared_ptr<StmtAST> _true, shared_ptr<StmtAST> _false) :
    cond(move(_cond)), true_block(move(_true)), false_block(move(_false))
{
    if (true_block && true_block->GetNodeType() != ASTNodeType::BLOCK)
        true_block = make_shared<BlockAST>(true_block);
    if (false_block && false_block->GetNodeType() != ASTNodeType::BLOCK)
        false_block = make_shared<BlockAST>(false_block);
}

void IfAST::Dump() const
{
    cout << "IfAST { condition: ";
    cond->Dump();
    cout << ", true_block: ";
    true_block->Dump();
    if (false_block)
    {
        cout << ", false_block: ";
        false_block->Dump();
    }
    cout << " }";
}

ASTNodeType IfAST::GetNodeType() const
{
    return ASTNodeType::IF;
}

string IfAST::GenerateRSICV(string target_reg)
{
    LABEL_GUARD;

    target_reg = cond->GenerateRSICV(target_reg);
    if (cond->GetType() != ValueType::INT)
        target_reg = TypeConvert(cond->GetType(), target_reg, ValueType::INT);
    Symbol_Table::PutAllToMemory(false);
    if (!false_block)
    {
        string prev_end_label = GetLabel(++cnt);

        output << "\tbeqz " << target_reg << ", " << prev_end_label << endl;
        RestoreRegister(target_reg);

        {
            SYMBOL_TABLE_GUARD;
            true_block->should_restore = true;
            RestoreRegister(true_block->GenerateRSICV());
        }

        output << prev_end_label << ":\n";
    }
    else
    {
        true_block->end_jump_label = GetLabel(++cnt);
        string prev_end_label = true_block->end_jump_label;

        false_block->start_label = GetLabel(++cnt);
        output << "\tbeqz " << target_reg << ", " << false_block->start_label << endl;
        RestoreRegister(target_reg);
        {
            SYMBOL_TABLE_GUARD;
            true_block->should_restore = true;
            RestoreRegister(true_block->GenerateRSICV());
        }
        if (false_block)
        {
            SYMBOL_TABLE_GUARD;
            false_block->should_restore = true;
            RestoreRegister(false_block->GenerateRSICV());
        }

        output << prev_end_label << ":\n";
    }

    return target_reg;
}

int IfAST::GetSize() const
{
    int size = true_block->GetSize();
    if (false_block)
        size += false_block->GetSize();
    return size;
}

FuncCallAST::FuncCallAST(unique_ptr<string> _name) :name(move(_name))
{
    if (*name == "starttime" || *name == "stoptime")
    {
        name = make_unique<string>("_sysy_" + *name);
        unique_ptr<vector<shared_ptr<DeclAST>>> params = make_unique<vector<shared_ptr<DeclAST>>>();
        if(*name == "_sysy_starttime")
    		params->push_back(make_shared<ValueAST>(++starttime_cnt));
        else
            params->push_back(make_shared<ValueAST>(++stoptime_cnt));
        this->param_vals = move(params);

        funccall_cnt += 1;
    }
}
FuncCallAST::FuncCallAST(unique_ptr<string> _name, unique_ptr<vector<shared_ptr<DeclAST>>> _params) :
    name(move(_name)), param_vals(move(_params))
{
    if (param_vals)
    {
        funccall_cnt += 1;
    }
}

void FuncCallAST::Dump() const
{
    cout << "FuncCallAST { ";
    cout << "name: " << *name;
    if (param_vals != NULL) {
        cout << ", params_value: {";
        for (auto i = param_vals->begin(); i != param_vals->end(); ++i)
        {
            (*i)->Dump();
            if (i != param_vals->end() - 1)
                cout << ", ";
        }
        cout << "}, ";
    }
    cout << " }";
}

pair<EvaluateType, Value> FuncCallAST::Evaluate()
{
    return { canEvaluate, value };
}

string FuncCallAST::GenerateRSICV(string target_reg)
{
    LABEL_GUARD;
    for (size_t i = 0; i < 8; ++i)
    {
        Symbol_Table::PutRegisterToMemory("a" + to_string(i), false);
        Symbol_Table::PutRegisterToMemory("fa" + to_string(i), true);
    }

    if (param_vals) {
        vector<int> param_address(param_vals->size());
        for (size_t i = 0, start_address = 0; i < param_vals->size(); ++i)
        {
            ValueType param_type = func_def->params->at(i)->type;
            ValueType value_type = param_vals->at(i)->GetType();
            if (!func_def->params->at(i)->var_defs.at(0)->IsSingle())
            {
                value_type = ValueType::INT;
                param_type = ValueType::INT;
            }

            string temp_reg = param_vals->at(i)->GenerateRSICV();
            Symbol_Table::PutRegisterToMemory(temp_reg);
            if (param_type != value_type)
                temp_reg = TypeConvert(value_type, temp_reg, param_type);

            if (!func_def->params->at(i)->var_defs.at(0)->IsSingle()) {
                Symbol_Table::avail_memory.back().pop_back();
                start_address = Symbol_Table::avail_memory.back().back();
                Symbol_Table::avail_memory.back().pop_back();
                PrintCommandRelativeToSP("sd", temp_reg, start_address);
            }
            else
            {
                start_address = Symbol_Table::avail_memory.back().back();
                Symbol_Table::avail_memory.back().pop_back();
                if (param_type == ValueType::INT)
                    PrintCommandRelativeToSP("sw", temp_reg, start_address);
                else
                    PrintCommandRelativeToSP("fsw", temp_reg, start_address);
            }
            param_address[i] = start_address;
            RestoreRegister(temp_reg);
        }

        for (size_t i = 0, start_address = 0, extra_param_address = 0; i < param_vals->size(); ++i)
        {
            ValueType param_type = func_def->params->at(i)->type;
            ValueType value_type = param_vals->at(i)->GetType();
            if (!func_def->params->at(i)->var_defs.at(0)->IsSingle())
            {
                value_type = ValueType::INT;
                param_type = ValueType::INT;
            }

            start_address = param_address[i];
            if (i < 8)
            {
                if (!func_def->params->at(i)->var_defs.at(0)->IsSingle())
                {
                    PrintCommandRelativeToSP("ld", "a" + to_string(i), start_address);
                }
                else
                {
                    if (param_type == ValueType::INT)
                        PrintCommandRelativeToSP("lw", "a" + to_string(i), start_address);
                    else
                        PrintCommandRelativeToSP("flw", "fa" + to_string(i), start_address);
                }
            }
            else
            {
                if (!func_def->params->at(i)->var_defs.at(0)->IsSingle())
                {
                    string temp_reg;
                    CheckAndSetRegister_INT(temp_reg);
                    PrintCommandRelativeToSP("ld", temp_reg, start_address);
                    PrintCommandRelativeToSP("sd", temp_reg, extra_param_address);
                    extra_param_address += 8;
                    RestoreRegister(temp_reg);
                }
                else
                {
                    if (param_type == ValueType::INT)
                    {
                        string temp_reg;
                        CheckAndSetRegister_INT(temp_reg);
                        PrintCommandRelativeToSP("lw", temp_reg, start_address);
                        PrintCommandRelativeToSP("sw", temp_reg, extra_param_address);
                        extra_param_address += 4;
                        RestoreRegister(temp_reg);
                    }
                    else
                    {
                        string temp_reg;
                        CheckAndSetRegister_FLOAT(temp_reg);
                        PrintCommandRelativeToSP("flw", temp_reg, start_address);
                        PrintCommandRelativeToSP("fsw", temp_reg, extra_param_address);
                        extra_param_address += 4;
                        RestoreRegister(temp_reg);
                    }
                    start_address += 4;
                }
            }
        }
    }

    //存储因为函数调用而被放到内存的值
    vector<pair<string, int>> saved_val;
    for (int i = 0; i < 7; ++i)
    {
        if (avail_register_int.find("t" + to_string(i)) == avail_register_int.end())
        {
            saved_val.push_back({ "t" + to_string(i), Symbol_Table::avail_memory.back().back() });
            PrintCommandRelativeToSP("sw", "t" + to_string(i), Symbol_Table::avail_memory.back().back());
            Symbol_Table::avail_memory.back().pop_back();
        }
        else
        {
            Symbol_Table::PutRegisterToMemory("t" + to_string(i), false);
        }
    }
    for (int i = 0; i < 12; ++i)
    {
        if (avail_register_float.find("ft" + to_string(i)) == avail_register_float.end())
        {
            saved_val.push_back({ "ft" + to_string(i), Symbol_Table::avail_memory.back().back() });
            PrintCommandRelativeToSP("fsw", "ft" + to_string(i), Symbol_Table::avail_memory.back().back());
            Symbol_Table::avail_memory.back().pop_back();
        }
        else
        {
            Symbol_Table::PutRegisterToMemory("ft" + to_string(i), true);
        }
    }

    //if (O1_optimize)
    //{
    //    for (int i = 1; i < NUM_S_REGISTER; ++i)
    //    {
    //        if (extra_register.find("s" + to_string(i)) == extra_register.end())
    //        {
    //            saved_val.push_back({ "s" + to_string(i), Symbol_Table::avail_memory.back().back() });
    //            PrintCommandRelativeToSP("sw", "s" + to_string(i), Symbol_Table::avail_memory.back().back());
    //            Symbol_Table::avail_memory.back().pop_back();
    //        }
    //        /*else
    //        {
    //            Symbol_Table::PutRegisterToMemory("s" + to_string(i), false);
    //        }*/
    //    }
    //}

    output << "\tcall " << *name << endl;
    for (const auto& val : saved_val)
    {
        ValueType reg_type = GetRegisterType(val.first);
        if (reg_type == ValueType::INT)
        {
            PrintCommandRelativeToSP("lw", val.first, val.second);
            //output << "\tlw " << val.first << ", " << val.second << "(sp)\n";
        }
        else
        {
            PrintCommandRelativeToSP("flw", val.first, val.second);
            //output << "\tflw " << val.first << ", " << val.second << "(sp)\n";
        }
    }

    string temp_reg;
    if (func_def->type == ValueType::VOID)
        return "";
    if (func_def->type == ValueType::INT)
    {
        CheckAndSetRegister_INT(target_reg);
        output << "\tmv " << target_reg << ", a0\n";
        return target_reg;
    }
    else
    {
        CheckAndSetRegister_FLOAT(target_reg);
        output << "\tfmv.s " << target_reg << ", fa0\n";
        return target_reg;
    }
}

int FuncCallAST::GetSize() const
{
    if (param_vals)
        return max(0, int((param_vals->size() - 7) * 4));
    else
        return 0;
}

ASTNodeType FuncCallAST::GetNodeType() const
{
    return ASTNodeType::FUNCCALL;
}


ValueType FuncCallAST::GetType() const
{
    if (func_def == NULL)
        return ValueType::UNKNOWN;
    return func_def->type;
}

FuncDefAST::FuncDefAST(ValueType _type, unique_ptr<string> _name, unique_ptr<BlockAST> _block) :
    type(_type), name(move(_name)), block(move(_block)) {
}
FuncDefAST::FuncDefAST(ValueType _type, unique_ptr<string> _name, unique_ptr<vector<shared_ptr<VarDefAST>>> _params, unique_ptr<BlockAST> _block) :
    type(_type), name(move(_name)), params(move(_params)), block(move(_block))
{
    max_param_length = max(max_param_length, int(params->size()));
}

void FuncDefAST::Dump() const
{
    cout << "FuncDefAST { value_type: " << ToString(type) << ", ";
    cout << "name: " << *name;
    if (params)
    {
        cout << ", params: {";
        for (size_t i = 0; i < params->size(); ++i)
            cout << ToString(params->at(i)->type) << ", ";
        cout << " }, ";
    }
    block->Dump();
    cout << " }";
}

string FuncDefAST::GenerateRSICV(string target_reg)
{
    string prev_label = FuncDefAST::current_return_label;
    FuncDefAST::func_type = type;
    FuncDefAST::current_return_label = GetLabel(++cnt);
    output << "\t.globl " << *name << endl;
    output << "\t.type func, @function\n";
    output << *name << ":\n";

    int size;
    if (O1_optimize)
        size = block->GetSize() + 48 + (params ? params->size() * 4 : 0) + max_param_length * 12 * (FuncCallAST::funccall_cnt + 1);
    else
        size = block->GetSize() + 48 + (params ? params->size() * 4 : 0) + max_param_length * 8 * (FuncCallAST::funccall_cnt + 1);
    size += (16 - (size % 16));

    auto store_stack_pointer = [](const string command, const string& target_reg, int offset)
    {
        if (offset > 2047 || offset < -2048)
        {
            string shift_reg;
            CheckAndSetRegister_INT(shift_reg);

            output << "\tli " << shift_reg << ", " << offset << endl;
            output << "\tadd " << shift_reg << ", " << shift_reg << ", sp\n";
            output << '\t' << command << " " << target_reg << ", 0(" << shift_reg << ")\n";

            RestoreRegister(shift_reg);
        }
        else
        {
            output << '\t' << command << " " << target_reg << ", " << offset << "(sp)\n";
        }
    };

    Symbol_Table::ShiftStack(-size);
    Symbol_Table::PutAllToMemory(true, true);
    Symbol_Table::avail_memory.back().pop_back();
    int ra_address = Symbol_Table::avail_memory.back().back();

    store_stack_pointer("sd", "ra", ra_address);
    Symbol_Table::avail_memory.back().pop_back();

    int s_register_num = max(min(NUM_S_REGISTER, int(in_func_var_use[*name].size())), 1);
    if (!O1_optimize)
        s_register_num = 1;
    this->s_register_used = s_register_num;
    vector<int> si_address(s_register_num);
    for (int i = 0; i < s_register_num; ++i)
    {
        Symbol_Table::avail_memory.back().pop_back();
        int temp_address = Symbol_Table::avail_memory.back().back();
        store_stack_pointer("sd", "s" + to_string(i), temp_address);
        Symbol_Table::avail_memory.back().pop_back();

        si_address[i] = temp_address;
    }



    SYMBOL_TABLE_GUARD;

    if (params)
    {
        int start_address = 0;
        for (size_t i = 0; i < params->size(); ++i)
        {
            Symbol_Table::Insert(*params->at(i));
            if (i < 8) {
                if (params->at(i)->var_defs.at(0)->IsSingle())
                {
                    if (params->at(i)->type == ValueType::INT)
                        Symbol_Table::SetRegister(params->at(i)->var_defs.at(0)->GetName(), "a" + to_string(i));
                    else if (params->at(i)->type == ValueType::FLOAT)
                        Symbol_Table::SetRegister(params->at(i)->var_defs.at(0)->GetName(), "fa" + to_string(i));
                }
                else {
                    Symbol_Table::SetRegister(params->at(i)->var_defs.at(0)->GetName(), "a" + to_string(i));
                }
            }
            else
            {
                if (params->at(i)->var_defs.at(0)->IsSingle())
                {
                    Symbol_Table::SetAddress(params->at(i)->var_defs.at(0)->GetName(), size + start_address);
                    start_address += 4;
                }
                else
                {
                    Symbol_Table::SetAddress(params->at(i)->var_defs.at(0)->GetName(), size + start_address);
                    start_address += 8;
                }
            }
        }
    }

    /*if(O1_optimize)
    {
	    for(auto& var:Symbol_Table::infos[0])
	    {
		    if(var.second.dimensions.empty() && var.second.type == ValueType::INT)
		    {
		    	string extra_reg;
			    if(CheckAndSetRegister_EXTRA(extra_reg))
			    {
                    Symbol_Table::LoadVariableToRegister(var.first, false, extra_reg);
                }
                else
                {
                    break;
                }
		    }
	    }
    }*/

    block->GenerateRSICV();

    output << FuncDefAST::current_return_label << ':' << endl;

    Symbol_Table::PutAllToMemory(true, true);

    if(O1_optimize)
    {
        extra_register.clear();
	    for(int i = 1; i < NUM_S_REGISTER; ++i)
            extra_register.insert("s" + to_string(i));
    }

    store_stack_pointer("ld", "ra", ra_address);
    for (int i = 0; i < s_register_num; ++i)
        store_stack_pointer("ld", "s" + to_string(i), si_address[i]);

    Symbol_Table::ShiftStack(size);
    output << "\tjr ra" << endl;
    output << "\t.size " << *name << ", .-" << *name << endl;

    FuncDefAST::current_return_label = prev_label;
    FuncDefAST::func_type = ValueType::UNKNOWN;
    return target_reg;
}

ASTNodeType FuncDefAST::GetNodeType() const
{
    return ASTNodeType::FUNCDEF;
}

void CompUnitAST::Dump() const
{
    cout << "CompUnitAST { ";
    for (const shared_ptr<DefAST>& node : nodes)
    {
        node->Dump();

        if (node != nodes.back())
            cout << ", " << endl;
    }
    cout << " }";
}

void CompUnitAST::Append(shared_ptr<DefAST> node)
{
    nodes.push_back(node);
}

string CompUnitAST::GenerateRSICV(string target_reg)
{
    Symbol_Table::ClearTable();
    Symbol_Table::NewScope();
    //Symbol_Table::InsertPredefinedFunction();

    InitRegister();
    output << "\t.option pic\n";
    //output << "\t.attribute arch, \"rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0\"\n";
    output << "\t.attribute unaligned_access, 0\n";
    output << "\t.attribute stack_align, 16\n";

    FloatPool::GenerateAllFloatLiterals();

    output << "\t.data\n";

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes.at(i)->GetNodeType() == ASTNodeType::VARDEF)
            nodes.at(i)->GenerateRSICV();
    }

    output << "\t.text\n";

    output << "\t.globl scut_memset_int\n";
    output << "\t.type func, @function\n";
    output << "scut_memset_int:\n";
    output << "\tble a1,zero,.L1\n";
    output << "\tslli a2,a1,2\n";
    output << "\tli a1,0\n";
    output << "\ttail memset\n";
    output << ".L1:\n";
    output << "\tret\n";
    output << "\t.size scut_memset_int, .-scut_memset_int\n";

    output << "\t.globl scut_memset_float\n";
    output << "\t.type func, @function\n";
    output << "scut_memset_float:\n";
    output << "\tble a1,zero,.L2\n";
    output << "\tslli a2,a1,2\n";
    output << "\tli a1,0\n";
    output << "\ttail memset\n";
    output << ".L2:\n";
    output << "\tret\n";
    output << "\t.size scut_memset_float, .-scut_memset_float\n";

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes.at(i)->GetNodeType() == ASTNodeType::FUNCDEF)
        {
            /*if(O1_optimize)
            {
                auto ptr = dynamic_pointer_cast<FuncDefAST>(nodes.at(i));

                if (*ptr->name != "step")
                    O1_optimize = false;
                else
                    O1_optimize = true;
                nodes.at(i)->GenerateRSICV();
                O1_optimize = true;
            }
            else*/
            {
                nodes.at(i)->GenerateRSICV();
            }
        }
    }

    output << "\t.ident  \"GCC: (Debian 12.2.0 - 10) 12.2.0\"\n";
    output << "\t.section  .note.GNU-stack,\"\",@progbits\n";

    return "";
}
