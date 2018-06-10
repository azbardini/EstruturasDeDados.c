#include <stdlib.h>
#include <string.h>
#include "operacoes.h"

/// Fun��o que retorna as consultas mais consultadas em uma determinada localidade
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas naquela localidade
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         char*               ->  Localidade desejada
///         int*                ->  Quantidade de consultas que devem ser retornadas
/// OUTPUT:
///         Consulta*           ->  Arvore com somente as qtdConsultas mais realizadas
void consultasPorLocalidade(Consulta* arvore, char* cidade, int qtdConsultas)
{

    return;
}

/// Fun��o que retorna as consultas mais consultadas em todo o arquivo
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         int*                ->  Quantidade de consultas que devem ser retornadas
/// OUTPUT:
///         Consulta*           ->  Arvore com somente as qtdConsultas mais realizadas
void consultasArquivo(Consulta* arvore, int qtdConsultas)
{

    return;
}

/// Fun��o que retorna os termos mais consultados em uma determinada localidade
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas naquela localidade
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         char*               ->  Localidade desejada
///         int*                ->  Quantidade de termos que devem ser retornados
/// OUTPUT:
///         LDE*                ->  Lista duplamente encadeada contendo os qtdTermos termos mais pesquisados nessa localidade
void termosPorLocalidade(Consulta* arvore, char* cidade, int qtdTermos)
{

    return;
}

/// Fun��o que retorna os termos mais consultados em todo o arquivo
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         int*                ->  Quantidade de termos que devem ser retornados
/// OUTPUT:
///         LDE*                ->  Lista duplamente encadeada contendo os qtdTermos termos mais pesquisados
LDE* termosArquivo(LDE* listaTermos, int qtdTermos)
{

    int i;
    LDE *listaRetorno, *novo, *auxiliar;

    if(qtdTermos == 0)
    {
        listaRetorno = listaTermos;
    }
    else
    {

        // Cria o primeiro nodo
        novo = (LDE*)malloc(sizeof(LDE));
        novo->ant = NULL;
        novo->prox = NULL;
        novo->qtde = listaTermos->qtde;
        strcpy(novo->nome, listaTermos->nome);
        listaRetorno = novo;

        // Seta o auxiliar
        auxiliar = listaRetorno;

        // Insiro os qtdTermos - 1 nodos restantes
        for(i=1, listaTermos = listaTermos->prox; i<qtdTermos; i++, auxiliar=novo, listaTermos = listaTermos->prox)
        {
            if (listaTermos == NULL) // Ja passei a lista inteira
                break;

            // Cria o nodo
            novo = (LDE*)malloc(sizeof(LDE));
            novo->ant = auxiliar;
            novo->prox = NULL;
            novo->qtde = listaTermos->qtde;
            strcpy(novo->nome, listaTermos->nome);
            auxiliar->prox = novo;
        }
    }



    return listaRetorno;
}

/// Fun��o que retorna a m�dia do tamanho das consultas realizadas em uma determinada localidade
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         char*               -> Localidade desejada
/// OUTPUT:
///         int                 -> M�dia de tamanho das consultas da localidade
int mediaTamanhoConsultasLocalidade(Consulta* arvore, char* cidade)
{

    return 0;
}


/// Fun��o que retorna a m�dia do tamanho das consultas realizadas em todo o arquivo
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
/// OUTPUT:
///         int                 -> M�dia de tamanho das consultas do arquivo
int mediaTamanhoConsultasArquivo(Consulta* arvore)
{

    int totalTermos = 0, totalConsultas = 0;

    // Fun��o auxiliar que vai contar o total de termos e o total de consultas;
    auxiliarMediaTamanhoConsultasArquivo(arvore, &totalTermos, &totalConsultas);

    // Faz a m�dia divindo o total de termos pelo total de consultas
    return totalTermos / totalConsultas;
}


/**> FUN��ES AUXILIARES PARA AS ANTERIORES <*/

/// Calcula a quantidade total de consultas e a quantidade total de termos
/// INPUT:
///         Consulta*           ->  �rvore na qual est�o armazenadas as consultas
///         int*                ->  Ponteiro para int que armazena o total de termos
///         int*                ->  Ponteiro para int que armazena o total de consultas
/// OUTPUT:
///         int                 -> M�dia de tamanho das consultas do arquivo
void auxiliarMediaTamanhoConsultasArquivo(Consulta *arvore, int *totTermos, int *totConsultas)
{

    // Se tenho um nodo da arvore
    if (arvore)
    {
        *totTermos += arvore->qtdeTermos;   // Somo ao total de termos, a qtde de termos desse nodo
        *totConsultas += 1;                 // Somo 1 ao total de nodos da arvore
        auxiliarMediaTamanhoConsultasArquivo(arvore->esq, totTermos, totConsultas);     // Recurs�o para avore esquerda
        auxiliarMediaTamanhoConsultasArquivo(arvore->dir, totTermos, totConsultas);     // Recurs�o para arvore direita
    }

    return;
}
