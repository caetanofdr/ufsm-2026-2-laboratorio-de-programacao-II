#include "lista.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct no No;

struct no
{
    dado_t dado;
    No *proximo;
};

struct lista
{
    unsigned int tamanho;
    No *primeiro;
    No *ultimo;
};

void static l_ok(Lista lista)
{
    assert(lista != NULL);

    if (lista->tamanho == 0) {
        assert(lista->primeiro == NULL);
        assert(lista->ultimo == NULL);
        return;
    }
    
    assert(lista->primeiro != NULL);
    assert(lista->ultimo != NULL);

    unsigned int tamanho;
    No *p = lista->primeiro;

    for (tamanho = 0; p != NULL ; tamanho++) p = p->proximo;
    
    assert(tamanho == lista->tamanho);
}

No static *l_cria_no(void)
{
    No *no = malloc(sizeof(struct no));
    assert(no != NULL);
    return no;
}

void static l_destroi_no(No *no)
{
    free(no);
}

No static *l_no_na_posicao(Lista lista, unsigned int index)
{
    No *p = lista->primeiro;
    for (unsigned int i = 0; i < index; i++) {
        p = p->proximo;
    }
    
    return p;
}

Lista l_cria(void)
{
    Lista lista = malloc(sizeof(struct lista));
    assert(lista != NULL);

    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->tamanho = 0;

    return lista;
}

void l_destroi(Lista lista)
{
    l_ok(lista);

    No *p = lista->primeiro;

    for (unsigned int i = 0; i < lista->tamanho; i++) {
        No *tmp = p;
        p = p->proximo;
        l_destroi_no(tmp);
    }
    
    free(lista);
}

unsigned int l_tamanho(Lista lista)
{
    l_ok(lista);
    return lista->tamanho;
}

bool l_vazia(Lista lista)
{
    l_ok(lista);

    if (lista->primeiro != NULL) {
        return false;
    }
    
    return true;
}

void l_insere_inicio(Lista lista, dado_t dado)
{
    l_ok(lista);
    No *no = l_cria_no();
    no->dado = dado;
    no->proximo = lista->primeiro;
    lista->primeiro = no;
    if (lista->tamanho == 0) lista->ultimo = no;
    lista->tamanho++;
}

    void l_insere_fim(Lista lista, dado_t dado)
{
    l_ok(lista);
    No *no = l_cria_no();
    no->dado = dado;
    no->proximo = NULL;
    lista->ultimo->proximo = no;
    lista->ultimo = no;
    if (lista->tamanho == 0) lista->primeiro = no;
    lista->tamanho++;
}

void l_insere_posicao(Lista lista, dado_t dado, unsigned int posicao)
{
    l_ok(lista);

    if (lista->tamanho < posicao) return;
    if (posicao == 0) return(l_insere_inicio(lista, dado));
    if (posicao == lista->tamanho) return(l_insere_fim(lista, dado));

    No *anterior = l_no_na_posicao(lista, posicao - 1);
    No *proximo = l_no_na_posicao(lista, posicao);
    No *no = l_cria_no();

    no->dado = dado;
    no->proximo = proximo;
    anterior->proximo = no;
    
    lista->tamanho++;
}

void l_remove_primeiro(Lista lista)
{
    l_ok(lista);
    if (lista->tamanho == 0) return;

    No *segundo = lista->primeiro->proximo;

    l_destroi_no(lista->primeiro);
    lista->primeiro = segundo;
    if (lista->tamanho == 1 || lista->tamanho == 2) lista->ultimo = segundo;
    lista->tamanho--;
}

void l_remove_ultimo(Lista lista)
{
    l_ok(lista);
    if (lista->tamanho == 0) return;
    if (lista->tamanho == 1) return(l_remove_primeiro(lista));

    No *anterior = l_no_na_posicao(lista, lista->tamanho - 2);
    anterior->proximo = NULL;
    l_destroi_no(lista->ultimo);
    lista->ultimo = anterior;
    lista->tamanho--;
}

void l_remove_posicao(Lista lista, unsigned int posicao)
{
    l_ok(lista);
    if (posicao >= lista->tamanho) return;
    if (posicao == 0) return(l_remove_primeiro(lista));
    if (posicao == lista->tamanho - 1) return(l_remove_ultimo(lista));

    No *anterior = l_no_na_posicao(lista, posicao - 1);
    No *no_remover = anterior->proximo;
    No *proximo = no_remover->proximo;

    anterior->proximo = proximo;
    l_destroi_no(no_remover);
    lista->tamanho--;
}

dado_t l_ler_primeiro(Lista lista)
{
    l_ok(lista);
    return lista->primeiro->dado;
}

dado_t l_ler_ultimo(Lista lista)
{
    l_ok(lista);
    return lista->ultimo->dado;
}

dado_t l_ler_posicao(Lista lista, unsigned int posicao)
{
    l_ok(lista);
    assert (posicao < lista->tamanho);
    No *no = l_no_na_posicao(lista, posicao);
    return no->dado;
}