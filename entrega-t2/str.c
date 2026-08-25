// includes, constantes e declarações {{{1
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8 // alocação mínima

struct str
{
    byte *dados;
    unsigned int ncaracteres;
    unsigned int nbytes;
    unsigned int nmemoria;
};

// A memória para conter os bytes de uma string deve ser alocada e/ou
//   realocada conforme a necessidade, cuidando para que a quantidade
//   de memória alocada seja sempre:
//   - nula (não alocada) se a string for vazia, ou
//   - não inferior ao necessário para armazenar os bytes da codificação utf8;
//   - não inferior à alocação mínima;
//   - não superior ao triplo do número de bytes necessários
//     (exceto quando for o mínimo);
//   - uma potência de 2.

// funções auxiliares {{{1

// verifica se a string cad está de acordo com a especificação
// aborta o programa se não tiver
static void s_ok(Str_c s)
{
    assert(s != NULL);

    if (s->nbytes == 0)
    {
        assert(s->dados == NULL);
        assert(s->nmemoria == 0);
        assert(s->ncaracteres == 0);
        return;
    }

    assert(s->dados != NULL);
    assert(s->ncaracteres <= s->nbytes);

    assert(s->nmemoria >= s->nbytes);
    assert(s->nmemoria >= MIN_ALLOC);
    assert((s->nmemoria & (s->nmemoria - 1)) == 0);
    if (s->nmemoria > MIN_ALLOC)
    {
        assert(s->nmemoria <= 3 * s->nbytes);
    }

    int ncaracteres_reais = u8_conta_unichar_nos_bytes(s->nbytes, s->dados);
    assert(ncaracteres_reais != -1);
    assert(ncaracteres_reais == s->ncaracteres);
}

static unsigned int s_proxima_potencia_2(int n)
{
    unsigned int p = MIN_ALLOC;

    while (p < n)
    {
        p = p * 2;
    }

    return p;
}

static void s_define_conteudo(Str s, byte const *conteudo, int nbytes)
{
    if (nbytes <= 0) return;

    int ncaracteres = u8_conta_unichar_nos_bytes(nbytes, (byte *)conteudo);
    if (ncaracteres == -1) return;

    s->nmemoria = s_proxima_potencia_2(nbytes);
    s->dados = malloc(s->nmemoria);
    assert(s->dados != NULL);
    memcpy(s->dados, conteudo, nbytes);

    s->nbytes = nbytes;
    s->ncaracteres = ncaracteres;
}

// operações de criação e destruição {{{1

Str s_cria(char const *strC)
{
    Str s = malloc(sizeof(*s));
    assert(s != NULL);

    s->dados = NULL;
    s->nbytes = 0;
    s->ncaracteres = 0;
    s->nmemoria = 0;

    if (strC != NULL) {
        s_define_conteudo(s, (byte const *)strC, strlen(strC));
    }

    s_ok(s);
    return s;
}

void s_destroi(Str s)
{
    s_ok(s);
    free(s->dados);
    free(s);
}

Str s_cria_substring(Str_c s, int pos, int tam)
{
    Str nova = s_cria("");
    s_substring(nova, s, pos, tam);
    return nova;
}

Str s_cria_cópia(Str_c s)
{
    return s_cria_substring(s, 0, -1);
}

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_cria_de_arquivo(char *nome)
{
    Str s = s_cria("");

    FILE *arquivo = fopen(nome, "rb");
    if (arquivo == NULL) return s;

    fseek(arquivo, 0, SEEK_END);
    long tamamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    if (tamamanho > 0) {
        byte *tmp = malloc(tamamanho);
        assert(tmp != NULL);
        size_t lidos = fread(tmp, 1, tamamanho, arquivo);
        
        if (lidos == (size_t)tamamanho) {
            s_define_conteudo(s, tmp, tamamanho);
        }

        free(tmp);
    }

    fclose(arquivo);
    s_ok(s);
    return s;
}

// operações de acesso {{{1

int s_tam(Str_c s)
{
    s_ok(s);
    return s->ncaracteres;
}

char *s_strc(Str_c s)
{
    s_ok(s);

    char *c = malloc(s->nbytes + 1);
    assert(c != NULL);

    if (s->nbytes > 0) {
        memcpy(c, s->dados, s->nbytes);
    }

    c[s->nbytes] = '\0';

    return c;
}

unichar s_ch(Str_c s, int posicao)
{
    s_ok(s);

    if (posicao < 0) {
        posicao += s->ncaracteres;
    }

    if (posicao < 0 || posicao >= (int) s->ncaracteres) {
        return UNI_INV;
    }
    
    byte *ptr = u8_avanca_unichar(s->dados, posicao);

    unichar uni;
    int nbytes_restantes = s->nbytes - (ptr - s->dados);
    int nb = u8_unichar_nos_bytes(nbytes_restantes, ptr, &uni);
    if (nb == -1) return UNI_INV;

    return uni;
}

// operações de busca e comparação {{{1

bool s_igual(Str_c s, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
    return false;
}

int s_busca_c(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
    return -1;
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
    return -1;
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
    return -1;
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
    return -1;
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
    s_ok(s);
    s_ok(buscada);
    //...
    return -1;
}

// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    //...
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
    s_ok(s);
    s_ok(sb);
    //...
}

void s_copia(Str s, Str_c sb)
{
    s_substring(s, sb, 0, -1);
}

void s_insere(Str s, int pos, Str_c sb)
{
    s_substitui(s, pos, 0, sb);
}

void s_insere_c(Str s, int pos, unichar c)
{
    s_ok(s);
    //...
}

void s_anexa(Str s, Str_c sb)
{
    s_substitui(s, -1, 0, sb);
}

void s_anexa_c(Str s, unichar c)
{
    s_insere_c(s, -1, c);
}

void s_remove(Str s, int pos, int tam)
{
    s_substitui(s, pos, tam, NULL);
}

void s_apara(Str s, Str_c sobras)
{
    s_ok(s);
    s_ok(sobras);
    //...
}

// operações de E/S {{{1

void s_imprime(Str_c s)
{
    s_ok(s);
    //...
}

void s_grava_arquivo(Str_c s, char *nome)
{
    s_ok(s);
    //...
}

// vim: foldmethod=marker shiftwidth=2
