# TrabOEA
--------------------------------------
Trabalho de OEA
--------------------------------------
Trabalho da disciplina Organização e Estrutura de Arquivos ministrada pelo prof Renato Mauro no 
Centro Federal de Tecnologia do Rio de Janeiro na unidade Maracanã (CEFET-RJ).

A finalidade deste trabalho é o de aprendizado na manipulação de estruturas de indexação arquivos por meio de tabelas de dispersão (hash).

--------------------------------------
Comandos para compilação e linkedição:
--------------------------------------
gcc.exe -c cep.c -o cep.o

gcc.exe -c hash.c -o hash.o

gcc.exe -c main.c -o main.o

gcc.exe -c util.c -o util.o

gcc.exe cep.o hash.o main.o util.o -o OEA.exe