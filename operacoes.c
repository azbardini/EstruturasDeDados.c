#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes.h"
#include "lse.h"
#include "lde.h"

#define TAM_VET 100000

/// Função que retorna as consultas mais consultadas em uma determinada localidade
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas naquela localidade
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         char*               ->  Localidade desejada
///         int*                ->  Quantidade de consultas que devem ser retornadas
/// OUTPUT:
///         Consulta*           ->  Arvore com somente as qtdConsultas mais realizadas

//pensei em salvar todas as quantidades de acesso em um vetor, ordenar o vetor em decrescente, e
//depois passar em toda a arvore copiando os nodos que tivessem a quantidade igual a do
//inicio do vetor.

void consultasPorLocalidade(Consulta* arvore, char* cidade, int qtdConsultas)
{


    if(qtdConsultas == 0)
    {
        qtdConsultas = TAM_VET;
    }

    int vetor[TAM_VET], i, j; //vetor de ordenamento
    Qtdcons qtdCons[TAM_VET], aux;
    for (i = 0; i < TAM_VET; i++) //zera o vetor
    {
        vetor[i] = 0;
    }
    int contador = 0;

    //copia todas as quantidades de acesso de cada consulta da arvore pra um vetor
    contador = achaVetorRepsLocalidade(arvore, vetor, contador, cidade, qtdCons);

    //double_quick_sort(qtdCons, 0, TAM_VET-1);
    for(i=0; i<TAM_VET && (qtdCons+i)->termos != NULL; i++)
    {
        for(j=0; j<TAM_VET && (qtdCons+j)->termos != NULL; j++)
        {
            if((qtdCons+i)->qtd > (qtdCons+j)->qtd)
            {
                aux = *(qtdCons+i);
                *(qtdCons+i) = *(qtdCons+j);
                *(qtdCons+j) = aux;
            }
        }

    }

    for(i=0; i<TAM_VET && ((qtdCons+i)->qtd) != 0 && i < qtdConsultas; i++)
    {
        printf("%d ", (qtdCons+i)->qtd);
        printaLSE((qtdCons+i)->termos);
    }


    return;
}
int achaVetorRepsLocalidade(Consulta* arvore, int *vetor, int contador, char *cidade, Qtdcons *qtdCons)
{

    int temCidade = temCidadeNaLista(cidade, arvore->cidades);

    if(temCidade > 0)
    {

        //printf("%d - ", temCidade);
        (qtdCons+contador)->qtd = temCidade;

        //printaLSE(arvore->termos);
        (qtdCons+contador)->termos = arvore->termos;

        contador++;
    }

    if(arvore == NULL)
        return contador;

    /*if(temCidade != 0)
    {
        vetor[contador] = arvore->qtdeAcessos;
        (qtdCons+contador)->qtd = temCidade;
        //printf("%d - ", temCidade);
        (qtdCons+contador)->termos = arvore->termos;
        contador++;
    }
    */
    if(arvore->esq != NULL)
    {
        contador = achaVetorRepsLocalidade(arvore->esq, vetor, contador, cidade, qtdCons);
    }
    if(arvore->dir != NULL)
    {
        contador = achaVetorRepsLocalidade(arvore->dir, vetor, contador, cidade, qtdCons);
    }
    //printf("%d %d\n", contador, arvore->qtdeAcessos);
    return contador;
}

