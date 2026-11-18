// includes, constantes e declarações {{{1
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8 // alocação mínima
#define NUMERO_MAXIMO 500

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

    if (s->nbytes == 0) {
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

    if (s->nmemoria > MIN_ALLOC) {
        assert(s->nmemoria <= 3 * s->nbytes);
    }
}

static unsigned int s_proxima_potencia_2(unsigned int n)
{
    unsigned int p = MIN_ALLOC;

    while (p < n) {
        p = p * 2;
    }

    return p;
}

static void s_define_conteudo(Str s, byte const *conteudo, int nbytes)
{
    if (nbytes <= 0) {
        return;
    }

    int ncaracteres = u8_conta_unichar_nos_bytes(nbytes, (byte *)conteudo);
    if (ncaracteres == -1) {
        return;
    }

    s->nmemoria = s_proxima_potencia_2(nbytes);
    s->dados = malloc(s->nmemoria);
    assert(s->dados != NULL);
    memcpy(s->dados, conteudo, nbytes);

    s->nbytes = nbytes;
    s->ncaracteres = ncaracteres;
}

static void s_esvazia(Str s)
{
    free(s->dados);
    s->dados = NULL;
    s->nmemoria = 0;
    s->nbytes = 0;
    s->ncaracteres = 0;
}

static byte *s_ptr_posicao(Str_c s, int pos)
{
    if (pos == (int)s->ncaracteres) {
        return s->dados + s->nbytes;
    }

    return u8_avanca_unichar(s->dados, pos);
}

static int s_posicao_corrigida(int posicao, int n)
{
    if (posicao < 0) {
        posicao = n + posicao + 1;
    }

    if (posicao < 0) {
        posicao = 0;
    }

    if (posicao > n) {
        posicao = n;
    }

    return posicao;
}

static unichar s_le_unichar(byte *ptr, byte *fim)
{
    unichar uni;

    if (u8_unichar_nos_bytes(fim - ptr, ptr, &uni) == -1) {
        return UNI_INV;
    }

    return uni;
}

static byte *s_proximo_unichar(byte *ptr)
{
    return ptr + u8_nbytes_no_unichar_que_comeca_com(*ptr);
}

static byte *s_unichar_anterior(byte *inicio, byte *ptr)
{
    do {
        ptr--;
    } while (ptr > inicio &&
        u8_nbytes_no_unichar_que_comeca_com(*ptr) == -1);

    return ptr;
}

static bool s_caractere_pertence(unichar c, Str_c sb)
{
    byte *ptr = sb->dados;
    byte *fim = sb->dados + sb->nbytes;

    while (ptr < fim) {
        if (s_le_unichar(ptr, fim) == c) {
            return true;
        }
        ptr = s_proximo_unichar(ptr);
    }

    return false;
}

static int s_busca_frente(Str_c s, int pos, Str_c sb, bool pertence)
{
    s_ok(s);
    s_ok(sb);

    int n = s->ncaracteres;
    pos = s_posicao_corrigida(pos, n);

    byte *fim = s->dados + s->nbytes;
    byte *ptr = u8_avanca_unichar(s->dados, pos);

    for (int i = pos; i < n; i++) {
        unichar c = s_le_unichar(ptr, fim);
        if (s_caractere_pertence(c, sb) == pertence) {
            return i;
        }
        ptr = s_proximo_unichar(ptr);
    }

    return -1;
}

static int s_busca_tras(Str_c s, int pos, Str_c sb, bool pertence)
{
    s_ok(s);
    s_ok(sb);

    int n = s->ncaracteres;
    pos = s_posicao_corrigida(pos, n);

    byte *fim = s->dados + s->nbytes;
    byte *ptr = u8_avanca_unichar(s->dados, pos);

    for (int i = pos - 1; i >= 0; i--) {
        ptr = s_unichar_anterior(s->dados, ptr);
        unichar c = s_le_unichar(ptr, fim);
        if (s_caractere_pertence(c, sb) == pertence) {
            return i;
        }
    }

    return -1;
}

static void s_garante_memoria(Str s, unsigned int nbytes_necessarios)
{
    if (nbytes_necessarios == 0) {
        free(s->dados);
        s->dados = NULL;
        s->nmemoria = 0;
        return;
    }

    bool precisa_realocar = (s->nmemoria < nbytes_necessarios) ||
        (s->nmemoria > MIN_ALLOC && s->nmemoria > 3 * nbytes_necessarios);

    if (precisa_realocar) {
        unsigned int nova_memoria = s_proxima_potencia_2(nbytes_necessarios);
        byte *novo_dados = realloc(s->dados, nova_memoria);
        assert(novo_dados != NULL);
        s->dados = novo_dados;
        s->nmemoria = nova_memoria;
    }
}

