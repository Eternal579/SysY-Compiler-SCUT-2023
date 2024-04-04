%code requires {
  #include <memory>
  #include <string>
  #include "AST.h"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "AST.h"
#include "Symbol_Table.h"

// 声明 lexer 函数和错误处理函数
extern int yylex();
extern void yyerror(std::shared_ptr<CompUnitAST> &ast, const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::shared_ptr<CompUnitAST> &ast }

// yyVal 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
%union {
  std::string *str_val;
  int int_val;
  float float_val;
  ValueType vartype_val;
  OpType optype_val;
  BaseAST *ast_val;
  BlockAST *block_val;
  vector<shared_ptr<StmtAST>> *blockitems_val;
  vector<shared_ptr<DeclAST>> *funccall_params_val;
  vector<shared_ptr<VarDefAST>> *funcdef_params_val;
  BreakAST *break_val;
  ContinueAST *continue_val;
  DeclAST *decl_val;
  ExprAST *expr_val;
  ValueAST *value_val;
  FuncCallAST *funccall_val;
  FuncDefAST *funcdef_val;
  IfAST *if_val;
  ReturnAST *return_val;
  StmtAST *stmt_val;
  VarDefAST *var_val;
  WhileAST *while_val;
  SingleVarDef *singlevardef_val;
  ArrayVarDef *arrayvardef_val;
  ListAST *list;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token <str_val> TOK_IDENT TOK_STRING
%token <int_val> TOK_INT_CONST
%token <float_val> TOK_FLOAT_CONST


%token TOK_CONST TOK_INT TOK_FLOAT TOK_COMMA TOK_SEMICOLON TOK_LBRACE TOK_RBRACE TOK_LBRACKET
%token TOK_RBRACKET TOK_LPAREN TOK_RPAREN TOK_ASSIGN TOK_VOID TOK_IF TOK_ELSE
%token TOK_WHILE TOK_BREAK TOK_CONTINUE TOK_RETURN TOK_PLUS TOK_MINUS TOK_MUL
%token TOK_DIV TOK_MOD TOK_GE TOK_GEQ TOK_LE TOK_LEQ TOK_EQ TOK_NEQ 
%token TOK_AND TOK_OR TOK_NOT


%nonassoc "then"
%nonassoc TOK_ELSE

%type <var_val> Decl ConstDecl VarDecl
%type <var_val> VarDef ConstDef
%type <singlevardef_val> VarDefSingle ConstDefSingle
%type <arrayvardef_val> VarDefArray DefArrayBody ConstDefArray
%type <decl_val> InitVal 
%type <list> InitValArray InitValArrayBody
%type <decl_val> Exp LOrExp LAndExp EqExp RelExp AddExp MulExp UnaryExp PrimaryExp 
%type <funccall_val> FuncCall
%type <value_val> LVal Number
%type <funcdef_val> FuncDef
%type <funcdef_params_val> FuncFParams
%type <var_val> FuncFParam FuncFParamSingle FuncFParamArray
%type <funccall_params_val> FuncRParams
%type <block_val> Block
%type <blockitems_val> BlockItems
%type <stmt_val> BlockItem
%type <stmt_val> Stmt
%type <if_val> IfStmt
%type <return_val> ReturnStmt
%type <while_val>  WhileStmt
%type <break_val> BreakStmt
%type <continue_val> ContinueStmt
%type <decl_val> Cond

%type <vartype_val> BType
%type <optype_val> AddOp MulOp UnaryOp RelOp

%type <str_val> IDENT STRING

%start CompUnit

%%

%left TOK_PLUS TOK_MINUS;
%left TOK_MUL TOK_DIV TOK_MOD;

CompUnit: CompUnit Decl { $2->is_global = true; if(!ast){ast = std::make_shared<CompUnitAST>();} ast->Append(shared_ptr<VarDefAST>($2)); //需要Append吗？？!!!!!!!!
          } 
        | CompUnit FuncDef { if(!ast){ast = std::make_shared<CompUnitAST>();} ast->Append(shared_ptr<FuncDefAST>($2)); }
        | Decl { $1->is_global = true; if(!ast)ast = std::make_shared<CompUnitAST>(); ast->Append(shared_ptr<VarDefAST>($1)); }
        | FuncDef { if(!ast)ast = std::make_shared<CompUnitAST>(); ast->Append(shared_ptr<FuncDefAST>($1)); }
        ;

Decl: ConstDecl TOK_SEMICOLON { $$ = $1; $$->is_const = true; }
    | VarDecl TOK_SEMICOLON { $$ = $1; }
    ;

BType: TOK_INT { $$ = ValueType::INT; }
     | TOK_FLOAT { $$ = ValueType::FLOAT; }
     ;

ConstDecl: TOK_CONST BType ConstDef { $$ = $3; $$->type = $2; }
         | ConstDecl TOK_COMMA ConstDef { $$ = $1; $$->Append(shared_ptr<VarDefAST>($3)); }
         ;

VarDecl: BType VarDef { $$ = $2; $$->type = $1; }
       | VarDecl TOK_COMMA VarDef { $$ = $1; $$->Append(shared_ptr<VarDefAST>($3)); }
       ;

VarDef: VarDefSingle { $$ = new VarDefAST(shared_ptr<VarDef>($1)); }
      | VarDefArray { $$ = new VarDefAST(shared_ptr<VarDef>($1)); }
      ;
VarDefSingle: IDENT TOK_ASSIGN InitVal { $$ = new SingleVarDef(unique_ptr<string>($1), unique_ptr<DeclAST>($3));}
            | IDENT { $$ = new SingleVarDef(unique_ptr<string>($1)); }
            ;
VarDefArray: DefArrayBody TOK_ASSIGN InitValArray {
     $$ = $1;
     shared_ptr<ListAST> arr($3);
     $$->list = arr;
     // shared_ptr<ListAST> test($3);
     // ListAST::preorder(test);
}
           | DefArrayBody { $$ = $1; }
           ;

DefArrayBody: DefArrayBody TOK_LBRACKET Exp TOK_RBRACKET { 
          $$ = $1; 
          $$->dimension.push_back(shared_ptr<DeclAST>($3)); 
     }
     | IDENT TOK_LBRACKET Exp TOK_RBRACKET { 
          vector<shared_ptr<DeclAST>> temp;
          temp.push_back(shared_ptr<DeclAST>($3));
          $$ = new ArrayVarDef(unique_ptr<string>($1), temp); 
     }
     ;

ConstDef: ConstDefSingle { $$ = new VarDefAST(unique_ptr<SingleVarDef>($1)); }
        | ConstDefArray { $$ = new VarDefAST(unique_ptr<ArrayVarDef>($1)); }
        ;

ConstDefSingle: IDENT TOK_ASSIGN InitVal { $$ = new SingleVarDef(unique_ptr<string>($1), unique_ptr<DeclAST>($3)); }
              ;

ConstDefArray: DefArrayBody TOK_ASSIGN InitValArray {
     /* TODO */
     $$ = $1;
     shared_ptr<ListAST> arr($3);
     $$->list = arr;
}
             ;

InitVal: AddExp { $$ = $1; };

InitValArray: TOK_LBRACE InitValArrayBody TOK_RBRACE {
          $$ = new ListAST();
          shared_ptr<ListAST> temp($2);
          $$->children.push_back(temp);
          $$->isBracket = true;
     }
            | TOK_LBRACE TOK_RBRACE {
          $$ = new ListAST();
          $$->isBracket = true;
     }
     ;

InitValArrayBody: InitValArrayBody TOK_COMMA InitValArray {
                    $$ = new ListAST();
                    shared_ptr<ListAST> temp1($1);
                    shared_ptr<ListAST> temp3($3);
                    $$->children.push_back(temp1);
                    $$->children.push_back(temp3);
                }
                | InitValArrayBody TOK_COMMA InitVal {
                    $$ = new ListAST();
                    shared_ptr<ListAST> temp1($1);
                    shared_ptr<DeclAST> temp3($3);
                    shared_ptr<ListAST> child = make_shared<ListAST>(temp3);
                    $$->children.push_back(temp1);
                    $$->children.push_back(child);
                }
                | InitValArray {
                    $$ = $1;
                }
                | InitVal {
                    $$ = new ListAST();
                    shared_ptr<DeclAST> temp($1);
                    shared_ptr<ListAST> child = make_shared<ListAST>(temp);
                    $$->children.push_back(child);
                }
                ;

Exp: AddExp { $$ = $1; };

LOrExp: LOrExp TOK_OR LAndExp { $$ = new ExprAST(OpType::OR, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3));}
      | LAndExp { $$ = $1; }
      ;

LAndExp: EqExp { $$ = $1; }
       | LAndExp TOK_AND EqExp { $$ = new ExprAST(OpType::AND, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3)); }
       ;

