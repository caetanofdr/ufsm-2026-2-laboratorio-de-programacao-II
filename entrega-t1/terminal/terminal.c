#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "terminal.h"

// funções de terminal e temporização

// coloca o terminal em modo cru para ler cada tecla sem esperar enter
void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    }

    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

// devolve o terminal ao modo normal
void normaliza_terminal()
{
    system("stty sane");
}

// lê um caractere do teclado; retorna 0 se nada foi digitado
static char lechar()
{
    fflush(stdout);
    clearerr(stdin); // coloquei isso aqui pois sem nao roda no meu pc
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}

// inicia o cronômetro com a hora atual
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// retorna os segundos passados desde o início do cronômetro
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);
    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

// lê uma tecla, descartando as sequências de escape das setas
char lerTecla()
{
    static char pendente = 0;
    if (pendente != 0) {
        char guardada = pendente;
        pendente = 0;
        return guardada;
    }

    char c = lechar();
    if (c != 27) return c;

    char proximo = lechar();
    if (proximo == '[' || proximo == 'O') {
        lechar();
        return 0;
    }

    pendente = proximo;
    return c;
}
