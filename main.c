/******************************************************************
04/06/2016                   TrabOEA                    04/06/2016
-------------------------------------------------------------------
Programa..: TrabOEA
Autor.....: Jos� Am�rico Rodrigues
Finalidade: Pesquisa de registros em um arquivo n�o ordenado de CEPs
            atrav�s de tabela hash
Atualiza��es:
13/06/2016 - Retirada a op��o do menu para a pesquisa avan�ada.
******************************************************************/
#include "cep.h"
#include "util.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

int main() {
    int opcao;  // Guarda a op��o do menu

	setlocale(LC_ALL, "Portuguese");	//habilita a acentua��o para o portugu�s
    // Abertura dos arquivos
    abreCep();    						// Abre o arquivo de CEPs
    abreHash();   						// Abre o arquivo de ordena��o (hash)
    // Loop para continua��o da exibi��o do menu
    do {
        opcao = menu();
        switch(opcao){
            case 1: pesquisaHash();     break;
            case 2: estatisticasHash(); break;
            case 3: listaCep();			break;
            case 4: listaHash();		break;
            case 5: registroHash();     break;
            case 0: break;
            default:
                printf("Op��o inv�lida!\n");
                pausa();
                break;
        }
    } while(opcao != 0);  				//Condi��o de sa�da
    // Fechamento dos arquivos
    fechaCep();    						// Fecha o arquivo de CEPs
    fechaHash();   						// Fecha o arquivo de ordena��o (hash)
	return 0;
}
