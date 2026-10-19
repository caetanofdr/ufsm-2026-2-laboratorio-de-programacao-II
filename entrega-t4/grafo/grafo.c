#include "grafo.h"

#include <stdlib.h>
#include <assert.h>

typedef enum
{
    CONSULTA_NAO_INICIADA,
    CONSULTA_ARESTAS_QUE_PARTEM,
    CONSULTA_ARESTAS_QUE_CHEGAM
} TipoConsulta;

struct consulta
{
    int no;
    int posicao;
    TipoConsulta tipo;
};

struct grafo
{
    int tamanho;
    int tamanho_no;
    int tamanho_aresta;
    void **dados;
    void ***arestas;
    struct consulta consulta;
};

static void g_ok(Grafo g)
{
    assert(g != NULL);
    assert(g->tamanho >= 0);
    assert(g->tamanho_no > 0);
    assert(g->tamanho_aresta > 0);

    if (g->tamanho > 0) {
        assert(g->dados != NULL);
        assert(g->arestas != NULL);
    }

    if (g->consulta.tipo != CONSULTA_NAO_INICIADA) {
        assert(g->consulta.no >= 0);
        assert(g->consulta.no < g->tamanho);
    }
}

static void g_cancela_consulta(Grafo g)
{
    g->consulta.no = -1;
    g->consulta.posicao = 0;
    g->consulta.tipo = CONSULTA_NAO_INICIADA;
}

Grafo g_cria(int tam_nó, int tam_aresta)
{
    Grafo g = (Grafo) malloc(sizeof(struct grafo));
    if (g == NULL) return NULL;

    g->tamanho = 0;
    g->tamanho_no = tam_nó;
    g->tamanho_aresta = tam_aresta;

    g->dados = NULL;
    g->arestas = NULL;

    g_cancela_consulta(g);

    g_ok(g);

    return g;
}

void g_destrói(Grafo self)
{
    g_ok(self);

    for (int i = 0; i < self->tamanho; i++) {
        free(self->dados[i]);
    }
    
    for (int i = 0; i < self->tamanho; i++) {
        for (int j = 0; j < self->tamanho; j++) {
            free(self->arestas[i][j]);
        }

        free(self->arestas[i]);
    }

    free(self->dados);
    free(self->arestas);

    free(self);
}