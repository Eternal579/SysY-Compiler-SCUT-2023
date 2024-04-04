/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 7 "sysy.y"


#include <iostream>
#include <memory>
#include <string>
#include "AST.h"
#include "Symbol_Table.h"

// 声明 lexer 函数和错误处理函数
extern int yylex();
extern void yyerror(std::shared_ptr<CompUnitAST> &ast, const char *s);

using namespace std;


#line 87 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 128 "y.tab.c"

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

#line 254 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (std::shared_ptr<CompUnitAST> &ast);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_IDENT = 3,                  /* TOK_IDENT  */
  YYSYMBOL_TOK_STRING = 4,                 /* TOK_STRING  */
  YYSYMBOL_TOK_INT_CONST = 5,              /* TOK_INT_CONST  */
  YYSYMBOL_TOK_FLOAT_CONST = 6,            /* TOK_FLOAT_CONST  */
  YYSYMBOL_TOK_CONST = 7,                  /* TOK_CONST  */
  YYSYMBOL_TOK_INT = 8,                    /* TOK_INT  */
  YYSYMBOL_TOK_FLOAT = 9,                  /* TOK_FLOAT  */
  YYSYMBOL_TOK_COMMA = 10,                 /* TOK_COMMA  */
  YYSYMBOL_TOK_SEMICOLON = 11,             /* TOK_SEMICOLON  */
  YYSYMBOL_TOK_LBRACE = 12,                /* TOK_LBRACE  */
  YYSYMBOL_TOK_RBRACE = 13,                /* TOK_RBRACE  */
  YYSYMBOL_TOK_LBRACKET = 14,              /* TOK_LBRACKET  */
  YYSYMBOL_TOK_RBRACKET = 15,              /* TOK_RBRACKET  */
  YYSYMBOL_TOK_LPAREN = 16,                /* TOK_LPAREN  */
  YYSYMBOL_TOK_RPAREN = 17,                /* TOK_RPAREN  */
  YYSYMBOL_TOK_ASSIGN = 18,                /* TOK_ASSIGN  */
  YYSYMBOL_TOK_VOID = 19,                  /* TOK_VOID  */
  YYSYMBOL_TOK_IF = 20,                    /* TOK_IF  */
  YYSYMBOL_TOK_ELSE = 21,                  /* TOK_ELSE  */
  YYSYMBOL_TOK_WHILE = 22,                 /* TOK_WHILE  */
  YYSYMBOL_TOK_BREAK = 23,                 /* TOK_BREAK  */
  YYSYMBOL_TOK_CONTINUE = 24,              /* TOK_CONTINUE  */
  YYSYMBOL_TOK_RETURN = 25,                /* TOK_RETURN  */
  YYSYMBOL_TOK_PLUS = 26,                  /* TOK_PLUS  */
  YYSYMBOL_TOK_MINUS = 27,                 /* TOK_MINUS  */
  YYSYMBOL_TOK_MUL = 28,                   /* TOK_MUL  */
  YYSYMBOL_TOK_DIV = 29,                   /* TOK_DIV  */
  YYSYMBOL_TOK_MOD = 30,                   /* TOK_MOD  */
  YYSYMBOL_TOK_GE = 31,                    /* TOK_GE  */
  YYSYMBOL_TOK_GEQ = 32,                   /* TOK_GEQ  */
  YYSYMBOL_TOK_LE = 33,                    /* TOK_LE  */
  YYSYMBOL_TOK_LEQ = 34,                   /* TOK_LEQ  */
  YYSYMBOL_TOK_EQ = 35,                    /* TOK_EQ  */
  YYSYMBOL_TOK_NEQ = 36,                   /* TOK_NEQ  */
  YYSYMBOL_TOK_AND = 37,                   /* TOK_AND  */
  YYSYMBOL_TOK_OR = 38,                    /* TOK_OR  */
  YYSYMBOL_TOK_NOT = 39,                   /* TOK_NOT  */
  YYSYMBOL_40_then_ = 40,                  /* "then"  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_CompUnit = 42,                  /* CompUnit  */
  YYSYMBOL_Decl = 43,                      /* Decl  */
  YYSYMBOL_BType = 44,                     /* BType  */
  YYSYMBOL_ConstDecl = 45,                 /* ConstDecl  */
  YYSYMBOL_VarDecl = 46,                   /* VarDecl  */
  YYSYMBOL_VarDef = 47,                    /* VarDef  */
  YYSYMBOL_VarDefSingle = 48,              /* VarDefSingle  */
  YYSYMBOL_VarDefArray = 49,               /* VarDefArray  */
  YYSYMBOL_DefArrayBody = 50,              /* DefArrayBody  */
  YYSYMBOL_ConstDef = 51,                  /* ConstDef  */
  YYSYMBOL_ConstDefSingle = 52,            /* ConstDefSingle  */
  YYSYMBOL_ConstDefArray = 53,             /* ConstDefArray  */
  YYSYMBOL_InitVal = 54,                   /* InitVal  */
  YYSYMBOL_InitValArray = 55,              /* InitValArray  */
  YYSYMBOL_InitValArrayBody = 56,          /* InitValArrayBody  */
  YYSYMBOL_Exp = 57,                       /* Exp  */
  YYSYMBOL_LOrExp = 58,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 59,                   /* LAndExp  */
  YYSYMBOL_EqExp = 60,                     /* EqExp  */
  YYSYMBOL_RelExp = 61,                    /* RelExp  */
  YYSYMBOL_AddExp = 62,                    /* AddExp  */
  YYSYMBOL_MulExp = 63,                    /* MulExp  */
  YYSYMBOL_UnaryExp = 64,                  /* UnaryExp  */
  YYSYMBOL_FuncCall = 65,                  /* FuncCall  */
  YYSYMBOL_PrimaryExp = 66,                /* PrimaryExp  */
  YYSYMBOL_LVal = 67,                      /* LVal  */
  YYSYMBOL_FuncDef = 68,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 69,               /* FuncFParams  */
  YYSYMBOL_FuncRParams = 70,               /* FuncRParams  */
  YYSYMBOL_FuncFParam = 71,                /* FuncFParam  */
  YYSYMBOL_FuncFParamSingle = 72,          /* FuncFParamSingle  */
  YYSYMBOL_FuncFParamArray = 73,           /* FuncFParamArray  */
  YYSYMBOL_Block = 74,                     /* Block  */
  YYSYMBOL_BlockItems = 75,                /* BlockItems  */
  YYSYMBOL_BlockItem = 76,                 /* BlockItem  */
  YYSYMBOL_Stmt = 77,                      /* Stmt  */
  YYSYMBOL_IfStmt = 78,                    /* IfStmt  */
  YYSYMBOL_ReturnStmt = 79,                /* ReturnStmt  */
  YYSYMBOL_WhileStmt = 80,                 /* WhileStmt  */
  YYSYMBOL_BreakStmt = 81,                 /* BreakStmt  */
  YYSYMBOL_ContinueStmt = 82,              /* ContinueStmt  */
  YYSYMBOL_Cond = 83,                      /* Cond  */
  YYSYMBOL_Number = 84,                    /* Number  */
  YYSYMBOL_AddOp = 85,                     /* AddOp  */
  YYSYMBOL_MulOp = 86,                     /* MulOp  */
  YYSYMBOL_UnaryOp = 87,                   /* UnaryOp  */
  YYSYMBOL_RelOp = 88,                     /* RelOp  */
  YYSYMBOL_IDENT = 89,                     /* IDENT  */
  YYSYMBOL_STRING = 90                     /* STRING  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   245

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  182

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   114,   115,   116,   119,   120,   123,   124,
     127,   128,   131,   132,   135,   136,   138,   139,   141,   148,
     151,   155,   162,   163,   166,   169,   177,   179,   185,   191,
     198,   206,   209,   217,   219,   220,   223,   224,   227,   228,
     229,   232,   233,   236,   237,   240,   241,   244,   245,   246,
     253,   254,   257,   258,   259,   260,   263,   274,   277,   278,
     279,   280,   283,   284,   287,   288,   291,   292,   295,   298,
     304,   310,   311,   314,   315,   318,   319,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   333,   334,   337,   338,
     341,   343,   345,   347,   349,   350,   353,   354,   357,   358,
     359,   362,   363,   364,   367,   368,   369,   370,   373,   375
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_IDENT",
  "TOK_STRING", "TOK_INT_CONST", "TOK_FLOAT_CONST", "TOK_CONST", "TOK_INT",
  "TOK_FLOAT", "TOK_COMMA", "TOK_SEMICOLON", "TOK_LBRACE", "TOK_RBRACE",
  "TOK_LBRACKET", "TOK_RBRACKET", "TOK_LPAREN", "TOK_RPAREN", "TOK_ASSIGN",
  "TOK_VOID", "TOK_IF", "TOK_ELSE", "TOK_WHILE", "TOK_BREAK",
  "TOK_CONTINUE", "TOK_RETURN", "TOK_PLUS", "TOK_MINUS", "TOK_MUL",
  "TOK_DIV", "TOK_MOD", "TOK_GE", "TOK_GEQ", "TOK_LE", "TOK_LEQ", "TOK_EQ",
  "TOK_NEQ", "TOK_AND", "TOK_OR", "TOK_NOT", "\"then\"", "$accept",
  "CompUnit", "Decl", "BType", "ConstDecl", "VarDecl", "VarDef",
  "VarDefSingle", "VarDefArray", "DefArrayBody", "ConstDef",
  "ConstDefSingle", "ConstDefArray", "InitVal", "InitValArray",
  "InitValArrayBody", "Exp", "LOrExp", "LAndExp", "EqExp", "RelExp",
  "AddExp", "MulExp", "UnaryExp", "FuncCall", "PrimaryExp", "LVal",
  "FuncDef", "FuncFParams", "FuncRParams", "FuncFParam",
  "FuncFParamSingle", "FuncFParamArray", "Block", "BlockItems",
  "BlockItem", "Stmt", "IfStmt", "ReturnStmt", "WhileStmt", "BreakStmt",
  "ContinueStmt", "Cond", "Number", "AddOp", "MulOp", "UnaryOp", "RelOp",
  "IDENT", "STRING", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     100,    50,  -153,  -153,    24,    84,  -153,    24,   111,   188,
    -153,    24,  -153,    81,  -153,  -153,  -153,  -153,  -153,  -153,
      -6,    64,    24,  -153,    24,  -153,    82,  -153,  -153,  -153,
      96,    45,   199,    36,   199,    55,   199,  -153,  -153,   106,
      36,   199,    92,    24,    15,  -153,  -153,   134,  -153,  -153,
    -153,   199,  -153,  -153,  -153,    97,   183,   145,  -153,  -153,
    -153,   138,  -153,   199,   150,  -153,   181,  -153,   135,    92,
      32,  -153,   183,  -153,  -153,   131,  -153,   157,    50,    92,
     199,   160,  -153,  -153,  -153,   199,  -153,  -153,  -153,   199,
     199,  -153,    18,  -153,  -153,  -153,   136,  -153,  -153,    92,
    -153,  -153,   176,   184,   195,   207,   185,  -153,    24,   208,
     127,  -153,   156,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
     206,  -153,  -153,   212,  -153,   145,  -153,   213,  -153,   183,
      60,    34,  -153,  -153,   199,   199,  -153,  -153,  -153,   211,
    -153,   199,  -153,  -153,  -153,  -153,  -153,   199,  -153,  -153,
    -153,   191,   186,   178,    98,   183,   214,   215,  -153,   219,
     183,   199,   199,   199,   199,  -153,  -153,  -153,  -153,   199,
      63,    63,  -153,   186,   178,    98,    98,   183,   216,  -153,
      63,  -153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     8,     9,     0,     0,     4,     0,     0,     0,
       5,     0,   108,     0,     1,     2,     3,    12,    14,    15,
      19,    17,     0,     6,     0,     7,     0,    10,    22,    23,
       0,     0,     0,     0,     0,     0,     0,    11,    13,    17,
       0,     0,     0,     0,     0,    63,    66,    67,   109,    94,
      95,     0,   101,   102,   103,     0,    33,    43,    45,    48,
      47,    53,    54,     0,    57,    55,     0,    18,     0,     0,
       0,    16,    26,    25,    24,     0,    59,    68,     0,     0,
       0,     0,    20,    96,    97,     0,    98,    99,   100,     0,
       0,    49,     0,    28,    32,    31,     0,    21,    61,     0,
      79,    71,     0,     0,     0,     0,     0,    75,     0,     0,
      53,    80,     0,    73,    76,    81,    85,    82,    83,    84,
       0,    62,    58,     0,    52,    44,    46,     0,    51,    65,
       0,     0,    27,    60,     0,     0,    91,    92,    89,     0,
      78,     0,    72,    74,    69,    70,    56,     0,    50,    30,
      29,    93,    35,    36,    38,    41,     0,     0,    88,     0,
      64,     0,     0,     0,     0,   104,   105,   106,   107,     0,
       0,     0,    77,    34,    37,    39,    40,    42,    87,    90,
       0,    86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,   118,     1,  -153,  -153,   209,  -153,  -153,    -7,
     217,  -153,  -153,   -30,   -37,  -153,   -25,  -153,    73,    74,
      53,   -36,   155,   -46,  -153,  -153,   -65,   230,   210,  -153,
     163,  -153,  -153,   -28,  -153,   130,  -152,  -153,  -153,  -153,
    -153,  -153,   108,  -153,  -153,  -153,  -153,  -153,     9,  -153
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,   107,    43,     8,     9,    17,    18,    19,    20,
      27,    28,    29,    71,    67,    96,   109,   151,   152,   153,
     154,    56,    57,    58,    59,    60,    61,    10,    44,   130,
      45,    46,    47,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   156,    62,    85,    89,    63,   169,    64,    65
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      72,     7,    11,    73,    26,    72,     7,    55,    32,    68,
     110,    74,    33,    13,    76,    26,    21,    91,   178,   179,
      30,    12,    48,    49,    50,    78,    81,    12,   181,    95,
      72,    30,    79,    39,    51,   128,    94,    12,    48,    49,
      50,    98,    78,   126,    52,    53,    66,   110,    66,    99,
      51,   122,    77,     2,     3,   123,   129,    54,     2,     3,
      52,    53,    42,     2,     3,   127,    12,    48,    49,    50,
     147,   133,    69,    54,   100,    75,   108,   148,    34,    51,
      35,   139,    36,   102,    14,   103,   104,   105,   106,    52,
      53,     1,     2,     3,   150,    72,    32,    31,   155,   155,
      40,   149,    54,     4,    75,   110,   110,     1,     2,     3,
      34,   160,    82,   108,    41,   110,   159,    39,     6,     4,
      34,    22,    23,    15,    36,   155,   155,   155,   155,   165,
     166,   167,   168,   177,    12,    48,    49,    50,     1,     2,
       3,    90,   100,    75,   101,   141,   131,    51,    80,   132,
      97,   102,    90,   103,   104,   105,   106,    52,    53,    12,
      48,    49,    50,     1,     2,     3,    92,   100,    75,   142,
      54,   120,    51,    86,    87,    88,   102,   124,   103,   104,
     105,   106,    52,    53,    12,    48,    49,    50,    12,    48,
      49,    50,   134,    66,    93,    54,   138,    51,    24,    25,
     135,    51,    12,    48,    49,    50,   136,    52,    53,    83,
      84,    52,    53,   163,   164,    51,   175,   176,   137,   140,
      54,   144,   158,   162,    54,    52,    53,   145,   146,   161,
     172,   170,   171,    38,   173,    16,   174,   180,    54,    37,
     125,   121,   143,   157,     0,    70
};

static const yytype_int16 yycheck[] =
{
      36,     0,     1,    40,    11,    41,     5,    32,    14,    34,
      75,    41,    18,     4,    42,    22,     7,    63,   170,   171,
      11,     3,     4,     5,     6,    10,    51,     3,   180,    66,
      66,    22,    17,    24,    16,    17,    66,     3,     4,     5,
       6,    69,    10,    89,    26,    27,    12,   112,    12,    17,
      16,    79,    43,     8,     9,    80,    92,    39,     8,     9,
      26,    27,    17,     8,     9,    90,     3,     4,     5,     6,
      10,    99,    17,    39,    11,    12,    75,    17,    14,    16,
      16,   106,    18,    20,     0,    22,    23,    24,    25,    26,
      27,     7,     8,     9,   131,   131,    14,    16,   134,   135,
      18,   131,    39,    19,    12,   170,   171,     7,     8,     9,
      14,   147,    15,   112,    18,   180,   141,   108,     0,    19,
      14,    10,    11,     5,    18,   161,   162,   163,   164,    31,
      32,    33,    34,   169,     3,     4,     5,     6,     7,     8,
       9,    14,    11,    12,    13,    18,    10,    16,    14,    13,
      15,    20,    14,    22,    23,    24,    25,    26,    27,     3,
       4,     5,     6,     7,     8,     9,    16,    11,    12,    13,
      39,    14,    16,    28,    29,    30,    20,    17,    22,    23,
      24,    25,    26,    27,     3,     4,     5,     6,     3,     4,
       5,     6,    16,    12,    13,    39,    11,    16,    10,    11,
      16,    16,     3,     4,     5,     6,    11,    26,    27,    26,
      27,    26,    27,    35,    36,    16,   163,   164,    11,    11,
      39,    15,    11,    37,    39,    26,    27,    15,    15,    38,
      11,    17,    17,    24,   161,     5,   162,    21,    39,    22,
      85,    78,   112,   135,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    19,    42,    43,    44,    45,    46,
      68,    44,     3,    89,     0,    43,    68,    47,    48,    49,
      50,    89,    10,    11,    10,    11,    50,    51,    52,    53,
      89,    16,    14,    18,    14,    16,    18,    51,    47,    89,
      18,    18,    17,    44,    69,    71,    72,    73,     4,     5,
       6,    16,    26,    27,    39,    57,    62,    63,    64,    65,
      66,    67,    84,    87,    89,    90,    12,    55,    57,    17,
      69,    54,    62,    55,    54,    12,    74,    89,    10,    17,
      14,    57,    15,    26,    27,    85,    28,    29,    30,    86,
      14,    64,    16,    13,    54,    55,    56,    15,    74,    17,
      11,    13,    20,    22,    23,    24,    25,    43,    44,    57,
      67,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      14,    71,    74,    57,    17,    63,    64,    57,    17,    62,
      70,    10,    13,    74,    16,    16,    11,    11,    11,    57,
      11,    18,    13,    76,    15,    15,    15,    10,    17,    54,
      55,    58,    59,    60,    61,    62,    83,    83,    11,    57,
      62,    38,    37,    35,    36,    31,    32,    33,    34,    88,
      17,    17,    11,    59,    60,    61,    61,    62,    77,    77,
      21,    77
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    42,    42,    42,    43,    43,    44,    44,
      45,    45,    46,    46,    47,    47,    48,    48,    49,    49,
      50,    50,    51,    51,    52,    53,    54,    55,    55,    56,
      56,    56,    56,    57,    58,    58,    59,    59,    60,    60,
      60,    61,    61,    62,    62,    63,    63,    64,    64,    64,
      65,    65,    66,    66,    66,    66,    67,    67,    68,    68,
      68,    68,    69,    69,    70,    70,    71,    71,    72,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    78,    78,    79,    79,
      80,    81,    82,    83,    84,    84,    85,    85,    86,    86,
      86,    87,    87,    87,    88,    88,    88,    88,    89,    90
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     2,     2,     1,     1,
       3,     3,     2,     3,     1,     1,     3,     1,     3,     1,
       4,     4,     1,     1,     3,     3,     1,     3,     2,     3,
       3,     1,     1,     1,     3,     1,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     2,
       4,     3,     3,     1,     1,     1,     4,     1,     6,     5,
       6,     5,     3,     1,     3,     1,     1,     1,     2,     4,
       4,     2,     3,     1,     2,     1,     1,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     7,     5,     3,     2,
       5,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, std::shared_ptr<CompUnitAST> &ast)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (ast);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, std::shared_ptr<CompUnitAST> &ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, ast);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, std::shared_ptr<CompUnitAST> &ast)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ast); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, std::shared_ptr<CompUnitAST> &ast)
{
  YY_USE (yyvaluep);
  YY_USE (ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (std::shared_ptr<CompUnitAST> &ast)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CompUnit: CompUnit Decl  */
