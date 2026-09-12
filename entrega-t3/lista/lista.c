#include "lista.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct no No;

struct no
{
    dado_t dado;
    No *anterior;
    No *proximo;
};

struct lista
{
    int tamanho;
    No *sentinela;
};

static void l_no_ok(No *no);
static void l_ok(Lista l);
static No *l_no_anterior_a_posicao(Lista l, int index);
static No *l_no_na_posicao(Lista l, int index);
static No *l_cria_no(void);
static void l_destroi_no(No *no);

static void l_no_ok(No *no)
{
    assert(no != NULL);
    assert(no->proximo != NULL);
    assert(no->anterior != NULL);
    assert(no->proximo->anterior == no);
    assert(no->anterior->proximo == no);
}

static void l_ok(Lista l)
{
    assert(l != NULL);
    l_no_ok(l->sentinela);

    if (l->tamanho == 0) {
        assert(l->sentinela->proximo == l->sentinela);
        assert(l->sentinela->anterior == l->sentinela);
        assert(l->sentinela->dado == NULL);
        return;
    }

    int tamanho;
    No *p = l->sentinela->proximo;

    for (tamanho = 0; p != l->sentinela; tamanho++) {
        l_no_ok(p);
        p = p->proximo;
    }

    assert(tamanho == l->tamanho);
}

static No *l_no_anterior_a_posicao(Lista l, int index)
{
    No *p = l->sentinela;
    for (int i = 0; i < index; i++) {
        p = p->proximo;
    }
    return p;
}

static No *l_no_na_posicao(Lista l, int index)
{
    return l_no_anterior_a_posicao(l, index)->proximo;
}

static No *l_cria_no(void)
{
    No *no = malloc(sizeof(struct no));
    assert(no != NULL);
    return no;
}

static void l_destroi_no(No *no)
{
    free(no);
}

Lista l_cria(void)
{
    Lista l = malloc(sizeof(struct lista));
    assert(l != NULL);

    l->sentinela = l_cria_no();

    l->sentinela->proximo = l->sentinela;
    l->sentinela->anterior = l->sentinela;
    l->sentinela->dado = NULL;
    l->tamanho = 0;

    l_ok(l);

    return l;
}

Lista l_cria_separando(Str s, Str sep)
{
    Lista l = l_cria();

    int n = s_tam(s);
    int i = s_busca_nc(s, 0, sep); // primeira posição que não é separador

    while (i != -1 && i < n) {
        int fim = s_busca_c(s, i, sep); // próxima posição que é separador
        if (fim == -1) fim = n;

        Str token = s_cria_substring(s, i, fim - i);
        l_insere_fim(l, token);

        i = s_busca_nc(s, fim, sep);
    }

    return l;
}

void l_destroi(Lista l)
{
    l_ok(l);

    No *p = l->sentinela->proximo;

    for (int i = 0; i < l->tamanho; i++) {
        No *tmp = p;
        p = p->proximo;
        l_destroi_no(tmp);
    }

    l_destroi_no(l->sentinela);
    free(l);
}

int l_tam(Lista l)
{
    l_ok(l);
    return l->tamanho;
}

bool l_cheia(Lista l)
{
    (void) l; // parâmetro exigido pela interface, mas não usado aqui
    return false; // não tem como estar cheia
}

bool l_vazia(Lista l)
{
    l_ok(l);

    if (l->tamanho != 0) {
        return false;
    }

    return true;
}

void l_imprime(Lista l)
{
    l_ok(l);

    No *p = l->sentinela->proximo;

    while (true) {
        s_imprime(p->dado);
        p = p->proximo;

        if (p == l->sentinela) {
            break;
        }

        printf(" ");
    }
}

void l_insere_inicio(Lista l, dado_t d)
{
    l_insere_pos(l, d, 0);
}

void l_insere_fim(Lista l, dado_t d)
{
    l_insere_pos(l, d, l->tamanho);
}

void l_insere_pos(Lista l, dado_t d, int p)
{
    l_ok(l);

    if (l->tamanho < p) return;

    No *inserido = l_cria_no();

    No *anterior = l_no_anterior_a_posicao(l, p);
    No *proximo = anterior->proximo;

    inserido->dado = d;
    inserido->anterior = anterior;
    inserido->proximo = proximo;

    anterior->proximo = inserido;
    proximo->anterior = inserido;

    l->tamanho++;

    l_ok(l);
}

dado_t l_dado_inicio(Lista l)
{
    return l_dado_pos(l, 0);
}

dado_t l_dado_fim(Lista l)
{
    return l_dado_pos(l, l->tamanho - 1);
}

dado_t l_dado_pos(Lista l, int pos)
{
    l_ok(l);
    No *p = l_no_na_posicao(l, pos);

    return p->dado;
}

dado_t l_remove_inicio(Lista l)
{
    return l_remove_pos(l, 0);
}

dado_t l_remove_fim(Lista l)
{
    return l_remove_pos(l, l->tamanho - 1);
}

dado_t l_remove_pos(Lista l, int pos)
{
    l_ok(l);

    if (l->tamanho <= pos) return NULL;

    No *removido = l_no_na_posicao(l, pos);
    No *anterior = removido->anterior;
    No *proximo = removido->proximo;

    anterior->proximo = proximo;
    proximo->anterior = anterior;

    dado_t d = removido->dado;
    free(removido);
    l->tamanho--;

    l_ok(l);

    return d;
}

dado_t l_primeiro(Lista l)
{
    return l_dado_inicio(l);
}

void l_insere(Lista l, dado_t d)
{
    l_insere_fim(l, d);
}

dado_t l_remove(Lista l)
{
    return l_remove_inicio(l);
}

dado_t l_topo(Lista l)
{
    return l_dado_fim(l);
}

void l_empilha(Lista l, dado_t d)
{
    l_insere_fim(l, d);
}

dado_t l_desempilha(Lista l)
{
    return l_remove_fim(l);
}