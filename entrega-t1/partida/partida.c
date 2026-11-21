#include <stdio.h>
#include <stdlib.h>

#include "programa.h"
#include "partida.h"
#include "onda.h"
#include "placar.h"
#include "som.h"
#include "terminal.h"

// opções da tela de fim de onda

#define CONTINUAR_PARTIDA 0
#define REINICIAR_PARTIDA 1
#define ENCERRAR_PARTIDA 2

// funções gerais da partida

// zera os dados da partida
static void iniciarNovaPartida(Programa *programa)
{
    programa->partidaAtual.armaAtual = 0;
    programa->partidaAtual.pontuacao = 0;
    programa->partidaAtual.ondasFinalizadas = 0;
    programa->partidaAtual.entrouTop3 = 0;
    programa->partidaAtual.desistiu = 0;
}

// sorteia o tipo da onda; retorna 1 se for diurna
static int sorteouDiurna(Programa *programa)
{
    int probabilidadeDiurna = 100 - (int)programa->partidaAtual.ondasFinalizadas * 20;
    if (probabilidadeDiurna < 20) probabilidadeDiurna = 20;

    int sorteio = rand() % 100;

    if (sorteio < probabilidadeDiurna) {
        return 1;
    } else {
        return 0;
    }
}

// define se a próxima onda é diurna ou noturna
static void sortearProximaOnda(Programa *programa)
{
    if (sorteouDiurna(programa)) {
        programa->partidaAtual.tipoOndaAtual = ONDA_DIURNA;
    } else {
        programa->partidaAtual.tipoOndaAtual = ONDA_NOTURNA;
    }
}

// funções de fim de onda

// desenha o resumo da onda que terminou
static void desenharFimOnda(Programa *programa)
{
    system("clear");
    printf("Você terminou a onda e sobreviveu! Parabéns!\n");
    printf("Tipo de onda jogada: ");

    if (programa->partidaAtual.tipoOndaAtual == ONDA_DIURNA) {
        printf("diurna\n");
    } else if (programa->partidaAtual.tipoOndaAtual == ONDA_NOTURNA) {
        printf("noturna\n");
    }

    printf("Pontuação: %d\n", programa->partidaAtual.pontuacao);
    printf("Tiros restantes: %d\n", programa->partidaAtual.municao);
    printf("Tiros acertados: %d\n", programa->partidaAtual.tirosAcertados);
    printf("Tiros errados: %d\n", programa->partidaAtual.tirosErrados);
    printf("Escudos restantes: %d\n", programa->partidaAtual.quantidadeEscudos);

    printf("[R] - continuar para a próxima onda\n");
    printf("[N] - reiniciar a partida do zero\n");
    printf("[ESC] - encerrar a partida\n");
}

// mostra o resumo da onda e retorna a opção escolhida
static int fimOnda(Programa *programa)
{
    tocaSom(SOM_ESCUDO);
    desenharFimOnda(programa);

    while (1) {
        char tecla = lerTecla();
        if (tecla == 0) continue;

        if (tecla == 27) {
            programa->partidaAtual.ondasFinalizadas++;
            programa->partidaAtual.desistiu = 1;
            return ENCERRAR_PARTIDA;
        } else if (tecla == 'r' || tecla == 'R') {
            programa->partidaAtual.ondasFinalizadas++;
            return CONTINUAR_PARTIDA;
        } else if (tecla == 'n' || tecla == 'N') {
            atualizarPlacar(programa);
            gravarMelhoresPontuacoes(programa);
            return REINICIAR_PARTIDA;
        }
    }
}

// funções de fim de partida

// desenha o resumo da partida e se ela entrou no top 3
static void desenharFimPartida(Programa *programa)
{
    system("clear");
    if (programa->partidaAtual.desistiu) {
        printf("Fim de jogo! Você desistiu da partida.\n");
    } else {
        printf("Fim de jogo! Você foi derrotado.\n");
    }
    printf("Pontuação final: %d\n", programa->partidaAtual.pontuacao);
    printf("Ondas concluídas: %d\n", programa->partidaAtual.ondasFinalizadas);

    if (pontuacaoEntrouTop3(programa)) {
        printf("Novo recorde! Sua pontuação entrou no top 3!\n");
    }

    printf("[R] - reiniciar a partida do zero\n");
    printf("[P] - ver placar das melhores pontuações\n");
    printf("[ESC] - sair do jogo\n");
}

// atualiza o placar, mostra o resumo da partida e escolhe o próximo
// estado do programa conforme a opção
static void fimPartida(Programa *programa)
{
    programa->partidaAtual.entrouTop3 = atualizarPlacar(programa);
    gravarMelhoresPontuacoes(programa);
    tocaSom(SOM_INIMIGO_NAVE);
    desenharFimPartida(programa);

    while (1) {
        char tecla = lerTecla();
        if (tecla == 0) continue;

        if (tecla == 27) {
            programa->estado = ESTADO_FIM_PROGRAMA;
            return;
        } else if (tecla == 'r' || tecla == 'R') {
            programa->estado = ESTADO_PARTIDA;
            return;
        } else if (tecla == 'p' || tecla == 'P') {
            programa->estado = ESTADO_MENU_MELHORES_PONTUACOES;
            return;
        }
    }
}

// função principal da partida

// executa uma partida: sorteia e joga uma onda após a outra até a
// partida terminar e depois mostra o resumo dela
void jogarPartida(Programa *programa)
{
    iniciarNovaPartida(programa);

    while (1) {
        sortearProximaOnda(programa);
        if (!jogarOnda(programa)) break;

        int opcao = fimOnda(programa);
        if (opcao == ENCERRAR_PARTIDA) break;
        if (opcao == REINICIAR_PARTIDA) iniciarNovaPartida(programa);
    }

    fimPartida(programa);
}