/// Função que retorna as consultas mais consultadas em todo o arquivo
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  Quantidade de consultas que devem ser retornadas
/// OUTPUT:
///         Consulta*           ->  Arvore com somente as qtdConsultas mais realizadas
void consultasArquivo(Consulta* arvore, int qtdConsultas)
{

    if(qtdConsultas == 0)
    {
        qtdConsultas = TAM_VET;
    }
    Consulta retorno[TAM_VET];//arvore de retorno
    int vezesRep = 0;
    int vetor[TAM_VET], i; //vetor de ordenamento
    int vetorOrdenado[TAM_VET];

    for (i = 0; i < TAM_VET; i++) //zera o vetor
    {
        vetor[i] = 0;
    }
    int contador = 0;


    //copia todas as quantidades de acesso de cada consulta da arvore pra um vetor
    contador = achaVetorReps(arvore, vetor, contador);
    //ordena o vetor de quantidades de acesso
    quick_sort(vetor, 0, TAM_VET-1);
    for(i=0; i<TAM_VET; i++)
    {
        vetorOrdenado[i] = vetor[TAM_VET-1-i];
    }

    //printa vetor ordenado
    /*for (i = 0; i < TAM_VET; i++)
    {
        printf("%d ", vetorOrdenado[i]);
    }
    printf("\n\n");*/

    //após isso, para cada posição do vetor ordenado por quantidade de acesso,
    //vai percorrer a árvore procurando pelos nodos cuja
    //quantidade de acesso sejam iguais as do vetor ordenado
    //salva esses nodos da arvore em um outro vetor de Consultas
    //printf("%d ", qtdAux);
    for (i = 0; i < TAM_VET && vetorOrdenado[i] != 0; i++)
    {
        if(i != 0)
        {
            if(vetorOrdenado[i-1] == vetorOrdenado[i])
                vezesRep++;
            else
                vezesRep = 0;
        }
        //printf("\n%d VEZESREP: %d \n", vetor[i], vezesRep);
        //printf("______________________________\n");

        copiaArvore(arvore, retorno, vetorOrdenado, qtdConsultas, i, vezesRep); //copiar os nodos com mais acesso para o vetor
    }
    //printf("%d \n\n", qtdAux);
    //no final, printa na tela (substituir por arquivo)
    //todas as consutas encontradas
    for (i = 0; i < qtdConsultas && vetorOrdenado[i] != 0; i++)
    {
        printf("%d ",(retorno+i)->qtdeAcessos);
        printaLSE((retorno+i)->termos);
    }
    return;
}


/// Copia a quantidade de acessos de cada nodo da arvore pra posições de um vetor
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  vetor onde sera armazenado as quantidades
///         int                 ->  posição do vetor, pra recursividade
/// OUTPUT:
///         int*                ->  Vetor com todas as quantidades de acesso
int achaVetorReps(Consulta* arvore, int *vetor, int contador)
{
    if(arvore == NULL)
        return contador;

    vetor[contador] = arvore->qtdeAcessos;
    contador++;
    if(arvore->esq != NULL)
    {
        contador = achaVetorReps(arvore->esq, vetor, contador);
    }
    if(arvore->dir != NULL)
    {
        contador = achaVetorReps(arvore->dir, vetor, contador);
    }
    //printf("%d %d\n", contador, arvore->qtdeAcessos);
    return contador;
}
/// DEVERIA MAS NAO FAZ:
/// Copia os nodos das arvore de consultas para a arvore de retorno utilizando o vetor ordenado
/// de quantidade de acessos de cada nodo;
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  vetor onde sera armazenado as quantidades
///         int                 ->  posição do vetor, pra recursividade
/// OUTPUT:
///         int*                ->  Vetor com todas as quantidades de acesso
int copiaArvore(Consulta* arvore, Consulta* retorno, int *vetor, int qtd, int pos, int vezesRep)
{
    //printf("Pos: %d  ---  QTD: %d  ---  VREP %d\n", pos, qtd, vezesRep);
    if(arvore == NULL)
    {
        return vezesRep;
    }
    if (pos < qtd)
    {
        if (vetor[pos] == arvore->qtdeAcessos)
        {
            //      printf("VEZES: %d\n", vezesRep);
            if(vezesRep <= 0)
            {

                retorno[pos] = *arvore;
                //printaLSE(arvore->termos);
                return 10000000;
            }
            else
            {
                --vezesRep;
            }
            //o problema com certeza é na inserção, pois as logicas do vetor estão certas
            //printf("%d", (retorno+pos)->qtdeAcessos);
            //printf("    Vet = ");
            //printaLSE((retorno+pos)->termos);
            //printf("    Arvore =");
            //printaLSE(arvore->termos);
        }
        if(arvore->esq != NULL && qtd != -1)
            vezesRep = copiaArvore(arvore->esq, retorno, vetor, qtd, pos, vezesRep);
        if(arvore->dir != NULL && qtd != -1)
            vezesRep = copiaArvore(arvore->dir, retorno, vetor, qtd, pos, vezesRep);
    }
    else
    {
        return vezesRep;
    }
    return vezesRep;
}

