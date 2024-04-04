bison -vdty sysy.y
flex sysy.l
g++ -o sysy Analyze.cpp ASMGenerate.cpp AST.cpp Code.cpp Constants.cpp LiteralPool.cpp Simplify.cpp Symbol_Table.cpp  Utility.cpp y.tab.c lex.yy.c
.\sysy.exe .\test.txt