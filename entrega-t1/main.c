#include <stdlib.h>
#include <time.h>

#include "programa.h"
#include "menus.h"
#include "partida.h"

// main

// executa a tela correspondente ao estado atual do programa
static void executarEstado(Programa *programa)
{
    switch (programa->estado) {
    case ESTADO_MENU_PRINCIPAL:
        menuPrincipal(programa);
        break;
    case ESTADO_MENU_MELHORES_PONTUACOES:
        melhoresPontuacoes(programa);
        break;
    case ESTADO_PARTIDA:
        jogarPartida(programa);
        break;
    default:
        programa->estado = ESTADO_FIM_PROGRAMA;
        break;
    }
}

// laço principal: executa os estados até o fim do programa
int main()
{
    Programa programa;
    inicializarPrograma(&programa);
    srand(time(NULL));

    while (programa.estado != ESTADO_FIM_PROGRAMA) {
        executarEstado(&programa);
    }

    finalizarPrograma(&programa);
    return 0;
}
