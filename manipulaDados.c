#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "manipulaDados.h"
#include "manipulaString.h"
#include "lde.h"
#include "lse.h"
#include "operacoes.h"

/// Função responsável por receber todos os dados do arquivo de entrada e realizar a inserção deles na estrutura correspondente
/// INPUT:
///         FILE*                ->  Ponteiro para o arquivo com os dados de entrada
/// OUTPUT:
///         Info*                ->  Estrutura que contem todas as informações recebidas
Info* entradaDados(FILE* entrada)
{
    int qtdTermos = 0;
    char str[601], cidade[100], *termo = NULL;
    Info *dados;
    LSE* listaTermos;

    // Forçando alocação
    do{
        dados = (Info*)malloc(sizeof(Info));
    }while(dados == NULL);

    // Inicialização dos dados
    dados->arvore = criaArvore();
    dados->termos = inicializaLDE();

    //printf("DADOS DE ENTRADA: \n");
    while(fgets(str, 600, entrada))
    {
        // String parse
        converteAcentos(str);
        removeCaracteres(str);

        /* PEGA A CIDADE */
        strcpy(cidade, strtok(str, ";"));

        /* PEGA OS TERMOS */
        listaTermos = inicializaLSE();                          // Zera a lista de termos
        qtdTermos = 0;                                          // Zera os termos
        while((termo = strtok(NULL, ";")))                      // Insere todos os termos da str na lista
        {
            if(termo[strlen(termo) - 1] == '\n')                // Tira o \n caso ele exista
                termo[strlen(termo) - 1] = '\0';

            listaTermos = insereLSE(listaTermos, termo);                // Insere o termo na sua lista
            dados->termos = insereLDENumerico(dados->termos, termo, 1);    // Insere o termo na lista geral
            qtdTermos++;                                                // Incrementa o contador de termos
        }
        dados->arvore = insereNodoArvore(dados->arvore, listaTermos, qtdTermos, cidade);
    }

    return dados;
}


/// Função responsável por realizar todas as operações contidas no arquivo de operações utilizando a árvore que possui todos os dados
/// INPUT:
///         FILE*                ->  Ponteiro para o arquivo com as operações a serem realizadas na árvore
///         Info*                ->  Estrutura contendo toda a informação necessária
void realizaOperacoes(FILE* operacoes, FILE* saida, Info* dados)
{
    char str[201], localidade[100], operacao;
    LDE *termos;
    int tamanho, qtdTermos; //Usado nas operações 'e' & 'f'

    while(fgets(str, 200, operacoes))
    {
        converteAcentos(str);
        removeCaracteres(str);
        operacao =  *(strtok(str, ";"));

        // Operacoes de consulta no arquivo
        switch(operacao)
        {
        case 'a':
            //Recebe a localidade e a quantidade de consultas
            strcpy(localidade,strtok(NULL, ";"));
            qtdTermos = atoi(strtok(NULL, ";"));

            printf("\n%d consultas mais realizadas em %s: \n", qtdTermos, localidade);
            consultasPorLocalidade(dados->arvore, localidade, qtdTermos);
            break;
        case 'b':
            //Recebe a quantidade de consultas
            qtdTermos = atoi(strtok(NULL, ";"));

            printf("\n%d consultas mais realizadas no arquivo:\n", qtdTermos);
            consultasArquivo(dados->arvore, qtdTermos);
            break;
        case 'c':
            //Recebe a localidade e a quantidade de termos
            strcpy(localidade, strtok(NULL, ";"));
            qtdTermos = atoi(strtok(NULL, ";"));

            printf("\n%d termos que mais aparecem em %s:\n", qtdTermos, localidade);
            termos = termosPorLocalidade(dados->arvore,  inicializaLDE(), localidade);
            printaLDE(termos, qtdTermos);
            break;
        case 'd':
            //Devolve os n termos mais consultados no arquivo
            qtdTermos = atoi(strtok(NULL, ";"));

            printf("\n%d termos mais consultados no arquivo:\n", qtdTermos);
            termos = termosArquivo(dados->termos);
            printaLDE(termos, qtdTermos);
            break;
        case 'e':
            //Média de termos por consulta em uma dada localidade
            strcpy(localidade,strtok(NULL, ";"));
            tamanho = mediaTamanhoConsultasLocalidade(dados->arvore, localidade);
            printf("\nMedia de termos em %s: %d\n", localidade, tamanho);
            break;
        case 'f':
            // Média de termos por consulta em todo o arquivo
            tamanho = mediaTamanhoConsultasArquivo(dados->arvore);
            printf("\nMedia de termos no arquivo: %d\n", tamanho);
            break;
        }
    }

    return;
}
