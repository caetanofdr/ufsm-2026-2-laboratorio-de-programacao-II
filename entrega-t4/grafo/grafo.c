#include "grafo.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef enum
{
    CONSULTA_NAO_INICIADA,
    CONSULTA_ARESTAS_QUE_PARTEM,
    CONSULTA_ARESTAS_QUE_CHEGAM
} TipoConsulta;

typedef enum
{
    ESTADO_VISITADO,
    ESTADO_EM_VISITA,
    ESTADO_NAO_VISITADO
} EstadoVisita;

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

void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino, void *p_dado)
{
    g_ok(self);
    assert(nó_origem >= 0 && nó_origem < self->tamanho);
    assert(nó_destino >= 0 && nó_destino < self->tamanho);

    void **posicao = &self->arestas[nó_origem][nó_destino];

    if (p_dado == NULL) {
        free(*posicao);
        *posicao = NULL;
        return;
    }

    if (*posicao == NULL) {
        *posicao = malloc(self->tamanho_aresta);
        assert(*posicao != NULL);
    }

    memcpy(*posicao, p_dado, self->tamanho_aresta);
}

void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino)
{
    g_ok(self);
    assert(nó_origem >= 0 && nó_origem < self->tamanho);
    assert(nó_destino >= 0 && nó_destino < self->tamanho);

    return self->arestas[nó_origem][nó_destino];
}

void g_arestas_que_partem(Grafo self, int nó_origem)
{
    g_ok(self);
    assert(nó_origem >= 0 && nó_origem < self->tamanho);

    self->consulta.no = nó_origem;
    self->consulta.posicao = 0;
    self->consulta.tipo = CONSULTA_ARESTAS_QUE_PARTEM;
}

void g_arestas_que_chegam(Grafo self, int nó_destino)
{
    g_ok(self);
    assert(nó_destino >= 0 && nó_destino < self->tamanho);

    self->consulta.no = nó_destino;
    self->consulta.posicao = 0;
    self->consulta.tipo = CONSULTA_ARESTAS_QUE_CHEGAM;
}

void *g_próxima_aresta(Grafo self, int *p_nó_vizinho)
{
    g_ok(self);

    if (self->consulta.tipo == CONSULTA_NAO_INICIADA) return NULL;

    while (self->consulta.posicao < self->tamanho) {
        int vizinho = self->consulta.posicao;
        self->consulta.posicao++;

        void *aresta;
        if (self->consulta.tipo == CONSULTA_ARESTAS_QUE_PARTEM) {
            aresta = self->arestas[self->consulta.no][vizinho];
        } else {
            aresta = self->arestas[vizinho][self->consulta.no];
        }

        if (aresta != NULL) {
            if (p_nó_vizinho != NULL) *p_nó_vizinho = vizinho;
            return aresta;
        }
    }

    return NULL;
}

static bool g_acha_ciclo(Grafo self, EstadoVisita visitas[], int nó)
{
    if (visitas[nó] == ESTADO_EM_VISITA) return true;
    if (visitas[nó] == ESTADO_VISITADO) return false;

    visitas[nó] = ESTADO_EM_VISITA;

    for (int i = 0; i < self->tamanho; i++) {
        if (self->arestas[nó][i] == NULL) continue;

        if (g_acha_ciclo(self, visitas, i)) return true;
    }

    visitas[nó] = ESTADO_VISITADO;
    return false;
}

bool g_tem_ciclo(Grafo self)
{
    g_ok(self);
    if (self->tamanho == 0) return false;

    EstadoVisita visitas[self->tamanho];
    for (int i = 0; i < self->tamanho; i++) visitas[i] = ESTADO_NAO_VISITADO;

    for (int i = 0; i < self->tamanho; i++) {
        if (visitas[i] == ESTADO_VISITADO) continue;
        if (g_acha_ciclo(self, visitas, i)) return true;
    }

    return false;
}

static int *g_calcula_grau_entrada(Grafo self)
{
    g_ok(self);
    if (self->tamanho < 1) return NULL;

    int *vet = calloc(self->tamanho, sizeof(int));
    if (vet == NULL) return NULL;

    for (int i = 0; i < self->tamanho; i++) {
        for (int j = 0; j < self->tamanho; j++) {
            if (self->arestas[j][i] == NULL) continue;
            vet[i]++;
        }
    }

    return vet;
}

Fila g_ordem_topológica(Grafo self)
{
    g_ok(self);

    Fila ordem = f_cria(sizeof(int));
    if (g_tem_ciclo(self)) return ordem;

    int *grau = g_calcula_grau_entrada(self);
    if (grau == NULL) return ordem;

    Fila prontos = f_cria(sizeof(int));

    for (int i = 0; i < self->tamanho; i++) {
        if (grau[i] == 0) f_insere(prontos, &i);
    }

    while (!f_tá_vazia(prontos)) {
        int nó;
        f_remove(prontos, &nó);
        f_insere(ordem, &nó);

        for (int i = 0; i < self->tamanho; i++) {
            if (self->arestas[nó][i] == NULL) continue;

            grau[i]--;
            if (grau[i] == 0) f_insere(prontos, &i);
        }
    }

    f_destrói(prontos);
    free(grau);

    return ordem;
}