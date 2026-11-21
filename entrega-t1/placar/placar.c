#include <stdio.h>

#include "programa.h"
#include "placar.h"

// arquivo

#define NOME_ARQUIVO_PLACAR "placar.txt"

// funções do placar

// ordena o placar em ordem decrescente
static void ordenarPlacar(Programa *programa)
{
    for (int passada = 0; passada < 2; passada++) {
        for (int i = 0; i < 2; i++) {
            if (programa->placar[i] < programa->placar[i + 1]) {
                unsigned int tmp = programa->placar[i];
                programa->placar[i] = programa->placar[i + 1];
                programa->placar[i + 1] = tmp;
            }
        }
    }
}

// lê as três melhores pontuações do arquivo, se ele existir,
// e as deixa em ordem decrescente
void lerArquivoPlacar(Programa *programa)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_PLACAR, "r");

    if (arquivo == NULL) return;

    for (int i = 0; i < 3; i++) {
        if (fscanf(arquivo, "%u", &programa->placar[i]) != 1) break;
    }

    fclose(arquivo);

    ordenarPlacar(programa);
}

// grava as três melhores pontuações no arquivo
void gravarMelhoresPontuacoes(Programa *programa)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_PLACAR, "w");
    if (arquivo == NULL) return;

    for (int i = 0; i < 3; i++) {
        fprintf(arquivo, "%u\n", programa->placar[i]);
    }

    fclose(arquivo);
}

// insere a pontuação da partida no placar se ela for top 3;
// retorna 1 se entrou e 0 se não entrou
int atualizarPlacar(Programa *programa)
{
    unsigned int pontuacao = programa->partidaAtual.pontuacao;

    if (pontuacao <= programa->placar[2]) return 0;

    int i = 2;
    while (i > 0 && programa->placar[i - 1] < pontuacao) {
        programa->placar[i] = programa->placar[i - 1];
        i--;
    }

    programa->placar[i] = pontuacao;
    return 1;
}

// retorna se a pontuação da partida atual entrou no top 3
int pontuacaoEntrouTop3(Programa *programa)
{
    return programa->partidaAtual.entrouTop3;
}
