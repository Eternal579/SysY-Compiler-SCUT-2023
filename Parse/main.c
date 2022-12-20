#include"scan.h"
/* allocate global variables */
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

main() {
	char pgm[200]; /* source code file name */
	printf("please input the source code file name\n");
	scanf("%s", pgm);
	source = fopen(pgm, "r");
	listing = stdout;
	fprintf(listing, "SysY COMPILATION\n");
	while (getToken() != ENDFILE);
}
/* Not End */