#ifndef _GLOBAL_H
#define _GLOBAL_H

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 12

typedef enum {
	/* book-keeping tokens */
	ENDFILE, ERROR,
	/* reserved words */
	MAIN, INT, FLOAT, VOID, CONST, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN, STRING,
	/* multicharacter tokens */
	ID, INTEGER, FLOATINGS, OCT, HEX, STRINGS, COMMENTS,
	/* special symbols */
	/*=, +, -, *, /, %, ==, !=, <, >, <=, >=, !, &&, ||, (, ), ;, //, /*, * /, {, }, [, ], ,*/
	ASSIGN, PLUS, MINUS, TIMES, OVER, MOD, EQ, NEQ, LT, RT, LTEQ, RTEQ, NOT, AND, OR, LPAREN, RPAREN, SEMI, note, Lnote, Rnote, BGLPAREN, BGRPAREN, LBRK, RBRK, COMMA,
	/*Wrong Token*/
	ILLEGALCH, ILLEGALID, WRONGSTRINGS, WRONGCOMMENT
}TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno;/* source line number for listing */

/* Not End */

/*  Flags for tracing  */

/*EchoSource = TRUE causes information to be
printed to the listing file as each token is recognized by
the scanner*/
extern int EchoSource;

extern int TraceScan;

#endif