#line 112 "sysy.y"
                        { (yyvsp[0].var_val)->is_global = true; if(!ast){ast = std::make_shared<CompUnitAST>();} ast->Append(shared_ptr<VarDefAST>((yyvsp[0].var_val))); //需要Append吗？？!!!!!!!!
          }
#line 1472 "y.tab.c"
    break;

  case 3: /* CompUnit: CompUnit FuncDef  */
#line 114 "sysy.y"
                           { if(!ast){ast = std::make_shared<CompUnitAST>();} ast->Append(shared_ptr<FuncDefAST>((yyvsp[0].funcdef_val))); }
#line 1478 "y.tab.c"
    break;

  case 4: /* CompUnit: Decl  */
#line 115 "sysy.y"
               { (yyvsp[0].var_val)->is_global = true; if(!ast)ast = std::make_shared<CompUnitAST>(); ast->Append(shared_ptr<VarDefAST>((yyvsp[0].var_val))); }
#line 1484 "y.tab.c"
    break;

  case 5: /* CompUnit: FuncDef  */
#line 116 "sysy.y"
                  { if(!ast)ast = std::make_shared<CompUnitAST>(); ast->Append(shared_ptr<FuncDefAST>((yyvsp[0].funcdef_val))); }
#line 1490 "y.tab.c"
    break;

  case 6: /* Decl: ConstDecl TOK_SEMICOLON  */