EqExp: RelExp { $$ = $1; }
     | EqExp TOK_EQ RelExp { $$ = new ExprAST(OpType::EQ, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3)); }
     | EqExp TOK_NEQ RelExp { $$ = new ExprAST(OpType::NEQ, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3)); }
     ;

RelExp: AddExp { $$ = $1; }
      | RelExp RelOp AddExp { $$ = new ExprAST($2, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3)); }
      ;

AddExp: MulExp { $$ = $1; }
      | AddExp AddOp MulExp { $$ = new ExprAST($2, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3)); }
      ;

MulExp: UnaryExp { $$ = $1; }
      | MulExp MulOp UnaryExp { $$ = new ExprAST($2, unique_ptr<DeclAST>($1), unique_ptr<DeclAST>($3));}
      ;

UnaryExp: PrimaryExp { $$ = $1; }
        | FuncCall { $$ = $1; }
        | UnaryOp UnaryExp { 
        if($1 == OpType::PLUS) 
            $$ = $2;
        else
            $$ = new ExprAST($1, unique_ptr<DeclAST>($2)); }
        ;

FuncCall: IDENT TOK_LPAREN FuncRParams TOK_RPAREN { $$ = new FuncCallAST(unique_ptr<string>($1), unique_ptr<vector<shared_ptr<DeclAST>>>($3));}
        | IDENT TOK_LPAREN TOK_RPAREN { $$ = new FuncCallAST(unique_ptr<string>($1)); }
        ;

