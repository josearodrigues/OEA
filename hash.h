/******************************************************************
04/06/2016                     hash.h                    04/06/2016
-------------------------------------------------------------------
Arquivo...: hash.h
Autor.....: Jos� Am�rico Rodrigues
Finalidade: Header para opera��es na tabela hash em arquivo
Atualiza��es:
13/06/2016 - Inclus�o de algumas rotinas
             Retirada da vari�vel ht para uso local nas rotinas
******************************************************************/
#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <stdio.h>

#define HASHFILE "cep_h.idx"
#define MAXHASH 900001
#define MAXCOLISOES 15

// Estrutura para a leitura dos registros da tabela hash
typedef struct {
    long CEP;		// CEP gravado
    long PosArq;	// Posi��o do CEP no arquivo de CEPs
    long Proximo;	// Pr�ximo CEP em colis�o
} HashTab;

int criaHash();
int indexaHash();
int pesquisaHash();
int estatisticasHash();
int listaHash();
int abreHash();
HashTab leHash(long pos);
int escreveHash(HashTab reg, long pos, int rel);
int fechaHash();
long ultregHash();
long calculaHash(long valor);
HashTab inicializaHash(long cep, long posarq, long proximo);
int registroHash();

FILE *arqhash;  // Ponteiro para o arquivo da tabela hash
// HashTab ht;   // Estrutura para o registro da tabela

#endif // HASH_H_INCLUDED