#line 119 "sysy.y"
                              { (yyval.var_val) = (yyvsp[-1].var_val); (yyval.var_val)->is_const = true; }
#line 1496 "y.tab.c"
    break;

  case 7: /* Decl: VarDecl TOK_SEMICOLON  */
#line 120 "sysy.y"
                            { (yyval.var_val) = (yyvsp[-1].var_val); }
#line 1502 "y.tab.c"
    break;

  case 8: /* BType: TOK_INT  */
#line 123 "sysy.y"
               { (yyval.vartype_val) = ValueType::INT; }
#line 1508 "y.tab.c"
    break;

  case 9: /* BType: TOK_FLOAT  */
#line 124 "sysy.y"
                 { (yyval.vartype_val) = ValueType::FLOAT; }
#line 1514 "y.tab.c"
    break;

  case 10: /* ConstDecl: TOK_CONST BType ConstDef  */
#line 127 "sysy.y"
                                    { (yyval.var_val) = (yyvsp[0].var_val); (yyval.var_val)->type = (yyvsp[-1].vartype_val); }
#line 1520 "y.tab.c"
    break;

  case 11: /* ConstDecl: ConstDecl TOK_COMMA ConstDef  */
#line 128 "sysy.y"
                                        { (yyval.var_val) = (yyvsp[-2].var_val); (yyval.var_val)->Append(shared_ptr<VarDefAST>((yyvsp[0].var_val))); }
