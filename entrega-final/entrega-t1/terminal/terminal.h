#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <time.h>

// estruturas

typedef struct timespec crono;

// funções de terminal e temporização

void configura_terminal();
void normaliza_terminal();
char lerTecla();
void crono_inicia(crono *c);
double crono_parcial(crono *c);

#endif
