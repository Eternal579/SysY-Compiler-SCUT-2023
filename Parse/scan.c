#include"scan.h"
#include"global.h"

/* states int scanner DFA */
typedef enum
{
	START, INID, INHEX, ININOCT, INOCT, ININTEGER, INFLOAT, INLTEQ, INRTEQ, INAND, INOR, INEQ, INNEQ, INnote, INRnote, NOTE, INLRNOTE, LRNOTE, WRONGID, INSTRING, WRONGSTRING, DONE,
}StateType;

/* lexme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN = length of hte input buffer for source code lines */
#define BUFLEN 256

int lineno;
static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position int lineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_Flag = FALSE;

/* getNextChar fetches the next non-blank character from lineBuf,
reading in a new line if lineBuf is exhausted */
static int getNextChar(void) {
	if (!(linepos < bufsize)) {
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source)) {
			if (EchoSource)
				fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else {
			EOF_Flag = TRUE;
			return EOF;
		}
	}
	else
		return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character in lineBuf */
static void ungetNextChar(void) {
	if (!EOF_Flag)
		linepos--;
}

/* look up table of reserved words */
static struct {
	char* str;
	TokenType tok;
} reservedWords[MAXRESERVED] = {
	{"main",MAIN},{"int",INT},{"float",FLOAT},{"void",VOID},{"const",CONST},{"if",IF},
	{"else",ELSE},{"while",WHILE},{"break",BREAK},{"continue",CONTINUE},{"return",RETURN},{"string",STRING}
};

/* lookup an identifier to see if it is a reserved word */
static TokenType reservedLookup(char* s) {
	for (int i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

/* function getToken returns the next token in source file */
TokenType getToken(void) {
	/* index for storing into tokenString */
	int tokenStringIndex = 0;
	/* holds current token to be returned */
	TokenType currentToken;
	/* current state - always begins at START */
	StateType state = START;
	/* flag to indicate save to tokenString */
	int save;
	while (state != DONE) {
		int c = getNextChar();
		save = TRUE;
		if (c == '`' | c == '~' | c == '#' | c == '$' | c == '^' | c == ':') {
			currentToken = ILLEGALCH;
			save = FALSE;
			state = DONE;
		}
		switch (state) {
		case START:
			if (c == ' ' || c == '\t' || c == '\n')
				save = FALSE;
			else if (isalpha(c) || c == '_')
				state = INID;
			else if (isdigit(c) && c != '0')
				state = ININTEGER;
			else if (c == '0')
				state = ININOCT;
			else if (c == '<')
				state = INLTEQ;
			else if (c == '>')
				state = INRTEQ;
			else if (c == '&')
				state = INAND;
			else if (c == '|')
				state = INOR;
			else if (c == '!')
				state = INNEQ;
			else if (c == '=')
				state = INEQ;
			else if (c == '/')
				state = INnote;
			else if (c == '*')
				state = INRnote;
			else if (c == '\'')
				state = INSTRING;
			else {
				state = DONE;
				switch (c) {
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '%':
					currentToken = MOD;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case '{':
					currentToken = BGLPAREN;
					break;
				case '}':
					currentToken = BGRPAREN;
					break;
				case '[':
					currentToken = LBRK;
					break;
				case ']':
					currentToken = RBRK;
					break;
				case ',':
					currentToken = COMMA;
					break;
				default: /* important */
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INID:
			if (!isalpha(c) && !isdigit(c) && c != '_') {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case INHEX:
			if (!(isdigit(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')) {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = HEX;
			}
			break;
		case ININOCT:
			if (c == '.')
				state = INFLOAT;
			else if (isdigit(c))
				state = INOCT;
			else if (c == 'x' || c == 'X')
				state = INHEX;
			else if (isalpha(c)) {
				ungetNextChar();
				save = FALSE;
				state = WRONGID;
				currentToken = ERROR;
			}
			else {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = INTEGER;
			}
			break;
		case INOCT:
			if (!isdigit(c)) {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = OCT;
			}
			break;
		case ININTEGER:
			if (isdigit(c))
				state = ININTEGER;
			else if (c == '.')
				state = INFLOAT;
			else if (isalpha(c)) {
				ungetNextChar();
				save = FALSE;
				state = WRONGID;
			}
			else {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = INTEGER;
			}
			break;
		case INFLOAT:
			if (isdigit(c))
				state = INFLOAT;
			else {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = FLOATINGS;
			}
			break;
		case INLTEQ:
			state = DONE;
			if (c == '=') {
				currentToken = LTEQ;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = LT;
			}
			break;
		case INRTEQ:
			state = DONE;
			if (c == '=') {
				currentToken = RTEQ;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = RT;
			}
			break;
		case INAND:
			state = DONE;
			if (c == '&') {
				currentToken = AND;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = ERROR;
			}
			break;
		case INOR:
			state = DONE;
			if (c == '|') {
				currentToken = OR;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = ERROR;
			}
			break;
		case INNEQ:
			state = DONE;
			if (c == '=') {
				currentToken = NEQ;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = NOT;
			}
			break;
		case INEQ:
			state = DONE;
			if (c == '=') {
				currentToken = EQ;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = ASSIGN;
			}
			break;
		case INnote:
			state = DONE;
			if (c == '/') {
				state = NOTE;
			}
			else if (c == '*') {
				currentToken = Lnote;
				state = INLRNOTE;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = OVER;
			}
			break;
		case INRnote:
			state = DONE;
			if (c == '/') {
				currentToken = Rnote;
			}
			else {
				ungetNextChar();
				save = FALSE;
				currentToken = TIMES;
			}
			break;
		case NOTE:
			if (c == '\n') {
				ungetNextChar();
				save = FALSE;
				currentToken = COMMENTS;
				state = DONE;
			}
			break;
		case INLRNOTE:
			if (c == '*')
				state = LRNOTE;
			else if (c == EOF) {
				save = FALSE;
				state = DONE;
				currentToken = WRONGCOMMENT;
			}
			break;
		case LRNOTE:
			if (c == '/') {
				currentToken = COMMENTS;
				state = DONE;
			}
			else if (c == EOF) {
				save = FALSE;
				state = DONE;
				currentToken = WRONGCOMMENT;
			}
			else{
				state = LRNOTE;
			}
			break;
		case WRONGID:
			if (!(isalpha(c) || isdigit(c) || c == '_')) {
				ungetNextChar();
				state = DONE;
				save = FALSE;
				currentToken = ILLEGALID;
			}
			break;
		case INSTRING:
			if (c == '\n') {
				state = WRONGSTRING;
			}
			else if (c == '\'') {
				state = DONE;
				currentToken = STRINGS;
			}
			break;
		case WRONGSTRING:
			if (c == '\n') {
				state = DONE;
				save = FALSE;
				currentToken = WRONGSTRINGS;
			}
			break;
		case DONE:
			break;
		default: /* this should never happen */
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char)c;
		if (state == DONE) {
			tokenString[tokenStringIndex++] = '\0';
			if (currentToken == ID) {
				currentToken = reservedLookup(tokenString);
			}
		}
	}
	if (TraceScan) {
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;

	/* Not End */
}