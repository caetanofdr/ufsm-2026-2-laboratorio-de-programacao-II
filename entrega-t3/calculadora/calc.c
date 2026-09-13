#include "calc.h"
#include "str.h"
#include "lista.h"
#include "dicionario.h"

/* OPERAÇÕES */

typedef enum {
    OPERA,
    TERMINA,
    EMPILHA,
    DESCARTA,
    ERRO_FALTA_ABRE,
    ERRO_FALTA_FECHA
} acao_t;

typedef enum {
    CAT_FIM,
    CAT_VAZIA,
    CAT_ADITIVO,
    CAT_MULTIPLICATIVO,
    CAT_POTENCIA,
    CAT_ABRE,
    CAT_FECHA,
    CAT_OPERANDO,
    CAT_ERRO
} categoria_t;

/* TABELA DE PRECEDÊNCIA */

static const acao_t tabela[7][7] = {
    [CAT_VAZIA] = {
        [CAT_FIM]            = TERMINA,
        [CAT_ADITIVO]        = EMPILHA,
        [CAT_MULTIPLICATIVO] = EMPILHA,
        [CAT_POTENCIA]       = EMPILHA,
        [CAT_ABRE]           = EMPILHA,
        [CAT_FECHA]          = ERRO_FALTA_ABRE,
    },

    [CAT_ADITIVO] = {
        [CAT_FIM]            = OPERA,
        [CAT_ADITIVO]        = OPERA,
        [CAT_MULTIPLICATIVO] = EMPILHA,
        [CAT_POTENCIA]       = EMPILHA,
        [CAT_ABRE]           = EMPILHA,
        [CAT_FECHA]          = OPERA,
    },

    [CAT_MULTIPLICATIVO] = {
        [CAT_FIM]            = OPERA,
        [CAT_ADITIVO]        = OPERA,
        [CAT_MULTIPLICATIVO] = OPERA,
        [CAT_POTENCIA]       = EMPILHA,
        [CAT_ABRE]           = EMPILHA,
        [CAT_FECHA]          = OPERA,
    },

    [CAT_POTENCIA] = {
        [CAT_FIM]            = OPERA,
        [CAT_ADITIVO]        = OPERA,
        [CAT_MULTIPLICATIVO] = OPERA,
        [CAT_POTENCIA]       = EMPILHA,
        [CAT_ABRE]           = EMPILHA,
        [CAT_FECHA]          = OPERA,
    },

    [CAT_ABRE] = {
        [CAT_FIM]            = ERRO_FALTA_FECHA,
        [CAT_ADITIVO]        = EMPILHA,
        [CAT_MULTIPLICATIVO] = EMPILHA,
        [CAT_POTENCIA]       = EMPILHA,
        [CAT_ABRE]           = EMPILHA,
        [CAT_FECHA]          = DESCARTA,
    },
};

static bool eh_numero(unichar c)
{
    return 
        (c >= '0' && c <= '9') || 
        (c == '.');
}

static bool eh_letra(unichar c)
{
    return 
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') || 
        (c == '_') ||
        (c == '$');
}

static bool eh_operando(unichar c)
{
    return 
        eh_numero(c) || 
        eh_letra(c);
}

static bool eh_espaco(unichar c)
{
    return 
        (c == ' ') || 
        (c == '\n') ||
        (c == '\t') ||
        (c == '\v') ||
        (c == '\f') ||
        (c == '\r');
}

static bool continua_nome(unichar c)
{
    return 
        eh_letra(c) ||
        (c >= '0' && c <= '9');
}

static categoria_t classifica_token(Str token)
{
    unichar c = s_ch(token, 0);

    if (c == '+' || c == '-') {
        return CAT_ADITIVO;
    } else if (c == '*' || c == '/') {
        return CAT_MULTIPLICATIVO;
    } else if (c == '^') {
        return CAT_POTENCIA;
    } else if (c == '(') {
        return CAT_ABRE;
    } else if (c == ')') {
        return CAT_FECHA;
    } else if (eh_operando(c)) {
        return CAT_OPERANDO;
    }
    
    return CAT_ERRO;
}

Lista tokeniza(Str txt)
{
    Lista resultado = l_cria();
    int n = s_tam(txt);

    int i = 0;

    while (i < n) {
        unichar c = s_ch(txt, i);
        
        if (eh_espaco(c)) {
            i++;
            continue;
        }
        
        int inicio = i;
        int tam;

        if (eh_numero(c)) {
            i++;
            while (i < n && eh_numero(s_ch(txt, i))) i++;
            tam = i - inicio;
        } else if (eh_letra(c)) {
            i++;
            while (i < n && continua_nome(s_ch(txt, i))) i++;
            tam = i - inicio;
        } else {
            i++;
            tam = 1;
        } 

        Str token = s_cria_substring(txt, inicio, tam);
        l_insere_fim(resultado, token);
    }

    return resultado;
}