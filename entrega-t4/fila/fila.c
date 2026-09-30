#include "fila.h"

#include <stdlib.h>
#include <assert.h>

typedef struct no {
    void *dado;
    No *anterior;
    No *proximo;
} No;

struct fila {
    int tamanho_dado;
    unsigned int tamanho;
    No *sentinela;
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

    int tamanho;
    No *p = f->sentinela->proximo;

    for (tamanho = 0; p != f->sentinela; tamanho++) {
        f_no_ok(p);
        p = p->proximo;
    }

    assert(tamanho == f->tamanho);
}

static void f_no_destroi(No *n)
{
    free(n);
}

No *f_no_cria(void)
{
    No *n = (No*) malloc(sizeof(No));
    assert(n != NULL);
    return n;
}

Fila f_cria(int tam_do_dado)
{
    Fila f = (Fila*) malloc(sizeof(struct fila));
    assert(f != NULL);
    
    f->sentinela = f_no_cria();
    f->sentinela->anterior = f->sentinela;
    f->sentinela->proximo = f->sentinela;
    f->sentinela->dado = NULL;

    f_no_ok(f->sentinela);

    f->tamanho = 0;
    f->tamanho_dado = tam_do_dado;

    f_ok(f);

    return f;
}

void f_destrói(Fila self)
{
    f_ok(self);

    No *p = self->sentinela->proximo;

    for (int i = 0; i < self->tamanho; i++) {
        No *tmp = p;
        p = p->proximo;
        f_no_destroi(tmp);
    }

    f_no_destroi(self->sentinela);
    free(self);
}