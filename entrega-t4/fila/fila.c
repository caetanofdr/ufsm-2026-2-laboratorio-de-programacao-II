#include "fila.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct no No;

typedef enum
{
    PERCURSO_NAO_INICIADO,
    PERCURSO_PARA_FRENTE,
    PERCURSO_PARA_TRAS
} SentidoPercurso;

struct no
{
    void *dado;
    No *anterior;
    No *proximo;
};

struct percurso
{
    No *posicao;
    SentidoPercurso sentido;
};

struct fila
{
    int tamanho_dado;
    unsigned int tamanho;
    No *sentinela;
    struct percurso percurso;
};

static void f_no_ok(No *no)
{
    assert(no != NULL);
    assert(no->proximo != NULL);
    assert(no->anterior != NULL);
    assert(no->proximo->anterior == no);
    assert(no->anterior->proximo == no);
}

static void f_ok(Fila f)
{
    assert(f != NULL);
    assert(f->tamanho_dado > 0);
    f_no_ok(f->sentinela);

    if (f->tamanho == 0) {
        assert(f->sentinela->proximo == f->sentinela);
        assert(f->sentinela->anterior == f->sentinela);
        assert(f->sentinela->dado == NULL);
        return;
    }

    unsigned int tamanho;
    No *p = f->sentinela->proximo;

    for (tamanho = 0; p != f->sentinela; tamanho++) {
        f_no_ok(p);
        p = p->proximo;
    }

    assert(tamanho == f->tamanho);
}

static void f_no_destroi(No *no)
{
    free(no->dado);
    free(no);
}

static No *f_no_cria(int tamanho_dado)
{
    No *no = malloc(sizeof(No));
    assert(no != NULL);

    if (tamanho_dado > 0) {
        no->dado = malloc(tamanho_dado);
        assert(no->dado != NULL);
    } else {
        no->dado = NULL;
    }

    return no;
}

Fila f_cria(int tam_do_dado)
{
    Fila f = malloc(sizeof(struct fila));
    assert(f != NULL);

    f->sentinela = f_no_cria(0);
    f->sentinela->anterior = f->sentinela;
    f->sentinela->proximo = f->sentinela;

    f_no_ok(f->sentinela);

    f->percurso.posicao = NULL;
    f->percurso.sentido = PERCURSO_NAO_INICIADO;

    f->tamanho = 0;
    f->tamanho_dado = tam_do_dado;

    f_ok(f);

    return f;
}

void f_destrói(Fila self)
{
    f_ok(self);

    No *p = self->sentinela->proximo;

    for (unsigned int i = 0; i < self->tamanho; i++) {
        No *temporario = p;
        p = p->proximo;
        f_no_destroi(temporario);
    }

    f_no_destroi(self->sentinela);
    free(self);
}

bool f_tá_vazia(Fila self)
{
    f_ok(self);
    return self->tamanho == 0;
}

void f_remove(Fila self, void *pdado)
{
    f_ok(self);
    assert(self->tamanho > 0);

    No *removido = self->sentinela->proximo;

    if (pdado != NULL) {
        memcpy(pdado, removido->dado, self->tamanho_dado);
    }

    removido->proximo->anterior = removido->anterior;
    removido->anterior->proximo = removido->proximo;
    self->tamanho--;

    f_no_destroi(removido);
}

void f_insere(Fila self, void *pdado)
{
    f_ok(self);

    No *sentinela = self->sentinela;
    No *inserido = f_no_cria(self->tamanho_dado);
    memcpy(inserido->dado, pdado, self->tamanho_dado);

    inserido->proximo = sentinela;
    inserido->anterior = sentinela->anterior;

    sentinela->anterior->proximo = inserido;
    sentinela->anterior = inserido;

    self->tamanho++;
}

void f_inicia_percurso(Fila self, int pos_inicial)
{
    f_ok(self);
    int tamanho = (int) self->tamanho;

    if (pos_inicial < 0) {
        assert(pos_inicial >= -tamanho);

        No *p = self->sentinela->anterior;

        for (int i = -1; i > pos_inicial; i--) {
            p = p->anterior;
        }

        self->percurso.posicao = p;
        self->percurso.sentido = PERCURSO_PARA_TRAS;
        return;
    }

    assert(pos_inicial < tamanho);

    No *p = self->sentinela->proximo;

    for (int i = 0; i < pos_inicial; i++) {
        p = p->proximo;
    }

    self->percurso.posicao = p;
    self->percurso.sentido = PERCURSO_PARA_FRENTE;
}

bool f_próximo(Fila self, void *pdado)
{
    assert(self != NULL);

    if (self->percurso.sentido == PERCURSO_NAO_INICIADO) return false;
    if (self->percurso.posicao == self->sentinela) return false;

    No *atual = self->percurso.posicao;

    if (pdado != NULL) {
        memcpy(pdado, atual->dado, self->tamanho_dado);
    }

    if (self->percurso.sentido == PERCURSO_PARA_TRAS) {
        self->percurso.posicao = atual->anterior;
    } else {
        self->percurso.posicao = atual->proximo;
    }

    return true;
}