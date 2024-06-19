/******************************************************************
04/06/2016                      hash                     04/06/2016
-------------------------------------------------------------------
Arquivo...: hash.c
Autor.....: Jos� Am�rico Rodrigues
Finalidade: Opera��es no arquivo da tabela hash
Atualiza��es:
13/06/2016 - Atualizadas algumas rotinas mas ainda com problemas
             na indexa��o (cria��o da tabela hash de dispers�o).
             Indexa at� o final, mas no meio l� posi��es de CEP
             zero que, at� ent�o, n�o existia.
           - Inclu�da precau��o contra CEPs duplicados.
           - Completada a rotina de pesquisa.
           - Criado uma arquivo de log para tentar desvendar o
             problema na cria��o da tabela de dispers�o.
******************************************************************/
#include <stdlib.h>
#include <math.h>
#include "hash.h"
#include "cep.h"
#include "util.h"
#pragma pack(1)

#define LOGFILE "cep_h.log"

/******************************************************************
Fun��o....: criaHash
Finalidade: Cria o arquivo zerado da tabela hash
******************************************************************/
int criaHash() {
	HashTab ht;     					// Estrutura para o registro da tabela
    int i;								// Vari�vel auxiliar
    long marco = trunc(MAXHASH / 50);	// Auxiliar para o marco da barra de progresso

    arqhash = fopen(HASHFILE, "w");							// Cria o arquivo
    arqhash = freopen(HASHFILE, "r+", arqhash);  			// Reabre-o para escrita
	printf("Criando o arquivo inicial de �ndices...   ");	// Exibe mensagem de cria��o
	printf("\n\n0%% ");										// Inicia a barra de progresso
    ht = inicializaHash(0, -1, -1);							// Inicializa os dados dos registros
    // Loop para inclus�o das linhas da tabela
    for (i = 0; i <= MAXHASH; i++) {
        escreveHash(ht, 0, SEEK_END);                		// Adiciona o registro no final do arquivo
        if (i % marco == 0) printf("%c", 187);     	 		// Exibe o andamento do processo
    }
    printf(" 100%%\n\n");   								// Finaliza a barra de progresso
    fechaHash();											// Fecha o arquivo de hash
	return 0;
}

