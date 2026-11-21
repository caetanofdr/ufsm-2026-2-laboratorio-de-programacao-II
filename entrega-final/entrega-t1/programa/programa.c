#include "programa.h"
#include "placar.h"
#include "terminal.h"

// funções gerais do programa

// configura o terminal, carrega o placar e vai para o menu
void inicializarPrograma(Programa *programa)
{
    configura_terminal();

    programa->placar[0] = 0;
    programa->placar[1] = 0;
    programa->placar[2] = 0;

    lerArquivoPlacar(programa);

    programa->estado = ESTADO_MENU_PRINCIPAL;
}

// grava o placar e devolve o terminal ao modo normal
void finalizarPrograma(Programa *programa)
{
    gravarMelhoresPontuacoes(programa);

    normaliza_terminal();
}
