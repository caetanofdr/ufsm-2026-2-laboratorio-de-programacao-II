#include <math.h>

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

static bool eh_numero(unichar c);
static bool eh_letra(unichar c);
static bool eh_operando(unichar c);
static bool eh_espaco(unichar c);
static bool continua_nome(unichar c);
static categoria_t classifica_token(Str token);


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

static Str opera(unichar operacao, Lista pilha_operandos)
{
    if (l_tam(pilha_operandos) < 2) {
        return s_cria("#ERRO faltam operandos");
    }

    Str b = l_desempilha(pilha_operandos);
    Str a = l_desempilha(pilha_operandos);

    double va = s_número(a);
    double vb = s_número(b);
    double resultado = 0;

    switch (operacao) {
        case '+': resultado = va + vb; break;
        case '-': resultado = va - vb; break;
        case '*': resultado = va * vb; break;
        case '/': resultado = va / vb; break;
        case '^': resultado = pow(va, vb); break;
    }

    s_destroi(a);
    s_destroi(b);

    l_empilha(pilha_operandos, s_cria_número(resultado));

    return NULL;
}

static void libera_memoria_lista(Lista l)
{
    for (int i = 0; i < l_tam(l); i++) {
        s_destroi(l_dado_pos(l, i));
    }

    l_destroi(l);
}

static void liberar_memoria_calculadora(Lista p_operadores, Lista p_operandos, Lista l_tokens)
{
    libera_memoria_lista(p_operadores);
    libera_memoria_lista(p_operandos);
    libera_memoria_lista(l_tokens);

}

Str calculadora(Str expressão)
{
    Lista pilha_operadores = l_cria();
    Lista pilha_operandos = l_cria();
    Lista lista_tokens = tokeniza(expressão);

    int i = 0;

    while (true) {
        Str token_atual = NULL;
        categoria_t coluna;

        if (i >= l_tam(lista_tokens)) {
            coluna = CAT_FIM;
        } else {
            token_atual = l_dado_pos(lista_tokens, i);
            coluna = classifica_token(token_atual);
        }

        if (coluna == CAT_OPERANDO) {
            l_empilha(pilha_operandos, s_cria_cópia(token_atual));
            i++;
            continue;
        }

        if (coluna == CAT_ERRO) {
            liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
            return s_cria("#ERRO token inválido");
        }

        categoria_t linha;

        if (l_vazia(pilha_operadores)) {
            linha = CAT_VAZIA;
        } else {
            Str topo = l_topo(pilha_operadores);
            linha = classifica_token(topo);
        }

        acao_t acao = tabela[linha][coluna];

        switch (acao) {
            case EMPILHA:
                l_empilha(pilha_operadores, s_cria_cópia(token_atual));
                i++;
                break;

            case OPERA: {
                Str operador = l_desempilha(pilha_operadores);
                unichar op = s_ch(operador, 0);
                s_destroi(operador);

                Str erro = opera(op, pilha_operandos);
                if (erro != NULL) {
                    liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
                    return erro;
                }

                break;
            }

            case TERMINA: {
                if (l_tam(pilha_operandos) != 1) {
                    liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
                    return s_cria("#ERRO expressão inválida");
                }
                Str resultado = l_desempilha(pilha_operandos);
                liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
                return resultado;
            }

            case DESCARTA: {
                Str abre = l_desempilha(pilha_operadores);
                s_destroi(abre);
                i++;
                break;
            }

            case ERRO_FALTA_ABRE:
                liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
                return s_cria("#ERRO falta (");

            case ERRO_FALTA_FECHA:
                liberar_memoria_calculadora(pilha_operadores, pilha_operandos, lista_tokens);
                return s_cria("#ERRO falta )");
        }
    }
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