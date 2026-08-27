
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void testa_criacao_e_acesso(void)
{
    Str s_vazia = s_cria("");
    assert(s_tam(s_vazia) == 0);
    assert(s_ch(s_vazia, 0) == UNI_INV);
    assert(s_ch(s_vazia, -1) == UNI_INV);
    char *c_vazia = s_strc(s_vazia);
    assert(strcmp(c_vazia, "") == 0);
    free(c_vazia);
    s_destroi(s_vazia);

    Str s_nula = s_cria(NULL);
    assert(s_tam(s_nula) == 0);
    s_destroi(s_nula);

    Str s_utf8 = s_cria("Rábica € 𝄞 😀");
    assert(s_tam(s_utf8) == 12);
    assert(s_ch(s_utf8, 0) == 'R');
    assert(s_ch(s_utf8, 1) == 0x00E1);
    assert(s_ch(s_utf8, 2) == 'b');
    assert(s_ch(s_utf8, -2) == 0x1F600);
    assert(s_ch(s_utf8, -3) == ' ');
    assert(s_ch(s_utf8, -4) == 0x1D11E);
    assert(s_ch(s_utf8, 50) == UNI_INV);

    char *c_utf8 = s_strc(s_utf8);
    assert(strcmp(c_utf8, "Rábica € 𝄞 😀") == 0);
    free(c_utf8);

    Str copia = s_cria_cópia(s_utf8);
    assert(s_igual(copia, s_utf8));
    s_destroi(copia);

    Str sub = s_cria_substring(s_utf8, 0, 6);
    char *c_sub = s_strc(sub);
    assert(strcmp(c_sub, "Rábica") == 0);
    free(c_sub);
    s_destroi(sub);

    s_destroi(s_utf8);
}

static void testa_comparacao(void)
{
    Str s1 = s_cria("Programação");
    Str s2 = s_cria("Programação");
    Str s3 = s_cria("Programa");
    Str s4 = s_cria("");
    Str s5 = s_cria("");

    assert(s_igual(s1, s2));
    assert(!s_igual(s1, s3));
    assert(!s_igual(s1, s4));
    assert(s_igual(s4, s5));

    s_destroi(s1);
    s_destroi(s2);
    s_destroi(s3);
    s_destroi(s4);
    s_destroi(s5);
}

static void testa_buscas(void)
{
    Str texto = s_cria("banana maçã");
    Str conjunto = s_cria("aã");
    Str nao_espaco = s_cria(" ");
    Str sub = s_cria("na");
    Str vazia = s_cria("");

    assert(s_busca_c(texto, 0, conjunto) == 1);
    assert(s_busca_c(texto, 2, conjunto) == 3);
    assert(s_busca_c(texto, 8, conjunto) == 8);
    assert(s_busca_c(texto, -2, conjunto) == 10);

    assert(s_busca_nc(texto, 0, conjunto) == 0);
    assert(s_busca_nc(texto, 1, nao_espaco) == 1);
    assert(s_busca_nc(texto, 6, nao_espaco) == 7);

    assert(s_busca_rc(texto, 0, conjunto) == -1);
    assert(s_busca_rc(texto, 4, conjunto) == 3);
    assert(s_busca_rc(texto, -1, conjunto) == 10);

    assert(s_busca_rnc(texto, 2, conjunto) == 0);
    assert(s_busca_rnc(texto, 7, nao_espaco) == 5);

    assert(s_busca_s(texto, 0, sub) == 2);
    assert(s_busca_s(texto, 3, sub) == 4);
    assert(s_busca_s(texto, 5, sub) == -1);
    assert(s_busca_s(texto, 0, vazia) == 0);
    assert(s_busca_s(texto, 3, vazia) == 3);
    assert(s_busca_s(texto, -1, vazia) == 11);

    s_destroi(texto);
    s_destroi(conjunto);
    s_destroi(nao_espaco);
    s_destroi(sub);
    s_destroi(vazia);
}

static void testa_substituicao_e_remocao(void)
{
    Str s = s_cria("abácaxi");
    Str rep1 = s_cria("te");
    s_substitui(s, 5, 2, rep1);
    char *c1 = s_strc(s);
    assert(strcmp(c1, "abácate") == 0);
    free(c1);
    s_destroi(rep1);

    Str ponto = s_cria(".");
    s_substitui(s, 7, 0, ponto);
    char *c2 = s_strc(s);
    assert(strcmp(c2, "abácate.") == 0);
    free(c2);

    s_substitui(s, -1, 5, ponto);
    char *c3 = s_strc(s);
    assert(strcmp(c3, "abácate..") == 0);
    free(c3);
    s_destroi(ponto);

    s_remove(s, -3, 2);
    char *c4 = s_strc(s);
    assert(strcmp(c4, "abácate") == 0);
    free(c4);

    Str s_num = s_cria("abácaxi");
    Str ins = s_cria("123");
    s_substitui(s_num, -4, 0, ins);
    char *c5 = s_strc(s_num);
    assert(strcmp(c5, "abác123axi") == 0);
    free(c5);

    s_copia(s_num, s);
    s_substitui(s_num, -4, 1, ins);
    char *c6 = s_strc(s_num);
    assert(strcmp(c6, "abác123te") == 0);
    free(c6);

    s_copia(s_num, s);
    s_substitui(s_num, 0, 1, ins);
    char *c7 = s_strc(s_num);
    assert(strcmp(c7, "123bácate") == 0);
    free(c7);

    s_copia(s_num, s);
    s_substitui(s_num, -9, 1, ins);
    char *c8 = s_strc(s_num);
    assert(strcmp(c8, "123abácate") == 0);
    free(c8);

    s_substitui(s_num, 2, 200, NULL);
    char *c9 = s_strc(s_num);
    assert(strcmp(c9, "12") == 0);
    free(c9);

    s_destroi(s_num);
    s_destroi(ins);
    s_destroi(s);
}

