# LUA MIPS COMPILER
## 1ª Parte - Analisador Léxico
---

### Desenvolvido por

* Jeferson Lima | @jefersonla | jefersonlimaa@dcc.ufba.br
* Jefferson Renê @jeffersonrnb

## Introdução

Este é a primeira etapa do processo de desenvolvimento do compilador da linguagem LUA
para o código de máquina MIPS, implementando um subset da linguagem LUA em sua atual
versão 5.3.

Este projeto contém o analisador léxico em formato FLEX com integração com o compilador
GCC.

## Instruções para build

Para facilitar o processo de compilação deste projeto foi criado um arquivo Makefile
aonde as principais rotas para execução estão definidas abaixo, este programa contém um
único arquivo flex de nome 'analisador-lexico.l'. A rota principal é 'main'.

* **main** 	    : Compila o código com as flags padrões
* **debug**     : Compila o código com as flags de debug do flex
* **test**      : Executa o teste básico de integração
* **all-tests** : Executa todos os testes de integração

## Supporte & Documentação

Para maiores informações entrar em contato através do e-mail jefersonlimaa@dcc.ufba.br ou
je.myandroid@gmail.com.
Detalhes sobre a liguagem lua em http://lua.org e sobre o código de máquina MIPS em
http://logos.cs.uic.edu/366/notes/mips%20quick%20tutorial.htm .
Desenvolvido no Departamento de Ciência da Computação da Universidade Federal da Bahia.