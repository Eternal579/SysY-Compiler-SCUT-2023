#include"global.h"
#include"util.h"

void printToken(TokenType token, const char* tokenString) {
	switch (token) {
	case MAIN:
	case INT:
	case FLOAT:
	case VOID:
	case CONST:
	case IF:
	case ELSE:
	case WHILE:
	case BREAK:
	case CONTINUE:
	case RETURN:
	case STRING:
		fprintf(listing, "Reserved Words, name = %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name= %s\n", tokenString);
		break;
	case INTEGER:
		fprintf(listing, "INTEGER, val= %s\n", tokenString);
		break;
	case FLOATINGS:
		fprintf(listing, "FLOATINGS, val= %s\n", tokenString);
		break;
	case OCT:
		fprintf(listing, "OCT, val= %s\n", tokenString);
		break;
	case HEX:
		fprintf(listing, "HEX, val= %s\n", tokenString);
		break;
	case STRINGS:
		fprintf(listing, "STRING, val= %s\n", tokenString);
		break;
	case COMMENTS:
		fprintf(listing, "COMMENTS, val= %s\n", tokenString);
		break;
	case ASSIGN:fprintf(listing, "=\n"); break;
	case PLUS:fprintf(listing, "+\n"); break;
	case MINUS:fprintf(listing, "-\n"); break;
	case TIMES:fprintf(listing, "*\n"); break;
	case OVER:fprintf(listing, "/\n"); break;
	case MOD:fprintf(listing, "%%\n"); break;
	case EQ:fprintf(listing, "==\n"); break;
	case NEQ:fprintf(listing, "!=\n"); break;
	case LT:fprintf(listing, "<\n"); break;
	case RT:fprintf(listing, ">\n"); break;
	case LTEQ:fprintf(listing, "<=\n"); break;
	case RTEQ:fprintf(listing, ">=\n"); break;
	case NOT:fprintf(listing, "!\n"); break;
	case AND:fprintf(listing, "&&\n"); break;
	case OR:fprintf(listing, "||\n"); break;
	case LPAREN:fprintf(listing, "(\n"); break;
	case RPAREN:fprintf(listing, ")\n"); break;
	case SEMI:fprintf(listing, ";\n"); break;
	case BGLPAREN:fprintf(listing, "{\n"); break;
	case BGRPAREN:fprintf(listing, "}\n"); break;
	case LBRK:fprintf(listing, "[\n"); break;
	case RBRK:fprintf(listing, "]\n"); break;
	case COMMA:fprintf(listing, ",\n"); break;
	case ENDFILE:fprintf(listing, "EOF\n"); break;
	case ILLEGALCH:printf("Illegal character, not in the character set\n"); break;
	case ILLEGALID:printf("Wrong start of identifier\n"); break;
	case WRONGSTRINGS:printf("The string is missing a closing quote or cannot have a newline\n"); break;
	case WRONGCOMMENT:printf("Annotation missing right border symbol\n"); break;
	case ERROR:
		fprintf(listing, "ERROR %s\n", tokenString);
		break;
	default: /* this should never happen */
		fprintf(listing, "Unknown token: %d\n", token);
	}
}