static void testa_substring(void)
{
    Str base = s_cria("bárcó");
    Str s = s_cria("");

    s_substring(s, base, 2, 2);
    char *c1 = s_strc(s);
    assert(strcmp(c1, "rc") == 0);
    free(c1);

    s_substring(s, base, 2, 4);
    char *c2 = s_strc(s);
    assert(strcmp(c2, "rcó") == 0);
    free(c2);

    s_substring(s, base, 2, -1);
    char *c3 = s_strc(s);
    assert(strcmp(c3, "rcó") == 0);
    free(c3);

    s_substring(s, base, -3, 1);
    char *c4 = s_strc(s);
    assert(strcmp(c4, "c") == 0);
    free(c4);

    s_substring(s, base, -7, 3);
    char *c5 = s_strc(s);
    assert(strcmp(c5, "bá") == 0);
    free(c5);

    s_substring(s, base, 10, 5);
    char *c6 = s_strc(s);
    assert(strcmp(c6, "") == 0);
    free(c6);

    s_destroi(s);
    s_destroi(base);
}

static void testa_insercao_e_anexo(void)
{
    Str s = s_cria("mundo");
    Str prefixo = s_cria("olá ");
    s_insere(s, 0, prefixo);
    char *c1 = s_strc(s);
    assert(strcmp(c1, "olá mundo") == 0);
    free(c1);

    s_insere_c(s, 3, '!');
    char *c2 = s_strc(s);
    assert(strcmp(c2, "olá! mundo") == 0);
    free(c2);

    Str sufixo = s_cria(" fim");
    s_anexa(s, sufixo);
    char *c3 = s_strc(s);
    assert(strcmp(c3, "olá! mundo fim") == 0);
    free(c3);

    s_anexa_c(s, 0x1F600);
    assert(s_ch(s, -2) == 0x1F600);

    s_destroi(s);
    s_destroi(prefixo);
    s_destroi(sufixo);
}

static void testa_apara(void)
{
    Str s1 = s_cria("teste 1");
    Str lixo = s_cria(" .");
    s_apara(s1, lixo);
    char *c1 = s_strc(s1);
    assert(strcmp(c1, "teste 1") == 0);
    free(c1);

    Str s2 = s_cria("... teste 2. .");
    s_apara(s2, lixo);
    char *c2 = s_strc(s2);
    assert(strcmp(c2, "teste 2") == 0);
    free(c2);

    Str s3 = s_cria("....   ...");
    s_apara(s3, lixo);
    assert(s_tam(s3) == 0);

    s_destroi(s1);
    s_destroi(s2);
    s_destroi(s3);
    s_destroi(lixo);
}

static void testa_autoatribuicao(void)
{
    Str s = s_cria("repetir");
    s_copia(s, s);
    char *c1 = s_strc(s);
    assert(strcmp(c1, "repetir") == 0);
    free(c1);

    s_substring(s, s, 2, 3);
    char *c2 = s_strc(s);
    assert(strcmp(c2, "pet") == 0);
    free(c2);

    s_substitui(s, 0, 0, s);
    char *c3 = s_strc(s);
    assert(strcmp(c3, "petpet") == 0);
    free(c3);

    s_destroi(s);
}

static void testa_arquivo(void)
{
    char const *nome = "teste_temp.txt";
    Str original = s_cria("Gravação e Leitura UTF-8: áéíóú € 𝄞");
    s_grava_arquivo(original, (char *)nome);

    Str carregada = s_cria_de_arquivo((char *)nome);
    assert(s_igual(original, carregada));

    remove(nome);

    Str inexistente = s_cria_de_arquivo("arquivo_que_nao_existe_12345.xyz");
    assert(s_tam(inexistente) == 0);

    s_destroi(original);
    s_destroi(carregada);
    s_destroi(inexistente);
}

int main(void)
{
    testa_criacao_e_acesso();
    testa_comparacao();
    testa_buscas();
    testa_substituicao_e_remocao();
    testa_substring();
    testa_insercao_e_anexo();
    testa_apara();
    testa_autoatribuicao();
    testa_arquivo();

    Str ok = s_cria("Todos os testes passaram com sucesso!\n");
    s_imprime(ok);
    s_destroi(ok);

    return 0;
}