static void s_intervalo_corrigido(int pos, int tam, int n, int *pos_final, int *tam_final)
{
    long long inicio = (pos < 0) ? n + pos + 1 : pos;
    long long fim = (tam < 0) ? n : inicio + tam;

    if (inicio < 0) inicio = 0;
    if (inicio > n) inicio = n;

    if (fim < 0) fim = 0;
    if (fim > n) fim = n;

    if (fim < inicio) fim = inicio;

    *pos_final = inicio;
    *tam_final = fim - inicio;
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

Str s_cria_número(double num)
{
    char tmp[NUMERO_MAXIMO];

    snprintf(tmp, sizeof(tmp), "%.15lf", num);
    
    char *pos = tmp + strlen(tmp) - 1;

    while (*pos == '0') {
        *pos = '\0';
        pos--;
    }

    if (*pos == '.') {
        *pos = '\0';
    }

    if (strcmp(tmp, "-0") == 0) {
        strcpy(tmp, "0");
    }

    Str num_str = s_cria(tmp);
    
    return num_str;
}

Str s_cria_unindo(Lista l, Str sep)
{
    Str s = s_cria("");

    for (int i = 0; i < l_tam(l); i++) {
        s_anexa(s, l_dado_pos(l, i));

        if (i == l_tam(l) - 1) break;

        s_anexa(s, sep);
    }
    
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

    if (arquivo == NULL) {
        return s;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    if (tamamanho > 0) {
        byte *buffer_arquivo = malloc(tamamanho);
        assert(buffer_arquivo != NULL);
        size_t lidos = fread(buffer_arquivo, 1, tamamanho, arquivo);

        if (lidos == (size_t)tamamanho) {
            s_define_conteudo(s, buffer_arquivo, tamamanho);
        }

        free(buffer_arquivo);
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

    char *string_c = malloc(s->nbytes + 1);
    assert(string_c != NULL);

    if (s->nbytes > 0) {
        memcpy(string_c, s->dados, s->nbytes);
    }

    string_c[s->nbytes] = '\0';

    return string_c;
}

unichar s_ch(Str_c s, int pos)
{
    s_ok(s);

    int n = s->ncaracteres;
    if (pos < 0) {
        pos = n + pos + 1;
    }

    if (pos < 0 || pos >= n) {
        return UNI_INV;
    }

    byte *ptr = u8_avanca_unichar(s->dados, pos);

    if (ptr == NULL) {
        return UNI_INV;
    }

    unichar uni;
    int nbytes_restantes = s->nbytes - (ptr - s->dados);
    int nb = u8_unichar_nos_bytes(nbytes_restantes, ptr, &uni);

    if (nb == -1) {
        return UNI_INV;
    }

    return uni;
}

double s_número(Str_c s)
{
    char *sn = s_strc(s);
    double n = strtod(sn, NULL);
    free(sn);
    return n;
}

// operações de busca e comparação {{{1

bool s_igual(Str_c s, Str_c sb)
{
    s_ok(s);
    s_ok(sb);

    if (s->nbytes != sb->nbytes) {
        return false;
    }

    if (s->ncaracteres != sb->ncaracteres) {
        return false;
    }

    if (s->nbytes == 0) {
        return true;
    }

    bool resposta = (memcmp(s->dados, sb->dados, s->nbytes) == 0);

    return resposta;
}

int s_busca_c(Str_c s, int pos, Str_c sb)
{
    return s_busca_frente(s, pos, sb, true);
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
    return s_busca_frente(s, pos, sb, false);
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
    return s_busca_tras(s, pos, sb, true);
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
    return s_busca_tras(s, pos, sb, false);
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
    s_ok(s);
    s_ok(buscada);

    int n = s->ncaracteres;
    int m = buscada->ncaracteres;
    pos = s_posicao_corrigida(pos, n);

    if (m == 0) {
        return pos;
    }

    byte *fim = s->dados + s->nbytes;
    byte *ptr = u8_avanca_unichar(s->dados, pos);
    unsigned int nb = buscada->nbytes;

    for (int i = pos; i + m <= n; i++) {
        bool cabe = (unsigned int)(fim - ptr) >= nb;
        if (cabe && memcmp(ptr, buscada->dados, nb) == 0) {
            return i;
        }
        ptr = s_proximo_unichar(ptr);
    }

    return -1;
}

// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
    s_ok(s);

    Str copia_temp = NULL;
    if (sb != NULL) {
        s_ok(sb);
        if (sb == (Str_c) s) {
            copia_temp = s_cria_cópia(sb);
            sb = copia_temp;
        }
    }

    int n = s->ncaracteres;
    s_intervalo_corrigido(pos, tam, n, &pos, &tam);

    byte *inicio_remocao = s_ptr_posicao(s, pos);
    byte *fim_remocao = u8_avanca_unichar(inicio_remocao, tam);

    int bytes_antes = inicio_remocao - s->dados;
    int bytes_removidos = fim_remocao - inicio_remocao;
    int bytes_depois = s->nbytes - bytes_antes - bytes_removidos;

    int nbytes_sb = (sb != NULL) ? sb->nbytes : 0;
    int ncaracteres_sb = (sb != NULL) ? sb->ncaracteres : 0;
    int novo_nbytes = s->nbytes - bytes_removidos + nbytes_sb;

    if (novo_nbytes == 0) {
        s_esvazia(s);
        if (copia_temp != NULL) s_destroi(copia_temp);
        s_ok(s);
        return;
    }

    int origem_depois = bytes_antes + bytes_removidos;
    int destino_depois = bytes_antes + nbytes_sb;

    if (destino_depois < origem_depois) {
        memmove(s->dados + destino_depois, s->dados + origem_depois,
            bytes_depois);
        s_garante_memoria(s, novo_nbytes);
    } else {
        s_garante_memoria(s, novo_nbytes);
        memmove(s->dados + destino_depois, s->dados + origem_depois,
            bytes_depois);
    }

    if (nbytes_sb > 0) {
        memmove(s->dados + bytes_antes, sb->dados, nbytes_sb);
    }

    s->nbytes = novo_nbytes;
    s->ncaracteres = n - tam + ncaracteres_sb;

    if (copia_temp != NULL) s_destroi(copia_temp);

    s_ok(s);
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
    s_ok(s);
    s_ok(sb);

    int m = sb->ncaracteres;
    s_intervalo_corrigido(pos, tam, m, &pos, &tam);

    byte *inicio = u8_avanca_unichar(sb->dados, pos);
    byte *fim = u8_avanca_unichar(inicio, tam);
    int nbytes = fim - inicio;

    if (nbytes == 0) {
        s_esvazia(s);
        s_ok(s);
        return;
    }

    if (sb == (Str_c) s) {
        memmove(s->dados, inicio, nbytes);
        s_garante_memoria(s, nbytes);
    } else {
        s_garante_memoria(s, nbytes);
        memcpy(s->dados, inicio, nbytes);
    }

    s->nbytes = nbytes;
    s->ncaracteres = tam;

    s_ok(s);
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

    byte buffer_utf8[4];
    int nbytes = u8_converte_pra_utf8(c, buffer_utf8);
    if (nbytes == -1) {
        return;
    }

    Str temp = s_cria("");
    s_define_conteudo(temp, buffer_utf8, nbytes);
    s_substitui(s, pos, 0, temp);
    s_destroi(temp);
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

    int n = s->ncaracteres;
    byte *ptr_inicio = s->dados;
    byte *ptr_fim = s->dados + s->nbytes;

    int fim = n;
    while (fim > 0) {
        byte *anterior = s_unichar_anterior(s->dados, ptr_fim);
        if (!s_caractere_pertence(s_le_unichar(anterior, ptr_fim), sobras)) {
            break;
        }
        ptr_fim = anterior;
        fim--;
    }

    int inicio = 0;
    while (inicio < fim) {
        if (!s_caractere_pertence(s_le_unichar(ptr_inicio, ptr_fim), sobras)) {
            break;
        }
        ptr_inicio = s_proximo_unichar(ptr_inicio);
        inicio++;
    }

    s_substitui(s, fim, n - fim, NULL);
    s_substitui(s, 0, inicio, NULL);

    s_ok(s);
}

// operações de E/S {{{1

void s_imprime(Str_c s)
{
    s_ok(s);

    if (s->nbytes > 0) {
        fwrite(s->dados, 1, s->nbytes, stdout);
    }
}

void s_grava_arquivo(Str_c s, char *nome)
{
    s_ok(s);

    FILE *arquivo = fopen(nome, "wb");
    if (arquivo == NULL) {
        return;
    }

    if (s->nbytes > 0) {
        fwrite(s->dados, 1, s->nbytes, arquivo);
    }

    fclose(arquivo);
}

// vim: foldmethod=marker shiftwidth=2