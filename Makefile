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

# Rota padrão
all: main

# Compilação padrão
main: lex.yy.c analisadorlexico

lex.yy.c: $(FLEXSRC)
	flex $(FLEXFLAGS) $(FLEXSRC)

# Compilação em modo Debug
debug: debug.lex.yy.c analisadorlexico

debug.lex.yy.c: $(FLEXSRC)
	flex $(DEBUG_FLEXFLAGS) $(FLEXSRC)

analisadorlexico: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)

# Executa apenas um teste
test:
	bash test.sh $(EXECUTABLE) tests/test.lua

# Executa diversos testes
tests:
	bash test.sh $(EXECUTABLE) tests/test.lua
	bash test.sh $(EXECUTABLE) tests/hello.lua

# Limpa o ambiente
clean:
	\rm -f *.yy.c *.o $(EXECUTABLE) *.out
