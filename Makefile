# Compilador a ser usado
CC=gcc
# Flags para o compilador
CFLAGS=-lfl
# Flags para o Linker
LDFLAGS=
# Flagas para o Flex
FLEXFLAGS=
# Codigo fonte
SOURCES=lex.yy.c
# Nome do executavel
EXECUTABLE=compilador
# Analisador Léxico
FLEXSRC=analisador-lexico.l

all: main

main: lex.yy.c
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

lex.yy.c: $(FLEXSRC)
	flex $(FLEXSRC)

test:
	bash $(EXECUTABLE)

clean:
	rm *.yy.c *.o $(EXECUTABLE)