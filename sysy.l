
%{

#include <cstdlib>
#include <string>
#include <cstring>
// 因为 Flex 会用到 Bison 中关于 token 的定义
// 所以需要 include Bison 生成的头文件

#ifndef __linux__
#include "sysy.tab.h"
#else
#include "y.tab.h"
#endif

using namespace std;

int line_count = 1;

%}


/* 空白符和注释 */
WhiteSpace    [ \t\n\r]
LineComment   "//"[^\n]*
MultiComment  \/\*(\*[^/]|[^*])*\*\/
String 	  \"([^\n\"]|.)*\"

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+
FloatConst1 [0-9]*"."[0-9]+([eE][+-]?[0-9]+)?
FloatConst2 [0-9]+"."([eE][+-]?[0-9]+)?
FloatConst3 [0-9]+[eE][+-]?[0-9]+
HexFloat1 "0"[xX][0-9a-fA-F]*"."[0-9a-fA-F]+[pP][+-]?[0-9]+
HexFloat2 "0"[xX][0-9a-fA-F]+"."?[pP][+-]?[0-9]+

%%

{WhiteSpace}    { if(yytext[0] == '\n') ++line_count; }
{LineComment}   { /* 忽略, 不做任何操作 */ }
{MultiComment}  { 
    for(int i = 0 ;i<strlen(yytext);i++)
    {
        if(yytext[i] == '\n')
            ++line_count;
    }
}

"const"    return TOK_CONST;
"int"      return TOK_INT;
"float"    return TOK_FLOAT;
","        return TOK_COMMA;
";"        return TOK_SEMICOLON;
"("        return TOK_LPAREN;
")"        return TOK_RPAREN;
"["        return TOK_LBRACKET;
"]"        return TOK_RBRACKET;
"{"        return TOK_LBRACE;
"}"        return TOK_RBRACE;
"void"      return TOK_VOID;
"if"      return TOK_IF;
"else"      return TOK_ELSE;
"while"      return TOK_WHILE;
"break"      return TOK_BREAK;
"continue"      return TOK_CONTINUE;
"return"      return TOK_RETURN;

"+"        return TOK_PLUS;
"-"        return TOK_MINUS;
"*"        return TOK_MUL;
"/"        return TOK_DIV;
"%"        return TOK_MOD;
">"        return TOK_GE;
">="        return TOK_GEQ;
"<"        return TOK_LE;
"<="        return TOK_LEQ;
"=="        return TOK_EQ;
"="        return TOK_ASSIGN;
"!="        return TOK_NEQ;

"&&"        return TOK_AND;
"||"        return TOK_OR;
"!"        return TOK_NOT; 


{Decimal}   {
  yylval.int_val = strtoll(yytext, NULL, 10);
  return TOK_INT_CONST;
}

{Hexadecimal}   {
  yylval.int_val = strtoll(yytext, NULL, 0);
  return TOK_INT_CONST;
}

{Octal}   {
  yylval.int_val = strtoll(yytext, NULL, 0);
  return TOK_INT_CONST;
}

{FloatConst1} {
  yylval.float_val = std::stof(std::string(yytext));
  return TOK_FLOAT_CONST;
}

{FloatConst2} {
  yylval.float_val = std::stof(std::string(yytext));
  return TOK_FLOAT_CONST;
}

{FloatConst3} {
  yylval.float_val = std::stof(std::string(yytext));
  return TOK_FLOAT_CONST;
}

{HexFloat1} {
  yylval.float_val = std::stof(std::string(yytext));
  return TOK_FLOAT_CONST;
}

{HexFloat2} {
  yylval.float_val = std::stof(std::string(yytext));
  return TOK_FLOAT_CONST;
}

{String} {yylval.str_val = new string(yytext); return TOK_STRING;}
{Identifier}       {yylval.str_val = new string(yytext); return TOK_IDENT;}
. { printf("[lexical error] invalid character(s): %s\n", yytext); }


%%

int yywrap(void)
{  
  return 1;    //如果返回0则表示要处理下一个输入流,返回1表示结束.
}

void yyerror(shared_ptr<CompUnitAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
