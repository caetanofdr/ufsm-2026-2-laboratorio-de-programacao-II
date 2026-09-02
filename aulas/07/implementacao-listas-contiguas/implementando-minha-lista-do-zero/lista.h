#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

typedef struct lista *Lista;
typedef int dado_t;

Lista l_cria(void);
void l_destroi(Lista lista);
unsigned int l_tamanho(Lista lista);
bool l_vazia(Lista lista);
void l_insere_inicio(Lista lista, dado_t dado);
void l_insere_fim(Lista lista, dado_t dado);
void l_insere_posicao(Lista lista, dado_t dado, unsigned int posicao);
void l_remove_primeiro(Lista lista);
void l_remove_ultimo(Lista lista);
void l_remove_posicao(Lista lista, unsigned int posicao);
dado_t l_ler_primeiro(Lista lista);
dado_t l_ler_ultimo(Lista lista);
dado_t l_ler_posicao(Lista lista, unsigned int posicao);

#endif