PrimaryExp: TOK_LPAREN Exp TOK_RPAREN { $$ = $2; }
          | LVal { $$ = $1; }
          | Number { $$ = $1; }
          | STRING { $$ = new ValueAST((unique_ptr<string>($1))); }
          ;

LVal: LVal TOK_LBRACKET Exp TOK_RBRACKET {
            if($1->var_def->IsSingle())
            {
                auto array_var_ast = new ValueAST(make_unique<ArrayVarDef>((make_unique<string>($1->GetName()))));
                delete $1;
                $1 = array_var_ast;
            }
          $$ = $1;
          shared_ptr<DeclAST> exp_ptr($3);
          dynamic_pointer_cast<ArrayVarDef>($1->var_def)->dimension.push_back(exp_ptr);
    }
    | IDENT { $$ = new ValueAST(make_unique<SingleVarDef>((unique_ptr<string>($1)))); }
    ;

FuncDef: TOK_VOID IDENT TOK_LPAREN FuncFParams TOK_RPAREN Block { $$ = new FuncDefAST(ValueType::VOID, unique_ptr<string>($2), unique_ptr<vector<shared_ptr<VarDefAST>>>($4), unique_ptr<BlockAST>($6)); }
       | TOK_VOID IDENT TOK_LPAREN TOK_RPAREN Block { $$ = new FuncDefAST(ValueType::VOID, unique_ptr<string>($2), unique_ptr<BlockAST>($5)); }
       | BType IDENT TOK_LPAREN FuncFParams TOK_RPAREN Block { $$ = new FuncDefAST($1, unique_ptr<string>($2), unique_ptr<vector<shared_ptr<VarDefAST>>>($4), unique_ptr<BlockAST>($6)); }
       | BType IDENT TOK_LPAREN TOK_RPAREN Block {$$ = new FuncDefAST($1, unique_ptr<string>($2), unique_ptr<BlockAST>($5)); }
       ;

FuncFParams: FuncFParams TOK_COMMA FuncFParam { $$ = $1; $$->push_back(shared_ptr<VarDefAST>($3));}
           | FuncFParam { $$ = new vector<shared_ptr<VarDefAST>>(); $$->push_back(shared_ptr<VarDefAST>($1));}
           ;

FuncRParams: FuncRParams TOK_COMMA AddExp { $$ = $1; $$->push_back(shared_ptr<DeclAST>($3));}
           | AddExp { $$ = new vector<shared_ptr<DeclAST>>(); $$->push_back(shared_ptr<DeclAST>($1)); }
           ;

FuncFParam: FuncFParamSingle { $$ = $1; }
          | FuncFParamArray { $$ = $1; }
          ;