#line 1526 "y.tab.c"
    break;

  case 12: /* VarDecl: BType VarDef  */
#line 131 "sysy.y"
                      { (yyval.var_val) = (yyvsp[0].var_val); (yyval.var_val)->type = (yyvsp[-1].vartype_val); }
#line 1532 "y.tab.c"
    break;

  case 13: /* VarDecl: VarDecl TOK_COMMA VarDef  */
#line 132 "sysy.y"
                                  { (yyval.var_val) = (yyvsp[-2].var_val); (yyval.var_val)->Append(shared_ptr<VarDefAST>((yyvsp[0].var_val))); }
#line 1538 "y.tab.c"
    break;

  case 14: /* VarDef: VarDefSingle  */
#line 135 "sysy.y"
                     { (yyval.var_val) = new VarDefAST(shared_ptr<VarDef>((yyvsp[0].singlevardef_val))); }
#line 1544 "y.tab.c"
    break;

  case 15: /* VarDef: VarDefArray  */
#line 136 "sysy.y"
                    { (yyval.var_val) = new VarDefAST(shared_ptr<VarDef>((yyvsp[0].arrayvardef_val))); }
#line 1550 "y.tab.c"
    break;

  case 16: /* VarDefSingle: IDENT TOK_ASSIGN InitVal  */
#line 138 "sysy.y"
                                       { (yyval.singlevardef_val) = new SingleVarDef(unique_ptr<string>((yyvsp[-2].str_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val)));}
#line 1556 "y.tab.c"
    break;

  case 17: /* VarDefSingle: IDENT  */
#line 139 "sysy.y"
                    { (yyval.singlevardef_val) = new SingleVarDef(unique_ptr<string>((yyvsp[0].str_val))); }
#line 1562 "y.tab.c"
    break;

  case 18: /* VarDefArray: DefArrayBody TOK_ASSIGN InitValArray  */
#line 141 "sysy.y"
                                                  {
     (yyval.arrayvardef_val) = (yyvsp[-2].arrayvardef_val);
     shared_ptr<ListAST> arr((yyvsp[0].list));
     (yyval.arrayvardef_val)->list = arr;
     // shared_ptr<ListAST> test($3);
     // ListAST::preorder(test);
}
#line 1574 "y.tab.c"
    break;

  case 19: /* VarDefArray: DefArrayBody  */
