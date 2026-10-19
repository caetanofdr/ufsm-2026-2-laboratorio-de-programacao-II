#include "grafo.h"

#include <stdlib.h>
#include <string.h>
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

int g_insere_nó(Grafo self, void *p_dado)
{
    g_ok(self);

    int n = self->tamanho;

    void **tmp_dados = realloc(self->dados, (n + 1) * sizeof(void *));
    if (tmp_dados == NULL) return -1;
    self->dados = tmp_dados;

    self->dados[n] = malloc(self->tamanho_no);
    if (self->dados[n] == NULL) return -1;

    for (int i = 0; i < n; i++) {
        void **tmp_linha = realloc(self->arestas[i], (n + 1) * sizeof(void *));
        if (tmp_linha == NULL) return -1;
        self->arestas[i] = tmp_linha;
        self->arestas[i][n] = NULL;
    }

    void ***tmp_arestas = realloc(self->arestas, (n + 1) * sizeof(void **));
    if (tmp_arestas == NULL) return -1;
    self->arestas = tmp_arestas;

    self->arestas[n] = malloc((n + 1) * sizeof(void *));
    if (self->arestas[n] == NULL) return -1;

    for (int j = 0; j <= n; j++) {
        self->arestas[n][j] = NULL;
    }

    memcpy(self->dados[n], p_dado, self->tamanho_no);

    self->tamanho++;
    g_cancela_consulta(self);

    return self->tamanho - 1;
}

void g_remove_nó(Grafo self, int nó)
{
    g_ok(self);
    assert(nó >= 0 && nó < self->tamanho);

    int n = self->tamanho;
    void *removido = self->dados[nó];

    for (int i = nó; i < n - 1; i++) {
        self->dados[i] = self->dados[i + 1];
    }

    for (int i = 0; i < n; i++) {
        free(self->arestas[nó][i]);
        if (i != nó) free(self->arestas[i][nó]);
    }

    free(self->arestas[nó]);

    for (int i = nó; i < n - 1; i++) {
        self->arestas[i] = self->arestas[i + 1];
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = nó; j < n - 1; j++) {
            self->arestas[i][j] = self->arestas[i][j + 1];
        }
    }

    free(removido);

    self->tamanho--;
    g_cancela_consulta(self);
}

void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado)
{
    g_ok(self);
    assert(nó >= 0 && nó < self->tamanho);

    memcpy(self->dados[nó], p_dado, self->tamanho_no);
}

void *g_valor_do_nó(Grafo self, int nó)
{
    g_ok(self);
    assert(nó >= 0 && nó < self->tamanho);

    return self->dados[nó];
}

int g_num_nós(Grafo self)
{
    g_ok(self);
    
    return self->tamanho;
}