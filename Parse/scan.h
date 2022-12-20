#ifndef _SCAN_H
#define _SCAN_H

#include"global.h"
/* MAXTOKEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* Fuction getToken returns the next token in source file */
TokenType getToken(void);

/* maybe end? */
#endif