#line 148 "sysy.y"
                          { (yyval.arrayvardef_val) = (yyvsp[0].arrayvardef_val); }
#line 1580 "y.tab.c"
    break;

  case 20: /* DefArrayBody: DefArrayBody TOK_LBRACKET Exp TOK_RBRACKET  */
#line 151 "sysy.y"
                                                         { 
          (yyval.arrayvardef_val) = (yyvsp[-3].arrayvardef_val); 
          (yyval.arrayvardef_val)->dimension.push_back(shared_ptr<DeclAST>((yyvsp[-1].decl_val))); 
     }
#line 1589 "y.tab.c"
    break;

  case 21: /* DefArrayBody: IDENT TOK_LBRACKET Exp TOK_RBRACKET  */
#line 155 "sysy.y"
                                           { 
          vector<shared_ptr<DeclAST>> temp;
          temp.push_back(shared_ptr<DeclAST>((yyvsp[-1].decl_val)));
          (yyval.arrayvardef_val) = new ArrayVarDef(unique_ptr<string>((yyvsp[-3].str_val)), temp); 
     }
#line 1599 "y.tab.c"
    break;

  case 22: /* ConstDef: ConstDefSingle  */
#line 162 "sysy.y"
                         { (yyval.var_val) = new VarDefAST(unique_ptr<SingleVarDef>((yyvsp[0].singlevardef_val))); }
#line 1605 "y.tab.c"
    break;

  case 23: /* ConstDef: ConstDefArray  */
#line 163 "sysy.y"
                        { (yyval.var_val) = new VarDefAST(unique_ptr<ArrayVarDef>((yyvsp[0].arrayvardef_val))); }
#line 1611 "y.tab.c"
    break;

  case 24: /* ConstDefSingle: IDENT TOK_ASSIGN InitVal  */
