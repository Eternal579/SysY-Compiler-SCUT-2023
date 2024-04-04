/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "sysy.y"

  #include <memory>
  #include <string>
  #include "AST.h"

#line 55 "y.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_IDENT = 258,               /* TOK_IDENT  */
    TOK_STRING = 259,              /* TOK_STRING  */
    TOK_INT_CONST = 260,           /* TOK_INT_CONST  */
    TOK_FLOAT_CONST = 261,         /* TOK_FLOAT_CONST  */
    TOK_CONST = 262,               /* TOK_CONST  */
    TOK_INT = 263,                 /* TOK_INT  */
    TOK_FLOAT = 264,               /* TOK_FLOAT  */
    TOK_COMMA = 265,               /* TOK_COMMA  */
    TOK_SEMICOLON = 266,           /* TOK_SEMICOLON  */
    TOK_LBRACE = 267,              /* TOK_LBRACE  */
    TOK_RBRACE = 268,              /* TOK_RBRACE  */
    TOK_LBRACKET = 269,            /* TOK_LBRACKET  */
    TOK_RBRACKET = 270,            /* TOK_RBRACKET  */
    TOK_LPAREN = 271,              /* TOK_LPAREN  */
    TOK_RPAREN = 272,              /* TOK_RPAREN  */
    TOK_ASSIGN = 273,              /* TOK_ASSIGN  */
    TOK_VOID = 274,                /* TOK_VOID  */
    TOK_IF = 275,                  /* TOK_IF  */
    TOK_ELSE = 276,                /* TOK_ELSE  */
    TOK_WHILE = 277,               /* TOK_WHILE  */
    TOK_BREAK = 278,               /* TOK_BREAK  */
    TOK_CONTINUE = 279,            /* TOK_CONTINUE  */
    TOK_RETURN = 280,              /* TOK_RETURN  */
    TOK_PLUS = 281,                /* TOK_PLUS  */
    TOK_MINUS = 282,               /* TOK_MINUS  */
    TOK_MUL = 283,                 /* TOK_MUL  */
    TOK_DIV = 284,                 /* TOK_DIV  */
    TOK_MOD = 285,                 /* TOK_MOD  */
    TOK_GE = 286,                  /* TOK_GE  */
    TOK_GEQ = 287,                 /* TOK_GEQ  */
    TOK_LE = 288,                  /* TOK_LE  */
    TOK_LEQ = 289,                 /* TOK_LEQ  */
    TOK_EQ = 290,                  /* TOK_EQ  */
    TOK_NEQ = 291,                 /* TOK_NEQ  */
    TOK_AND = 292,                 /* TOK_AND  */
    TOK_OR = 293,                  /* TOK_OR  */
    TOK_NOT = 294                  /* TOK_NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_IDENT 258
#define TOK_STRING 259
#define TOK_INT_CONST 260
#define TOK_FLOAT_CONST 261
#define TOK_CONST 262
#define TOK_INT 263
#define TOK_FLOAT 264
#define TOK_COMMA 265
#define TOK_SEMICOLON 266
#define TOK_LBRACE 267
#define TOK_RBRACE 268
#define TOK_LBRACKET 269
#define TOK_RBRACKET 270
#define TOK_LPAREN 271
#define TOK_RPAREN 272
#define TOK_ASSIGN 273
#define TOK_VOID 274
#define TOK_IF 275
#define TOK_ELSE 276
#define TOK_WHILE 277
#define TOK_BREAK 278
#define TOK_CONTINUE 279
#define TOK_RETURN 280
#define TOK_PLUS 281
#define TOK_MINUS 282
#define TOK_MUL 283
#define TOK_DIV 284
#define TOK_MOD 285
#define TOK_GE 286
#define TOK_GEQ 287
#define TOK_LE 288
#define TOK_LEQ 289
#define TOK_EQ 290
#define TOK_NEQ 291
#define TOK_AND 292
#define TOK_OR 293
#define TOK_NOT 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 31 "sysy.y"

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

#line 181 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (std::shared_ptr<CompUnitAST> &ast);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
