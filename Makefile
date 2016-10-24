# Compilador a ser usado
CC=gcc
# Flags para o compilador
CFLAGS=-Wall -Wextra -Wno-unused-function -Wno-sign-compare
CFLAGS_LEXICAL=-lfl
CFLAGS_PARSER=-lm
# Flags para o Linker
LDFLAGS=
# Flags para o Flex
FLEX_FLAGS=
DEBUG_FLEXFLAGS=--debug --verbose
# Flags para o Bison
BISON_FLAGS=
DEBUG_BISONFLAGS=--debug --verbose
# Nome do executavel
EXECUTABLE=compilador
# Executavel Analisador Léxico
LEXICAL_EXECUTABLE=analisador_lexico
# Executavel Analisador Semantico/Sintatico
PARSER_EXECUTABLE=analisador_semantico
# Todos os executaveis
EXECUTABLES=$(LEXICAL_EXECUTABLE) $(PARSER_EXECUTABLE) $(EXECUTABLE)
# Analisador Léxico
FLEX_SRC=analisador-lexico.l
# Analisador Sintatico/Semantico
BISON_SRC=analisador-semantico.y
# Arquivos a serem criados pelo FLEX
FLEX_HEADER_FILE=lex.yy.h
# Arquivos a serem criados pelo BISON
BISON_HEADER_FILE=y.tab.h
# Codigos fontes
SOURCES=$(FLEX_SRC) $(FLEX_HEADER_FILE) $(BISON_SRC) $(BISON_HEADER_FILE)
# Package files
PKG_FILES=lexical.defs.h analisador-lexico.l analisador-semantico.y parser.defs.h

# Rota padrão
all: main

# Pega os dois arquivos
# TODO Constroi o compilador
# Por enquanto o compilador vai ser o parser
main: parser lexical
	@printf "TODO: Constroi ultima parte do compilador\n"
	@printf "Compilation Finished With Success!\n"

# Compilação Analisador Léxico e Sintático/Semântico com flags de DEBUG
debug: parser-debug lexical-debug

# Compilação Analisador Sintático/Semântico
parser: lex.yy.c y.tab.c $(PARSER_EXECUTABLE)

# Compilação Bison
y.tab.c: $(BISON_SRC)
	bison $(BISON_SRC) -d -o y.tab.c

# Compilação Bison em modo Debug
parser-debug: debug.lex.yy.c debug.y.tab.c debug_parser_executable

# Compilação Bison Debug
debug.y.tab.c:
	bison $(BISON_SRC) -d -o debug.y.tab.c $(DEBUG_BISONFLAGS)

# Executavel com debug habilitado
debug_parser_executable: debug.lex.yy.c debug.y.tab.c
	$(CC) $(CFLAGS) -o $(PARSER_EXECUTABLE) -g debug.y.tab.c debug.lex.yy.c $(CFLAGS_PARSER) -D DEBUG_MODE

# Executavel Analisador Semantico/Sintatico
$(PARSER_EXECUTABLE): lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o $(PARSER_EXECUTABLE) y.tab.c lex.yy.c $(CFLAGS_PARSER)

# Compilação Analisador Léxico
lexical: lex.yy.c $(LEXICAL_EXECUTABLE)

# Compilação Flex
lex.yy.c: $(FLEX_SRC)
	flex $(FLEX_FLAGS) $(FLEX_SRC)

# Compilação Flex em modo Debug
lexical-debug: debug.lex.yy.c debug_lexical_executable

# Compilação Flex Debug
debug.lex.yy.c: $(FLEX_SRC)
	flex -o debug.lex.yy.c $(DEBUG_FLEXFLAGS) $(FLEX_SRC)

# Executavel de Debug Analisador Lexico
debug_lexical_executable: debug.y.tab.c debug.lex.yy.c
	$(CC) $(CFLAGS) debug.lex.yy.c -o $(LEXICAL_EXECUTABLE) -g $(CFLAGS_LEXICAL) -D LEXICAL_ANALYSER

# Executavel Analisador Lexico
$(LEXICAL_EXECUTABLE): y.tab.c lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c -o $(LEXICAL_EXECUTABLE) $(CFLAGS_LEXICAL) -D LEXICAL_ANALYSER

# Realiza os testes nos executaveis
check: lexical-test parser-test
#test: lexical-test parser-test

# Executa todos os testes
all-tests: lexical-all-tests parser-all-tests
	@printf "Finished All integration tests...\n"

# Executa o teste do analisador léxico
lexical-test:
	@bash test.sh $(LEXICAL_EXECUTABLE) tests/lexical/test.lua
	@printf "\n-- Finished Lexical Test --\n"

# Executa diversos testes no analisador léxico
lexical-all-tests:
	@bash test.sh $(LEXICAL_EXECUTABLE) tests/lexical/test.lua
	@printf "\n"
	@bash test.sh $(LEXICAL_EXECUTABLE) tests/lexical/hello.lua
	@printf "\n-- Finished Lexical All Tests --\n"

# TODO Executa testes unitários no parser
parser-test:
	@bash test.sh $(PARSER_EXECUTABLE) tests/parser/test.lua
	@printf "\n-- Finished Parser Test --\n"

# TODO Varios testes unitários para o parser
parser-all-tests:
	@bash test.sh $(PARSER_EXECUTABLE) tests/parser/test.lua
	@printf "\n"
	#@bash test.sh $(PARSER_EXECUTABLE) tests/parser/globals.lua
	#@printf "\n"
	@bash test.sh $(PARSER_EXECUTABLE) tests/parser/simple.lua
	@printf "\n-- Finished Parser All Tests-- \n"

# Parser extra
check-extra:
	rm -r tests/testing
	mkdir tests/testing
	ls tests/input | \
	sort |\
	grep -v '^$$' |\
	awk '{ \
			for(i = 1; i <= NF; i++) { \
				split($$1,a,"-"); \
				param2="testing-"a[2] ;\
				param3="output-"a[2] ; \
				command="./test.sh $(PARSER_EXECUTABLE) tests/input/"$$1" tests/testing/"param2" tests/output/"param3; \
				system(command); \
				print param3, param2, $$1 ; \
				print command; \
			} \
		}'
	
#./test.sh "$(PARSER_EXECUTABLE)" "tests/input/$$x" "tests/testing/testing-$$y" "tests/output/output-$$y"; \
# dessa saque só, meu problema é que eu tenho que quebrar aquele ls em varias linhas

# Cria pacote para enviar trabalho
package:
	@printf "Creating pkg zip with source code and base files ...\n"
	@\rm -rf pkg pkg.zip
	mkdir pkg
	cp $(PKG_FILES) pkg/
	cp pkgMakefile pkg/Makefile
	zip -j pkg.zip pkg/*

# Limpa o ambiente
clean:
	@printf "Cleaning project folder...\n"
	@\rm -rf *.yy.c *.yy.h *.tab.c *.tab.h *.o $(EXECUTABLES) *.out tests/parser/*.out tests/lexical/*.out *.output pkg pkg.zip