/// Função que retorna os termos mais consultados em uma determinada localidade
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas naquela localidade
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         char*               ->  Localidade desejada
///         int*                ->  Quantidade de termos que devem ser retornados
/// OUTPUT:
///         LDE*                ->  Lista duplamente encadeada contendo os qtdTermos termos mais pesquisados nessa localidade

LDE* termosPorLocalidade(Consulta* arvore, LDE *lista, char cidade[])
{
    int quantidade;

    if(arvore)
    {
        quantidade = temCidadeNaLista(cidade, arvore->cidades); //Quantidade de vezes que aparece a árvore nessa lista


        if(quantidade)
        {
            lista = insereTermosNodo(lista, arvore->termos, quantidade);
        }

        lista = termosPorLocalidade(arvore->esq, lista, cidade);
        lista = termosPorLocalidade(arvore->dir, lista, cidade);
    }

    return lista;
}


LDE* insereTermosNodo(LDE *lista, LSE* termos, int qtde){


    while(termos){
        lista = insereLDENumerico(lista, termos->termo, qtde);
        termos = termos->prox;
    }

    return lista;
}

/// Função que retorna os termos mais consultados em todo o arquivo
/// Se for passado 0 como qtdConsultas, retorna todas as consultas realizadas
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  Quantidade de termos que devem ser retornados
/// OUTPUT:
///         LDE*                ->  Lista duplamente encadeada contendo os qtdTermos termos mais pesquisados
LDE* termosArquivo(LDE* listaTermos)
{

    return listaTermos;

}

