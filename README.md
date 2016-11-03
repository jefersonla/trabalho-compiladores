# LUA MIPS COMPILER
## 1ª Parte - Analisador Léxico - Completa
## 2ª Parte - Analisador Sintático e Semântico - Completa
## 3ª Parte - Gerador de Código - Finalizando
---

### Desenvolvido por

* Jeferson Lima | @jefersonla | jefersonlimaa@dcc.ufba.br
* Jefferson Renê @jeffersonrnb

## Introdução

### Version 0.1
Compilador da linguagem LUA para o código de máquina MIPS, implementando um subset da
linguagem LUA formalizado pelo professor doutor Vinicius Petrucci, da Universidade Federal da Bahia.

### Version 0.2
Implementação do Analisador Sintático e Semântico. Foram adicionados os arquivos do Bison
com nome analisador-semantico.y, e foram feitas as integrações com o arquivo prévio do Flex.

Este projeto contém o analisador léxico em formato Flex e analisador sintático em formato Bison
com integração com o compilador GCC.

### Version 0.3

Implmenetação do analisador sintatico, com alguns bindings para analise semantica

### Version 0.4

Impelementadas AST e iniciado geração de código

### Version 0.6

Implementação do geardor de código, ainda instável devido a leaks of memory visualizados pelo
Valgrind, com integração com o GDB.

## Instruções para build

Para facilitar o processo de compilação deste projeto foi criado um arquivo Makefile
aonde as principais rotas para execução estão definidas abaixo. A rota principal é 'main'.

Para compilar o projeto todo basta usar a rota principal main, ou apenas executar o make.

### Geral

* **main** 	            : Compila todo o projeto com as flags padrões
* **debug**             : Compila todo o projeto com as flags de debug do flex
* **check**             : Realiza a checagem básica de execução do código
* **all-tests**         : Executa todos os testes de integração

### Lexico

* **lexical**           : Compila apenas o léxico
* **lexical-debug**     : Compila o analisador léxico com flags de debug
* **lexical-test**      : Realiza o teste de integração básico do analisador léxico
* **lexical-all-tests** : Realiza todos os teste de integração para o analisador léxico

### Sintático/Semântico

* **parser**            : Compila apenas o analisador sintático semântico
* **parser-debug**      : Compila o analisador sintático semântico com flags de debug
* **parser-test**       : Realiza o teste de integração básico do analisador sintático/semântico
* **parser-all-tests**  : Realiza todos os teste de integração para o analisador sintático/semântico
* **check-extra** 		: Realiza testes exaustivos de integração

### Gerador de código

* **codegen**			: Compila apenas o gerador de código
* **codegen-debug** 	: Compila o gerador de código com flags de debug
* **codegen-test** 		: Realiza o teste básico de integração para o gerador de código com o spim
* **codegen-all-tests** : Realiza todos os testes de integração para o gerador de código -- TODO!
* **check-final** 		: Realiza testes exaustivos de integração -- TODO!

Os arquivos a serem criados pelas rotas tem nome `analisador_lexico`, `analisador_semantico`
e `gerador_codigo`, aonde cada função utiliza das funções presentes no projeto anterior,
logo analisador_semantico implementa analisador_lexico também e consequentemente gerador_codigo
implementa as duas ações prévias.

## Supporte & Documentação

Para maiores informações entrar em contato através do e-mail jefersonlimaa@dcc.ufba.br ou
je.myandroid@gmail.com.

Detalhes sobre a liguagem lua em http://lua.org e sobre o código de máquina MIPS em
http://logos.cs.uic.edu/366/notes/mips%20quick%20tutorial.htm .
Desenvolvido no Departamento de Ciência da Computação da Universidade Federal da Bahia
como requisito parcial da disciplina MATA-61 - Compiladores (https://sites.google.com/site/mata61ufba://sites.google.com/site/mata61ufba/).

Professor Vinicius Petrucci (http://homes.dcc.ufba.br/~petrucci/).

#### The english version of this readme will coming soon.
