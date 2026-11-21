#include <stdio.h>
#include <stdlib.h>

#include "programa.h"
#include "menus.h"
#include "terminal.h"

// funções do menu principal

// desenha as opções do menu principal
static void desenharMenuPrincipal()
{
    system("clear");
    printf("Bem-vindo ao Invasores do Espaço!\n");
    printf("[J] - Jogar nova partida\n");
    printf("[P] - Placar das melhores pontuações\n");
    printf("[ESC] - Sair do Invasores do Espaço\n");
}

// mostra o menu principal e trata a opção escolhida
void menuPrincipal(Programa *programa)
{
    desenharMenuPrincipal();

    while (1) {
        char tecla = lerTecla();
        if (tecla == 0) continue;

        if (tecla == 27) {
            programa->estado = ESTADO_FIM_PROGRAMA;
            return;
        } else if (tecla == 'j' || tecla == 'J') {
            programa->estado = ESTADO_PARTIDA;
            return;
        } else if (tecla == 'p' || tecla == 'P') {
            programa->estado = ESTADO_MENU_MELHORES_PONTUACOES;
            return;
        }
    }
}

// funções do placar de melhores pontuações

// desenha a tela com as três melhores pontuações
static void desenharMelhoresPontuacoes(Programa *programa)
{
    system("clear");
    printf("Placar de melhores pontuações\n");

    for (int i = 0; i < 3; i++) {
        printf("%dº - %d pontos\n", i + 1, programa->placar[i]);
    }
    
    printf("[J] - jogar nova partida\n");
    printf("[ESC] - sair do jogo\n");
}

// mostra o placar e trata a opção escolhida
void melhoresPontuacoes(Programa *programa)
{
    desenharMelhoresPontuacoes(programa);

    while (1) {
        char tecla = lerTecla();
        if (tecla == 0) continue;

        if (tecla == 27) {
            programa->estado = ESTADO_FIM_PROGRAMA;
            return;
        } else if (tecla == 'j' || tecla == 'J') {
            programa->estado = ESTADO_PARTIDA;
            return;
        }
    }
}