LDE* termosArquivoLocalidade(LDE* listaTermos, int qtdTermos, char* localidade)
{
    //printf("%d", qtdTermos);
    //printf("%s", cidade);
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

/// Função que retorna a média do tamanho das consultas realizadas em uma determinada localidade
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         char*               -> Localidade desejada
/// OUTPUT:
///         int                 -> Média de tamanho das consultas da localidade


int mediaTamanhoConsultasLocalidade(Consulta* arvore, char* cidade)
{

    int totalTermos = 0, totalConsultas = 0;

    // Função auxiliar que vai contar o total de termos e o total de consultas;
    auxiliarMediaTamanhoConsultasLocalidade(arvore, &totalTermos, &totalConsultas, cidade);

    // Faz a média divindo o total de termos pelo total de consultas
    return totalTermos / totalConsultas;
}

/// Calcula a quantidade total de consultas e a quantidade total de termos em uma localidade
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  Ponteiro para int que armazena o total de termos
///         int*                ->  Ponteiro para int que armazena o total de consultas
///         char*               ->  String da localidade
/// OUTPUT:
///         int                 -> Média de tamanho das consultas do arquivo
void auxiliarMediaTamanhoConsultasLocalidade(Consulta *arvore, int *totTermos, int *totConsultas, char* cidade)
{

    // Se tenho um nodo da arvore
    if (arvore)
    {
        if(temCidadeNaLista(cidade, arvore->cidades))
        {
            *totTermos += arvore->qtdeTermos;   // Somo ao total de termos, a qtde de termos desse nodo
            *totConsultas += 1;                 // Somo 1 ao total de nodos da arvore
        }
        auxiliarMediaTamanhoConsultasLocalidade(arvore->esq, totTermos, totConsultas, cidade);     // Recursão para avore esquerda
        auxiliarMediaTamanhoConsultasLocalidade(arvore->dir, totTermos, totConsultas, cidade);     // Recursão para arvore direita
    }
    return;
}


/// Função que 1 ou 0 se uma cidade estiver ou nao em uma lista, respectivamente
/// INPUT:
///         char                -> string da cidade
///         LDE*                -> lista de cidades
/// OUTPUT:
///         int                 -> 1 sim, 0 nao
int temCidadeNaLista(char* cidade, LDE* lista)
{
    LDE* auxiliar = lista;
    int quantidade = 0;
    if(auxiliar)
    {
        while(auxiliar->prox != lista && auxiliar->prox)
        {
            if(strcmp(auxiliar->nome,cidade) == 0)
            {
                quantidade = auxiliar->qtde;
                break;
            }
            auxiliar = auxiliar->prox;
        }
        if(strcmp(auxiliar->nome,cidade) == 0)  quantidade = auxiliar->qtde;
    }

    return quantidade;
}


/// Função que retorna a média do tamanho das consultas realizadas em todo o arquivo
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
/// OUTPUT:
///         int                 -> Média de tamanho das consultas do arquivo
int mediaTamanhoConsultasArquivo(Consulta* arvore)
{

    int totalTermos = 0, totalConsultas = 0;

    // Função auxiliar que vai contar o total de termos e o total de consultas;
    auxiliarMediaTamanhoConsultasArquivo(arvore, &totalTermos, &totalConsultas);

    // Faz a média divindo o total de termos pelo total de consultas
    return totalTermos / totalConsultas;
}


/**> FUNÇÕES AUXILIARES PARA AS ANTERIORES <*/

/// Calcula a quantidade total de consultas e a quantidade total de termos
/// INPUT:
///         Consulta*           ->  Árvore na qual estão armazenadas as consultas
///         int*                ->  Ponteiro para int que armazena o total de termos
///         int*                ->  Ponteiro para int que armazena o total de consultas
/// OUTPUT:
///         int                 -> Média de tamanho das consultas do arquivo
void auxiliarMediaTamanhoConsultasArquivo(Consulta *arvore, int *totTermos, int *totConsultas)
{

    // Se tenho um nodo da arvore
    if (arvore)
    {
        *totTermos += arvore->qtdeTermos;   // Somo ao total de termos, a qtde de termos desse nodo
        *totConsultas += 1;                 // Somo 1 ao total de nodos da arvore
        auxiliarMediaTamanhoConsultasArquivo(arvore->esq, totTermos, totConsultas);     // Recursão para avore esquerda
        auxiliarMediaTamanhoConsultasArquivo(arvore->dir, totTermos, totConsultas);     // Recursão para arvore direita
    }

    return;
}


void quick_sort(int *a, int left, int right)
{
    int i, j, x, y;

    i = left;
    j = right;
    x = a[(left + right) / 2];

    while(i <= j)
    {
        while(a[i] < x && i < right)
        {
            i++;
        }
        while(a[j] > x && j > left)
        {
            j--;
        }
        if(i <= j)
        {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }

    if(j > left)
    {
        quick_sort(a, left, j);
    }
    if(i < right)
    {
        quick_sort(a, i, right);
    }

}


void double_quick_sort(int *a, int *b, int left, int right)
{
    int i, j, x, y;

    i = left;
    j = right;
    x = a[(left + right) / 2];

    while(i <= j)
    {
        while(a[i] < x && i < right)
        {
            i++;
        }
        while(a[j] > x && j > left)
        {
            j--;
        }
        if(i <= j)
        {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            y = b[i];
            b[i] = b[j];
            b[j] = y;

            i++;
            j--;
        }
    }

    if(j > left)
    {
        double_quick_sort(a,b, left, j);
    }
    if(i < right)
    {
        double_quick_sort(a,b, i, right);
    }

}
