CC = g++
CFLAGS = -std=c++17

sysy: AST.o Symbol_Table.o y.tab.o lex.yy.o Code.o Utility.o ASMGenerate.o Simplify.o Analyze.o LiteralPool.o BasicBlocks.o Delete_Useless.o Unroll_Loop.o
	$(CC) $(CFLAGS) -o sysy AST.o Symbol_Table.o y.tab.o lex.yy.o Code.o Utility.o ASMGenerate.o Simplify.o Analyze.o LiteralPool.o BasicBlocks.o Delete_Useless.o Unroll_Loop.o -lfl

AST.o: AST.cpp
	$(CC) $(CFLAGS) -g -c AST.cpp

Symbol_Table.o: Symbol_Table.cpp
	$(CC) $(CFLAGS) -g -c Symbol_Table.cpp

y.tab.o: sysy.y
	bison -vdty sysy.y
	$(CC) $(CFLAGS) -g -c y.tab.c

lex.yy.o: sysy.l
	flex sysy.l
	$(CC) $(CFLAGS) -g -c lex.yy.c

Code.o: Code.cpp
	$(CC) $(CFLAGS) -g -c Code.cpp

Utility.o: Utility.cpp
	$(CC) $(CFLAGS) -g -c Utility.cpp

ASMGenerate.o: ASMGenerate.cpp
	$(CC) $(CFLAGS) -g -c ASMGenerate.cpp

Simplify.o: Simplify.cpp
	$(CC) $(CFLAGS) -g -c Simplify.cpp

Analyze.o: Analyze.cpp
	$(CC) $(CFLAGS) -g -c Analyze.cpp

LiteralPool.o: LiteralPool.cpp
	$(CC) $(CFLAGS) -g -c LiteralPool.cpp

BasicBlocks.o: BasicBlocks.cpp
	$(CC) $(CFLAGS) -g -c BasicBlocks.cpp

Delete_Useless.o: Delete_Useless.cpp
	$(CC) $(CFLAGS) -g -c Delete_Useless.cpp

Unroll_Loop.o: Unroll_Loop.cpp
	$(CC) $(CFLAGS) -g -c Unroll_Loop.cpp
clean:
	rm -f *.o sysy sysy.tab.* lex.yy.*

