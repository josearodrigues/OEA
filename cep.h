/******************************************************************
04/06/2016                     cep.h                    04/06/2016
-------------------------------------------------------------------
Arquivo...: cep.h
Autor.....: José Américo Rodrigues
Finalidade: Header para manuseio do arquivo nao ordenado de CEPs
Atualizações:
13/06/2016 - Retirada da variável e para uso local nas rotinas
******************************************************************/
#ifndef CEP_H_INCLUDED
#define CEP_H_INCLUDED

#include <stdio.h>

#define CEPFILE "cep.dat"

// Estrutura para a leitura dos registros do arquivo de CEPs
typedef struct {
	char endereco[73];
	char bairro[73];
	char cidade[73];
	char nomeEstado[73];
	char siglaEstado[3];
	char cep[9];
} Endereco;

int abreCep();
Endereco leCep(long pos);
int imprimeCep(long pos, Endereco reg);
int pesquisaListaCep(char cep[9]);
int fechaCep();
long ultregCep();

FILE *arqcep;   // Ponteiro para o arquivo não ordenado de CEPs
//Endereco e;     // Estrutura para guardar um registro

#endif // CEP_H_INCLUDED