/******************************************************************
Fun��o....: indexaHash
Finalidade: Indexa o arquivo da tabela hash atrav�s do arquivo de CEPs
******************************************************************/
int indexaHash() {
    FILE *arqlog;       // Ponteiro para o arquivo de log
	Endereco e;         // Estrutura para guardar um registro
    HashTab ht, htaux;   // Auxiliares para adicionar registros na tabela
    long h = 0;			// Posi��o na tabela hash
    long registro = -1;	// Posi��o atual do registro de CEPs
    long posfinal = 0;  // Final do arquivo de �ndices (hash)
    long marco;		    // Auxiliar para o marco da barra de progresso

	marco = trunc(ultregCep()/50);		// Calcula o marco para desenho da barra
    arqhash = fopen(HASHFILE, "r+");	// Abre o �ndice hash zerado
    arqlog = fopen(LOGFILE, "w");	    // Abre o arquivo de log
    abreCep();							// Abre o arquivo de CEPs
	printf("Indexando o arquivo de CEPs (hash)...     ");
	printf("\n\n0%% ");
    registro++;
    e = leCep(registro);   //L� o registro do arquivo de CEps
    fprintf(arqlog, "INICIO DO LOG\n\n***Registro: %ld***\nLido de CEPs - CEP: %s\n",
                    registro, e.cep);
    // Loop para circular pelos registros de CEPs
	while(strlen(e.cep) > 0) {
        h = calculaHash(atol(e.cep));  // Calcula a posi��o para a indexa��o
        fprintf(arqlog, "Hash calculado - Posi��o: %ld - CEP: %s %% 900001\n", h, e.cep);
        // Vai at� a linha calculada da tabela e guarda as informa��es contidas
        ht = leHash(h);
        fprintf(arqlog, "Lido do Hash - Posi��o: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                        h, ht.CEP, ht.PosArq, ht.Proximo);
        // Guarda as informa��es na vari�vel auxiliar
        htaux = inicializaHash(atol(e.cep), registro, -1);    // Novo CEP
        // Se n�o for um CEP repetido...
        if (ht.CEP != atol(e.cep)) {
            // Se n�o houver um CEP na linha...
            if ((ht.CEP == 0) || (ht.PosArq == -1)) {
                escreveHash(htaux, h, SEEK_SET);    // Guarda o CEP no arquivo hash
                fprintf(arqlog, "SEM COLIS�O\nGravado no Hash - Posi��o: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                                h, htaux.CEP, htaux.PosArq, htaux.Proximo);
            }
            // H� colis�o...
            else {
	            // 1� colis�o
	            if (ht.Proximo == -1) {
                	escreveHash(htaux, 0, SEEK_END);    // Adiciona o CEP no final do arquivo
                    // Guarda a posi��o da nova linha para sobrescrever o campo
                	// Proximo da linha onde houve a colis�o
                	posfinal = ultregHash(); //- 1;
                	fprintf(arqlog, "1� COLIS�O\nAdicionado no Hash - Posi��o: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                                    posfinal, htaux.CEP, htaux.PosArq, htaux.Proximo);
                	// Guarda a nova informa��o do arquivo de CEPs
                	htaux = inicializaHash(ht.CEP, ht.PosArq, posfinal);
                	escreveHash(htaux, h, SEEK_SET);
                    fprintf(arqlog, "Gravado no Hash - Posi��o anterior: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                                    h, htaux.CEP, htaux.PosArq, htaux.Proximo);
	            }
	            // H� v�rias colis�es
	            else {
                	htaux = inicializaHash(atol(e.cep), registro, ht.Proximo);
	                escreveHash(htaux, 0, SEEK_END);    // Adiciona o CEP no final do arquivo
	                // Guarda a posi��o da nova linha para sobrescrever o campo
	                // Proximo da linha onde houve a colis�o
	                posfinal = ultregHash(); //- 1;
                    fprintf(arqlog, "V�RIAS COLIS�ES\nAdicionado no Hash - Posi��o: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                                    posfinal, htaux.CEP, htaux.PosArq, htaux.Proximo);
	                // Grava a nova informa��o do arquivo de CEPs
                	htaux = inicializaHash(ht.CEP, ht.PosArq, posfinal);
	                escreveHash(htaux, h, SEEK_SET);
                    fprintf(arqlog, "Gravado no Hash - Posi��o anterior: %ld - CEP: %ld - PosArq: %ld - Prox: %ld\n",
                                    h, htaux.CEP, htaux.PosArq, htaux.Proximo);
//                    pausa();
	            }
            }
        }
        //if ((registro % 50 ==0) && (registro > 0)) pausa();
        if (registro % marco == 0) printf("%c", 187);	// Exibe o andamento do processo
        registro++;
        e = leCep(registro);   //L� os registros do arquivo de CEps
        fprintf(arqlog, "\n***Registro: %ld***\nLido de CEPs - CEP: %s\n", registro, e.cep);
    }
    printf(" 100%%\n\n");
    fclose(arqlog);
    fechaCep();		// fecha o arquivo de CEPs
    fechaHash();											// Fecha o arquivo de hash
	return 0;
}

/******************************************************************
Fun��o....: pesquisaHash
Finalidade: Consulta um elemento atrav�s da tabela de hash
******************************************************************/
int pesquisaHash() {
	Endereco e;     // Estrutura para guardar um registro
    HashTab ht;     // Auxiliares para adicionar registros na tabela
	char cep[9];    // Guarda o nome a ser pesquisado
	long h;         // Guarda o n�mero do registro
	long aux, prox;

    // Leitura via teclado do CEP a ser pesuisado
    printf("CEP a ser pesquisado: "); scanf("%s", cep);
 	printf("Procurando o CEP [%s]\n", cep);
 	aux = atol(cep);
 	abreHash();
    abreCep();							// Abre o arquivo de CEPs
    // Calcula a posi��o para pesquisar
    h = aux % MAXHASH;
    // Vai at� a linha calculada da tabela e guarda as informa��es contidas
    ht = leHash(h);
    printf("\n***Registro: %ld***\nLido de Hash - CEP: %ld - PosArq: %ld - Proximo: %ld\n", h, ht.CEP, ht.PosArq, ht.Proximo);
    while (ht.CEP != aux) {
        if (ht.Proximo == -1) break;
        prox = ht.Proximo;
        ht = leHash(prox);
        printf("\n***Registro: %ld***\nLido de Hash - CEP: %ld - PosArq: %ld - Proximo: %ld\n", prox, ht.CEP, ht.PosArq, ht.Proximo);
    }
    if (ht.CEP == aux) {
        e = leCep(ht.PosArq);
        printf("CEP %s encontrado!", cep);
        imprimeCep(ht.PosArq, e);
    }
    else {
        printf("CEP n�o encontrado!");
    }
	pausa();
    fechaCep();		// Fecha o arquivo de CEPs
    fechaHash();
	return 0;
}

/******************************************************************
Fun��o....: estatisticasHash
Finalidade: Estat�sticas gerais do arquivo de Hash
******************************************************************/
int estatisticasHash() {
	Endereco e;     // Estrutura para guardar um registro
	int *colisoes;
	long h;
    long registro = -1;
    long tamarqCep;   // Guarda o tamanho do arquivo
    long marco; 	// Auxiliar para o marco da barra de progresso
	long resultado[MAXCOLISOES + 2];
	long i;
	long somacolisoes = 0;
	long totalregCep;

    colisoes = malloc (MAXHASH * sizeof (int));
    if (colisoes == NULL) {
        printf ("Erro! Nao foi poss�vel alocar mem�ria para o vetor.\n");
        exit (EXIT_FAILURE);
    }
	marco = trunc(MAXHASH / 50);
    limpaTela();
 	printf("Zerando vetor...\n\n0%% ");
    for (i = 0; i <= MAXHASH; i++) {
        if (i < MAXCOLISOES + 2) resultado[i] = 0;
        colisoes[i] = 0;
        if (i % marco == 0) printf("%c", 187);	// Exibe o andamento do processo
    }
    printf(" 100%%\n");
	marco = trunc(ultregCep()/50);		// Calcula o marco para desenho da barra
    arqhash = fopen(HASHFILE, "r");	// Abre o �ndice hash zerado
    abreCep();							// Abre o arquivo de CEPs
  	printf("\nVerificando colis�es...\n\n0%% ");
    registro++;
    e = leCep(registro);
	while(strlen(e.cep) > 0) {
        h = calculaHash(atol(e.cep));
        colisoes[h]++;
        if (registro % marco == 0) printf("%c", 187);	// Exibe o andamento do processo
        registro++;
        e = leCep(registro);
    }
    tamarqCep = ftell(arqcep);
    totalregCep = tamarqCep / 300;
    fechaCep();		// Fecha o arquivo de CEPs
    fechaHash();
    printf(" 100%%\n");
 	marco = trunc(MAXHASH / 50);
  	printf("Gerando estatisticas...\n0%% ");
    for (i = 0; i <= MAXHASH; i++) {
        resultado[colisoes[i]]++;
        if (i % marco == 0) printf("%c", 187);	// Exibe o andamento do processo
    }
    printf(" 100%%\n\n");
    limpaTela();
 	// Exibe informa��es estat�sticas
    for (i = 0; i < MAXCOLISOES + 2; i++){
        if (i == 0) {
            printf("registros n�o ocupados: %ld de %ld\n", resultado[0], MAXHASH);
        } else if (i == 1) {
            printf("Nenhuma colis�o.......: %ld\n", resultado[1]);
        } else if (i == 2) {
            printf(" 1 colis�o............: %ld\n", resultado[i]);
        } else if (i > 2 && i < 11){
            printf(" %d colis�es...........: %ld\n", (i - 1), resultado[i]);
        } else {
            printf("%d colis�es...........: %ld\n", (i - 1), resultado[i]);
        }
        if (i > 1) somacolisoes = somacolisoes + resultado[i];
    }
 	printf("CEPs no arquivo: %ld ocupando %d bytes, totalizando %ld bytes\n", totalregCep, 300, tamarqCep);
    printf("Ocupa��o: %ld de %ld posi��es na tabela hash original\n", resultado[1], MAXHASH);
    printf("Taxa de ocupa��o: %.2f%%\n", ((float)resultado[1] / MAXHASH * 100));
    printf("Somat�rio de colis�es: %ld\n", somacolisoes);
    printf("Taxa de colis�es: %.2f%%", ((float)somacolisoes / totalregCep * 100));
 	// Exibe informa��es do arquivo de CEPs
    free(colisoes);
    pausa();
	return 0;
}

/******************************************************************
Fun��o....: listaHash
Finalidade: Lista a tabela Hash
******************************************************************/
int listaHash() {
    HashTab ht;   // Auxiliares para adicionar registros na tabela
    long registro = -1;

    limpaTela();
	printf("Listando o arquivo de �ndices...\n\n");
    registro++;
    abreHash();
    ht = leHash(registro);
    while(ht.CEP != "") {
        printf("\nRegistro: %ld - ", registro);
        printf("CEP: %ld - ", ht.CEP);
        printf("Posi��o: %ld - ", ht.PosArq);
        printf("Pr�ximo: %ld", ht.Proximo);
        registro++;
        ht = leHash(registro);
    }
    fechaHash();
	pausa();
	return 1;
}

/******************************************************************
Fun��o....: abreHash
Finalidade: Abre o arquivo da tabela hash
******************************************************************/
int abreHash() {
    // Verifica a exist�ncia do arquivo de hash e o abre. Caso n�o exista, o cria.
	arqhash = fopen(HASHFILE, "r");
	if(!arqhash) {
        fclose(arqhash);
    	limpaTela();   		// Limpa a tela
		criaHash();			// Cria o arquivo de hash
		indexaHash();		// Indexa atrav�s do arquivo de CEPs
	}
    return 0;
}

/******************************************************************
Fun��o....: leHash
Finalidade: Le um registro da tabela de Hash
******************************************************************/
HashTab leHash(long pos) {
    HashTab reg;	// Vari�vel para a guarda de um registro de hash

//    fseek(arqhash, pos * sizeof(HashTab), SEEK_SET);	// Posiciona o ponteiro na posi��o desejada
    fseek(arqhash, pos * 12, SEEK_SET);	// Posiciona o ponteiro na posi��o desejada
    memset(&reg, 0, 12);    				// Zera a vari�vel para guardar o hash
    fread(&reg, 12, 1, arqhash);			// L� o registro
//    fread(&reg.CEP, 1, sizeof(long), arqhash);
//    fread(&reg.PosArq, 1, sizeof(long), arqhash);
//    fread(&reg.Proximo, 1,  sizeof(long), arqhash);
    return reg;
}

/******************************************************************
Fun��o....: escreveHash
Finalidade: Grava um registro no arquivo da tabela de Hash
Par�metros: reg - Registro a ser gravado
            pos - Deslocamento do registro em rela��o ao in�cio do
                  arquivo de hash
            rel - a pesquisa � relativa ao in�cio do arquivo, � po-
                  si��o corrente ou ao fim do arquivo?
******************************************************************/
int escreveHash(HashTab reg, long pos, int rel) {
    fseek(arqhash, pos * 12, rel);		// Posiciona o ponteiro na posi��o desejada
    fwrite(&reg, 12, 1, arqhash);		// Grava o registro no arquivo
    return 1;
}

/******************************************************************
Fun��o....: fechaHash
Finalidade: Fecha o arquivo da tabela hash
******************************************************************/
int fechaHash() {
	fclose(arqhash);     // Fecha o arquivo de ordena��o (hash)
    return 1;
}

/******************************************************************
Fun��o....: ultregHash
Finalidade: Retorna a �ltima posi��o do arquivo de hash
******************************************************************/
long ultregHash() {
    return (ftell(arqhash) / 12);
}

/******************************************************************
Fun��o....: calculaHash
Finalidade: Retorna o c�lculo para a tabela de dispers�o
******************************************************************/
long calculaHash(long valor) {
	return (valor % MAXHASH);
}

/******************************************************************
Fun��o....: inicializaHash
Finalidade: Inicializa um registro da tabela de Hash
******************************************************************/
HashTab inicializaHash(long cep, long posarq, long proximo) {
    HashTab reg;	// Vari�vel para a guarda de um registro de hash

	memset(&reg, 0, 12);    						// Zera a vari�vel para guardar o hash
    reg.CEP = cep; reg.PosArq = posarq; reg.Proximo = proximo;	// Inicializa os dados dos registros
	return reg;
}

/******************************************************************
Fun��o....: registroHash
Finalidade: Visualiza um elemento da tabela de hash
******************************************************************/
int registroHash() {
    HashTab ht;     // Auxiliares para adicionar registros na tabela
	long h;         // Guarda o n�mero do registro

    // Leitura via teclado do registro a ser visualizado
    printf("registro a ser visualizado: "); scanf("%ld", &h);
    abreHash();
    // Vai at� a linha calculada da tabela e guarda as informa��es contidas
    ht = leHash(h);
    printf("\n***Registro: %ld***\nLido do Hash - CEP: %ld - PosArq: %ld - Proximo: %ld\n", h, ht.CEP, ht.PosArq, ht.Proximo);
	fechaHash();
	pausa();
	return 0;
}