FuncFParamSingle: BType IDENT { $$ = new VarDefAST(unique_ptr<string>($2)); $$->type = $1; }
                ;

FuncFParamArray: BType IDENT TOK_LBRACKET TOK_RBRACKET { 
          shared_ptr<ArrayVarDef> var_def = make_shared<ArrayVarDef>(unique_ptr<string>($2));
          var_def->dimension.push_back(nullptr);
          $$ = new VarDefAST(var_def); 
          $$->type = $1; 
     }
               | FuncFParamArray TOK_LBRACKET Exp TOK_RBRACKET { 
          $$ = $1;
          (dynamic_pointer_cast<ArrayVarDef>($$->var_defs[0]))->dimension.push_back(shared_ptr<DeclAST>($3));
     }
               ;

Block: TOK_LBRACE TOK_RBRACE { $$ = new BlockAST(); }
     | TOK_LBRACE BlockItems TOK_RBRACE { $$ = new BlockAST(unique_ptr<vector<shared_ptr<StmtAST>>>($2));}
     ;

BlockItems: BlockItem { $$ = new vector<shared_ptr<StmtAST>>(); if($1) $$->push_back(shared_ptr<StmtAST>($1)); }
          | BlockItems BlockItem { $$ = $1; if($2) $$->push_back(shared_ptr<StmtAST>($2)); }
          ;

BlockItem: Decl { $$ = $1; }
         | Stmt { $$ = $1; }
         ;

Stmt: LVal TOK_ASSIGN Exp TOK_SEMICOLON { $$ = new AssignmentAST( unique_ptr<ValueAST>($1), unique_ptr<DeclAST>($3)); }
    | Exp TOK_SEMICOLON { $$ = $1; }
    | TOK_SEMICOLON { $$ = new NullStmtAST(); }
    | Block { $$ = $1; } 
    | IfStmt { $$ = $1; }
    | WhileStmt { $$ = $1; }
    | BreakStmt { $$ = $1; }
    | ContinueStmt { $$ = $1; }
    | ReturnStmt { $$ = $1; }
    ;

IfStmt: TOK_IF TOK_LPAREN Cond TOK_RPAREN Stmt TOK_ELSE Stmt { $$ = new IfAST(unique_ptr<DeclAST>($3), shared_ptr<StmtAST>($5), shared_ptr<StmtAST>($7));}
      | TOK_IF TOK_LPAREN Cond TOK_RPAREN Stmt { $$ = new IfAST(unique_ptr<DeclAST>($3), shared_ptr<StmtAST>($5));} %prec "then"
      ;

ReturnStmt: TOK_RETURN Exp TOK_SEMICOLON { $$ = new ReturnAST(shared_ptr<DeclAST>($2)); }
          | TOK_RETURN TOK_SEMICOLON { $$ = new ReturnAST(); }
          ;

WhileStmt: TOK_WHILE TOK_LPAREN Cond TOK_RPAREN Stmt { $$ = new WhileAST(unique_ptr<DeclAST>($3), shared_ptr<StmtAST>($5));};

BreakStmt: TOK_BREAK TOK_SEMICOLON { $$ = new BreakAST();};

ContinueStmt: TOK_CONTINUE TOK_SEMICOLON { $$ = new ContinueAST();};

Cond: LOrExp { $$ = $1; };

Number: TOK_INT_CONST { $$ = new ValueAST($1); }
    | TOK_FLOAT_CONST { $$ = new ValueAST($1); }
    ;

AddOp: TOK_PLUS { $$ = OpType::PLUS; }
     | TOK_MINUS { $$ = OpType::MINUS; }
     ;

MulOp: TOK_MUL { $$ = OpType::MUL; }
     | TOK_DIV { $$ = OpType::DIV; }
     | TOK_MOD { $$ = OpType::MOD; }
     ;

UnaryOp: TOK_PLUS { $$ = OpType::PLUS; }
       | TOK_MINUS { $$ = OpType::MINUS; }
       | TOK_NOT { $$ = OpType::NOT; }
       ;

RelOp: TOK_GE { $$ = OpType::GE; } 
     | TOK_GEQ { $$ = OpType::GEQ; } 
     | TOK_LE { $$ = OpType::LE; } 
     | TOK_LEQ { $$ = OpType::LEQ; } 
     ;

IDENT: TOK_IDENT { $$ = $1; };

STRING: TOK_STRING { $$ = $1; };


%%