#line 166 "sysy.y"
                                         { (yyval.singlevardef_val) = new SingleVarDef(unique_ptr<string>((yyvsp[-2].str_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1617 "y.tab.c"
    break;

  case 25: /* ConstDefArray: DefArrayBody TOK_ASSIGN InitValArray  */
#line 169 "sysy.y"
                                                    {
     /* TODO */
     (yyval.arrayvardef_val) = (yyvsp[-2].arrayvardef_val);
     shared_ptr<ListAST> arr((yyvsp[0].list));
     (yyval.arrayvardef_val)->list = arr;
}
#line 1628 "y.tab.c"
    break;

  case 26: /* InitVal: AddExp  */
#line 177 "sysy.y"
                { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1634 "y.tab.c"
    break;

  case 27: /* InitValArray: TOK_LBRACE InitValArrayBody TOK_RBRACE  */
#line 179 "sysy.y"
                                                     {
          (yyval.list) = new ListAST();
          shared_ptr<ListAST> temp((yyvsp[-1].list));
          (yyval.list)->children.push_back(temp);
          (yyval.list)->isBracket = true;
     }
#line 1645 "y.tab.c"
    break;

  case 28: /* InitValArray: TOK_LBRACE TOK_RBRACE  */
#line 185 "sysy.y"
                                    {
          (yyval.list) = new ListAST();
          (yyval.list)->isBracket = true;
     }
#line 1654 "y.tab.c"
    break;

  case 29: /* InitValArrayBody: InitValArrayBody TOK_COMMA InitValArray  */
#line 191 "sysy.y"
                                                          {
                    (yyval.list) = new ListAST();
                    shared_ptr<ListAST> temp1((yyvsp[-2].list));
                    shared_ptr<ListAST> temp3((yyvsp[0].list));
                    (yyval.list)->children.push_back(temp1);
                    (yyval.list)->children.push_back(temp3);
                }
#line 1666 "y.tab.c"
    break;

  case 30: /* InitValArrayBody: InitValArrayBody TOK_COMMA InitVal  */
#line 198 "sysy.y"
                                                     {
                    (yyval.list) = new ListAST();
                    shared_ptr<ListAST> temp1((yyvsp[-2].list));
                    shared_ptr<DeclAST> temp3((yyvsp[0].decl_val));
                    shared_ptr<ListAST> child = make_shared<ListAST>(temp3);
                    (yyval.list)->children.push_back(temp1);
                    (yyval.list)->children.push_back(child);
                }
#line 1679 "y.tab.c"
    break;

  case 31: /* InitValArrayBody: InitValArray  */
#line 206 "sysy.y"
                               {
                    (yyval.list) = (yyvsp[0].list);
                }
#line 1687 "y.tab.c"
    break;

  case 32: /* InitValArrayBody: InitVal  */
#line 209 "sysy.y"
                          {
                    (yyval.list) = new ListAST();
                    shared_ptr<DeclAST> temp((yyvsp[0].decl_val));
                    shared_ptr<ListAST> child = make_shared<ListAST>(temp);
                    (yyval.list)->children.push_back(child);
                }
#line 1698 "y.tab.c"
    break;

  case 33: /* Exp: AddExp  */
#line 217 "sysy.y"
            { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1704 "y.tab.c"
    break;

  case 34: /* LOrExp: LOrExp TOK_OR LAndExp  */
#line 219 "sysy.y"
                              { (yyval.decl_val) = new ExprAST(OpType::OR, unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val)));}
#line 1710 "y.tab.c"
    break;

  case 35: /* LOrExp: LAndExp  */
#line 220 "sysy.y"
                { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1716 "y.tab.c"
    break;

  case 36: /* LAndExp: EqExp  */
#line 223 "sysy.y"
               { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1722 "y.tab.c"
    break;

  case 37: /* LAndExp: LAndExp TOK_AND EqExp  */
#line 224 "sysy.y"
                               { (yyval.decl_val) = new ExprAST(OpType::AND, unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1728 "y.tab.c"
    break;

  case 38: /* EqExp: RelExp  */
#line 227 "sysy.y"
              { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1734 "y.tab.c"
    break;

  case 39: /* EqExp: EqExp TOK_EQ RelExp  */
#line 228 "sysy.y"
                           { (yyval.decl_val) = new ExprAST(OpType::EQ, unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1740 "y.tab.c"
    break;

  case 40: /* EqExp: EqExp TOK_NEQ RelExp  */
#line 229 "sysy.y"
                            { (yyval.decl_val) = new ExprAST(OpType::NEQ, unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1746 "y.tab.c"
    break;

  case 41: /* RelExp: AddExp  */
#line 232 "sysy.y"
               { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1752 "y.tab.c"
    break;

  case 42: /* RelExp: RelExp RelOp AddExp  */
#line 233 "sysy.y"
                            { (yyval.decl_val) = new ExprAST((yyvsp[-1].optype_val), unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1758 "y.tab.c"
    break;

  case 43: /* AddExp: MulExp  */
#line 236 "sysy.y"
               { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1764 "y.tab.c"
    break;

  case 44: /* AddExp: AddExp AddOp MulExp  */
#line 237 "sysy.y"
                            { (yyval.decl_val) = new ExprAST((yyvsp[-1].optype_val), unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1770 "y.tab.c"
    break;

  case 45: /* MulExp: UnaryExp  */
#line 240 "sysy.y"
                 { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1776 "y.tab.c"
    break;

  case 46: /* MulExp: MulExp MulOp UnaryExp  */
#line 241 "sysy.y"
                              { (yyval.decl_val) = new ExprAST((yyvsp[-1].optype_val), unique_ptr<DeclAST>((yyvsp[-2].decl_val)), unique_ptr<DeclAST>((yyvsp[0].decl_val)));}
#line 1782 "y.tab.c"
    break;

  case 47: /* UnaryExp: PrimaryExp  */
#line 244 "sysy.y"
                     { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 1788 "y.tab.c"
    break;

  case 48: /* UnaryExp: FuncCall  */
#line 245 "sysy.y"
                   { (yyval.decl_val) = (yyvsp[0].funccall_val); }
#line 1794 "y.tab.c"
    break;

  case 49: /* UnaryExp: UnaryOp UnaryExp  */
#line 246 "sysy.y"
                           { 
        if((yyvsp[-1].optype_val) == OpType::PLUS) 
            (yyval.decl_val) = (yyvsp[0].decl_val);
        else
            (yyval.decl_val) = new ExprAST((yyvsp[-1].optype_val), unique_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1804 "y.tab.c"
    break;

  case 50: /* FuncCall: IDENT TOK_LPAREN FuncRParams TOK_RPAREN  */
#line 253 "sysy.y"
                                                  { (yyval.funccall_val) = new FuncCallAST(unique_ptr<string>((yyvsp[-3].str_val)), unique_ptr<vector<shared_ptr<DeclAST>>>((yyvsp[-1].funccall_params_val)));}
#line 1810 "y.tab.c"
    break;

  case 51: /* FuncCall: IDENT TOK_LPAREN TOK_RPAREN  */
#line 254 "sysy.y"
                                      { (yyval.funccall_val) = new FuncCallAST(unique_ptr<string>((yyvsp[-2].str_val))); }
#line 1816 "y.tab.c"
    break;

  case 52: /* PrimaryExp: TOK_LPAREN Exp TOK_RPAREN  */
#line 257 "sysy.y"
                                      { (yyval.decl_val) = (yyvsp[-1].decl_val); }
#line 1822 "y.tab.c"
    break;

  case 53: /* PrimaryExp: LVal  */
#line 258 "sysy.y"
                 { (yyval.decl_val) = (yyvsp[0].value_val); }
#line 1828 "y.tab.c"
    break;

  case 54: /* PrimaryExp: Number  */
#line 259 "sysy.y"
                   { (yyval.decl_val) = (yyvsp[0].value_val); }
#line 1834 "y.tab.c"
    break;

  case 55: /* PrimaryExp: STRING  */
#line 260 "sysy.y"
                   { (yyval.decl_val) = new ValueAST((unique_ptr<string>((yyvsp[0].str_val)))); }
#line 1840 "y.tab.c"
    break;

  case 56: /* LVal: LVal TOK_LBRACKET Exp TOK_RBRACKET  */
#line 263 "sysy.y"
                                         {
            if((yyvsp[-3].value_val)->var_def->IsSingle())
            {
                auto array_var_ast = new ValueAST(make_unique<ArrayVarDef>((make_unique<string>((yyvsp[-3].value_val)->GetName()))));
                delete (yyvsp[-3].value_val);
                (yyvsp[-3].value_val) = array_var_ast;
            }
          (yyval.value_val) = (yyvsp[-3].value_val);
          shared_ptr<DeclAST> exp_ptr((yyvsp[-1].decl_val));
          dynamic_pointer_cast<ArrayVarDef>((yyvsp[-3].value_val)->var_def)->dimension.push_back(exp_ptr);
    }
#line 1856 "y.tab.c"
    break;

  case 57: /* LVal: IDENT  */
#line 274 "sysy.y"
            { (yyval.value_val) = new ValueAST(make_unique<SingleVarDef>((unique_ptr<string>((yyvsp[0].str_val))))); }
#line 1862 "y.tab.c"
    break;

  case 58: /* FuncDef: TOK_VOID IDENT TOK_LPAREN FuncFParams TOK_RPAREN Block  */
#line 277 "sysy.y"
                                                                { (yyval.funcdef_val) = new FuncDefAST(ValueType::VOID, unique_ptr<string>((yyvsp[-4].str_val)), unique_ptr<vector<shared_ptr<VarDefAST>>>((yyvsp[-2].funcdef_params_val)), unique_ptr<BlockAST>((yyvsp[0].block_val))); }
#line 1868 "y.tab.c"
    break;

  case 59: /* FuncDef: TOK_VOID IDENT TOK_LPAREN TOK_RPAREN Block  */
#line 278 "sysy.y"
                                                    { (yyval.funcdef_val) = new FuncDefAST(ValueType::VOID, unique_ptr<string>((yyvsp[-3].str_val)), unique_ptr<BlockAST>((yyvsp[0].block_val))); }
#line 1874 "y.tab.c"
    break;

  case 60: /* FuncDef: BType IDENT TOK_LPAREN FuncFParams TOK_RPAREN Block  */
#line 279 "sysy.y"
                                                             { (yyval.funcdef_val) = new FuncDefAST((yyvsp[-5].vartype_val), unique_ptr<string>((yyvsp[-4].str_val)), unique_ptr<vector<shared_ptr<VarDefAST>>>((yyvsp[-2].funcdef_params_val)), unique_ptr<BlockAST>((yyvsp[0].block_val))); }
#line 1880 "y.tab.c"
    break;

  case 61: /* FuncDef: BType IDENT TOK_LPAREN TOK_RPAREN Block  */
#line 280 "sysy.y"
                                                 {(yyval.funcdef_val) = new FuncDefAST((yyvsp[-4].vartype_val), unique_ptr<string>((yyvsp[-3].str_val)), unique_ptr<BlockAST>((yyvsp[0].block_val))); }
#line 1886 "y.tab.c"
    break;

  case 62: /* FuncFParams: FuncFParams TOK_COMMA FuncFParam  */
#line 283 "sysy.y"
                                              { (yyval.funcdef_params_val) = (yyvsp[-2].funcdef_params_val); (yyval.funcdef_params_val)->push_back(shared_ptr<VarDefAST>((yyvsp[0].var_val)));}
#line 1892 "y.tab.c"
    break;

  case 63: /* FuncFParams: FuncFParam  */
#line 284 "sysy.y"
                        { (yyval.funcdef_params_val) = new vector<shared_ptr<VarDefAST>>(); (yyval.funcdef_params_val)->push_back(shared_ptr<VarDefAST>((yyvsp[0].var_val)));}
#line 1898 "y.tab.c"
    break;

  case 64: /* FuncRParams: FuncRParams TOK_COMMA AddExp  */
#line 287 "sysy.y"
                                          { (yyval.funccall_params_val) = (yyvsp[-2].funccall_params_val); (yyval.funccall_params_val)->push_back(shared_ptr<DeclAST>((yyvsp[0].decl_val)));}
#line 1904 "y.tab.c"
    break;

  case 65: /* FuncRParams: AddExp  */
#line 288 "sysy.y"
                    { (yyval.funccall_params_val) = new vector<shared_ptr<DeclAST>>(); (yyval.funccall_params_val)->push_back(shared_ptr<DeclAST>((yyvsp[0].decl_val))); }
#line 1910 "y.tab.c"
    break;

  case 66: /* FuncFParam: FuncFParamSingle  */
#line 291 "sysy.y"
                             { (yyval.var_val) = (yyvsp[0].var_val); }
#line 1916 "y.tab.c"
    break;

  case 67: /* FuncFParam: FuncFParamArray  */
#line 292 "sysy.y"
                            { (yyval.var_val) = (yyvsp[0].var_val); }
#line 1922 "y.tab.c"
    break;

  case 68: /* FuncFParamSingle: BType IDENT  */
#line 295 "sysy.y"
                              { (yyval.var_val) = new VarDefAST(unique_ptr<string>((yyvsp[0].str_val))); (yyval.var_val)->type = (yyvsp[-1].vartype_val); }
#line 1928 "y.tab.c"
    break;

  case 69: /* FuncFParamArray: BType IDENT TOK_LBRACKET TOK_RBRACKET  */
#line 298 "sysy.y"
                                                       { 
          shared_ptr<ArrayVarDef> var_def = make_shared<ArrayVarDef>(unique_ptr<string>((yyvsp[-2].str_val)));
          var_def->dimension.push_back(nullptr);
          (yyval.var_val) = new VarDefAST(var_def); 
          (yyval.var_val)->type = (yyvsp[-3].vartype_val); 
     }
#line 1939 "y.tab.c"
    break;

  case 70: /* FuncFParamArray: FuncFParamArray TOK_LBRACKET Exp TOK_RBRACKET  */
#line 304 "sysy.y"
                                                               { 
          (yyval.var_val) = (yyvsp[-3].var_val);
          (dynamic_pointer_cast<ArrayVarDef>((yyval.var_val)->var_defs[0]))->dimension.push_back(shared_ptr<DeclAST>((yyvsp[-1].decl_val)));
     }
#line 1948 "y.tab.c"
    break;

  case 71: /* Block: TOK_LBRACE TOK_RBRACE  */
#line 310 "sysy.y"
                             { (yyval.block_val) = new BlockAST(); }
#line 1954 "y.tab.c"
    break;

  case 72: /* Block: TOK_LBRACE BlockItems TOK_RBRACE  */
#line 311 "sysy.y"
                                        { (yyval.block_val) = new BlockAST(unique_ptr<vector<shared_ptr<StmtAST>>>((yyvsp[-1].blockitems_val)));}
#line 1960 "y.tab.c"
    break;

  case 73: /* BlockItems: BlockItem  */
#line 314 "sysy.y"
                      { (yyval.blockitems_val) = new vector<shared_ptr<StmtAST>>(); if((yyvsp[0].stmt_val)) (yyval.blockitems_val)->push_back(shared_ptr<StmtAST>((yyvsp[0].stmt_val))); }
#line 1966 "y.tab.c"
    break;

  case 74: /* BlockItems: BlockItems BlockItem  */
#line 315 "sysy.y"
                                 { (yyval.blockitems_val) = (yyvsp[-1].blockitems_val); if((yyvsp[0].stmt_val)) (yyval.blockitems_val)->push_back(shared_ptr<StmtAST>((yyvsp[0].stmt_val))); }
#line 1972 "y.tab.c"
    break;

  case 75: /* BlockItem: Decl  */
#line 318 "sysy.y"
                { (yyval.stmt_val) = (yyvsp[0].var_val); }
#line 1978 "y.tab.c"
    break;

  case 76: /* BlockItem: Stmt  */
#line 319 "sysy.y"
                { (yyval.stmt_val) = (yyvsp[0].stmt_val); }
#line 1984 "y.tab.c"
    break;

  case 77: /* Stmt: LVal TOK_ASSIGN Exp TOK_SEMICOLON  */
#line 322 "sysy.y"
                                        { (yyval.stmt_val) = new AssignmentAST( unique_ptr<ValueAST>((yyvsp[-3].value_val)), unique_ptr<DeclAST>((yyvsp[-1].decl_val))); }
#line 1990 "y.tab.c"
    break;

  case 78: /* Stmt: Exp TOK_SEMICOLON  */
#line 323 "sysy.y"
                        { (yyval.stmt_val) = (yyvsp[-1].decl_val); }
#line 1996 "y.tab.c"
    break;

  case 79: /* Stmt: TOK_SEMICOLON  */
#line 324 "sysy.y"
                    { (yyval.stmt_val) = new NullStmtAST(); }
#line 2002 "y.tab.c"
    break;

  case 80: /* Stmt: Block  */
#line 325 "sysy.y"
            { (yyval.stmt_val) = (yyvsp[0].block_val); }
#line 2008 "y.tab.c"
    break;

  case 81: /* Stmt: IfStmt  */
#line 326 "sysy.y"
             { (yyval.stmt_val) = (yyvsp[0].if_val); }
#line 2014 "y.tab.c"
    break;

  case 82: /* Stmt: WhileStmt  */
#line 327 "sysy.y"
                { (yyval.stmt_val) = (yyvsp[0].while_val); }
#line 2020 "y.tab.c"
    break;

  case 83: /* Stmt: BreakStmt  */
#line 328 "sysy.y"
                { (yyval.stmt_val) = (yyvsp[0].break_val); }
#line 2026 "y.tab.c"
    break;

  case 84: /* Stmt: ContinueStmt  */
#line 329 "sysy.y"
                   { (yyval.stmt_val) = (yyvsp[0].continue_val); }
#line 2032 "y.tab.c"
    break;

  case 85: /* Stmt: ReturnStmt  */
#line 330 "sysy.y"
                 { (yyval.stmt_val) = (yyvsp[0].return_val); }
#line 2038 "y.tab.c"
    break;

  case 86: /* IfStmt: TOK_IF TOK_LPAREN Cond TOK_RPAREN Stmt TOK_ELSE Stmt  */
#line 333 "sysy.y"
                                                             { (yyval.if_val) = new IfAST(unique_ptr<DeclAST>((yyvsp[-4].decl_val)), shared_ptr<StmtAST>((yyvsp[-2].stmt_val)), shared_ptr<StmtAST>((yyvsp[0].stmt_val)));}
#line 2044 "y.tab.c"
    break;

  case 87: /* IfStmt: TOK_IF TOK_LPAREN Cond TOK_RPAREN Stmt  */
#line 334 "sysy.y"
                                               { (yyval.if_val) = new IfAST(unique_ptr<DeclAST>((yyvsp[-2].decl_val)), shared_ptr<StmtAST>((yyvsp[0].stmt_val)));}
#line 2050 "y.tab.c"
    break;

  case 88: /* ReturnStmt: TOK_RETURN Exp TOK_SEMICOLON  */
#line 337 "sysy.y"
                                         { (yyval.return_val) = new ReturnAST(shared_ptr<DeclAST>((yyvsp[-1].decl_val))); }
#line 2056 "y.tab.c"
    break;

  case 89: /* ReturnStmt: TOK_RETURN TOK_SEMICOLON  */
#line 338 "sysy.y"
                                     { (yyval.return_val) = new ReturnAST(); }
#line 2062 "y.tab.c"
    break;

  case 90: /* WhileStmt: TOK_WHILE TOK_LPAREN Cond TOK_RPAREN Stmt  */
#line 341 "sysy.y"
                                                     { (yyval.while_val) = new WhileAST(unique_ptr<DeclAST>((yyvsp[-2].decl_val)), shared_ptr<StmtAST>((yyvsp[0].stmt_val)));}
#line 2068 "y.tab.c"
    break;

  case 91: /* BreakStmt: TOK_BREAK TOK_SEMICOLON  */
#line 343 "sysy.y"
                                   { (yyval.break_val) = new BreakAST();}
#line 2074 "y.tab.c"
    break;

  case 92: /* ContinueStmt: TOK_CONTINUE TOK_SEMICOLON  */
#line 345 "sysy.y"
                                         { (yyval.continue_val) = new ContinueAST();}
#line 2080 "y.tab.c"
    break;

  case 93: /* Cond: LOrExp  */
#line 347 "sysy.y"
             { (yyval.decl_val) = (yyvsp[0].decl_val); }
#line 2086 "y.tab.c"
    break;

  case 94: /* Number: TOK_INT_CONST  */
#line 349 "sysy.y"
                      { (yyval.value_val) = new ValueAST((yyvsp[0].int_val)); }
#line 2092 "y.tab.c"
    break;

  case 95: /* Number: TOK_FLOAT_CONST  */
#line 350 "sysy.y"
                      { (yyval.value_val) = new ValueAST((yyvsp[0].float_val)); }
#line 2098 "y.tab.c"
    break;

  case 96: /* AddOp: TOK_PLUS  */
#line 353 "sysy.y"
                { (yyval.optype_val) = OpType::PLUS; }
#line 2104 "y.tab.c"
    break;

  case 97: /* AddOp: TOK_MINUS  */
#line 354 "sysy.y"
                 { (yyval.optype_val) = OpType::MINUS; }
#line 2110 "y.tab.c"
    break;

  case 98: /* MulOp: TOK_MUL  */
#line 357 "sysy.y"
               { (yyval.optype_val) = OpType::MUL; }
#line 2116 "y.tab.c"
    break;

  case 99: /* MulOp: TOK_DIV  */
#line 358 "sysy.y"
               { (yyval.optype_val) = OpType::DIV; }
#line 2122 "y.tab.c"
    break;

  case 100: /* MulOp: TOK_MOD  */
#line 359 "sysy.y"
               { (yyval.optype_val) = OpType::MOD; }
#line 2128 "y.tab.c"
    break;

  case 101: /* UnaryOp: TOK_PLUS  */
#line 362 "sysy.y"
                  { (yyval.optype_val) = OpType::PLUS; }
#line 2134 "y.tab.c"
    break;

  case 102: /* UnaryOp: TOK_MINUS  */
#line 363 "sysy.y"
                   { (yyval.optype_val) = OpType::MINUS; }
#line 2140 "y.tab.c"
    break;

  case 103: /* UnaryOp: TOK_NOT  */
#line 364 "sysy.y"
                 { (yyval.optype_val) = OpType::NOT; }
#line 2146 "y.tab.c"
    break;

  case 104: /* RelOp: TOK_GE  */
#line 367 "sysy.y"
              { (yyval.optype_val) = OpType::GE; }
#line 2152 "y.tab.c"
    break;

  case 105: /* RelOp: TOK_GEQ  */
#line 368 "sysy.y"
               { (yyval.optype_val) = OpType::GEQ; }
#line 2158 "y.tab.c"
    break;

  case 106: /* RelOp: TOK_LE  */
#line 369 "sysy.y"
              { (yyval.optype_val) = OpType::LE; }
#line 2164 "y.tab.c"
    break;

  case 107: /* RelOp: TOK_LEQ  */
#line 370 "sysy.y"
               { (yyval.optype_val) = OpType::LEQ; }
#line 2170 "y.tab.c"
    break;

  case 108: /* IDENT: TOK_IDENT  */
#line 373 "sysy.y"
                 { (yyval.str_val) = (yyvsp[0].str_val); }
#line 2176 "y.tab.c"
    break;

  case 109: /* STRING: TOK_STRING  */
#line 375 "sysy.y"
                   { (yyval.str_val) = (yyvsp[0].str_val); }
#line 2182 "y.tab.c"
    break;


#line 2186 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (ast, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, ast);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (ast, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 378 "sysy.y"


