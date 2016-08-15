# Compilador a ser usado
CC=gcc
# Flags para o compilador
CFLAGS=-lfl
# Flags para o Linker
LDFLAGS=
# Flagas para o Flex
FLEXFLAGS=
DEBUG_FLEXFLAGS=--debug --verbose
# Codigo fonte
SOURCES=lex.yy.c
# Nome do executavel
EXECUTABLE=analisadorlexico
# Analisador Léxico
FLEXSRC=analisador-lexico.l

all: main

main: lex.yy.c analisadorlexico

lex.yy.c: $(FLEXSRC)
	flex $(FLEXFLAGS) $(FLEXSRC)

debug: debug.lex.yy.c analisadorlexico

debug.lex.yy.c: $(FLEXSRC)
	flex $(DEBUG_FLEXFLAGS) $(FLEXSRC)

analisadorlexico: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

test:
	bash test.sh $(EXECUTABLE)

clean:
	\rm -f *.yy.c *.o $(EXECUTABLE) *.out