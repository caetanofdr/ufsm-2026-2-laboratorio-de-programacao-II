#ifndef _PLACAR_H_
#define _PLACAR_H_

#include "programa.h"

// funções do placar

void lerArquivoPlacar(Programa *programa);
void gravarMelhoresPontuacoes(Programa *programa);
int atualizarPlacar(Programa *programa);
int pontuacaoEntrouTop3(Programa *programa);

#endif
