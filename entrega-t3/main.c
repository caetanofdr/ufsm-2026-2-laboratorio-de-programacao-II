#include <stdio.h>

#include "str.h"
#include "lista.h"
#include "calc.h"

#define ARQUIVO_ENTRADA "input.txt"
#define ARQUIVO_SAIDA "output.txt"

int main()
{
    Str sep_linhas = s_cria("\r\n");
    Str sep_saida = s_cria("\n");

    Str conteudo = s_cria_de_arquivo(ARQUIVO_ENTRADA);
    Lista linhas = l_cria_separando(conteudo, sep_linhas);

    Lista resultados = l_cria();

    for (int i = 0; i < l_tam(linhas); i++) {
        Str linha = l_dado_pos(linhas, i);
        Str resultado = calculadora(linha);
        l_insere_fim(resultados, resultado);
    }

    Str saida = s_cria_unindo(resultados, sep_saida);
    s_grava_arquivo(saida, ARQUIVO_SAIDA);

    s_destroi(saida);

    for (int i = 0; i < l_tam(resultados); i++) {
        s_destroi(l_dado_pos(resultados, i));
    }
    l_destroi(resultados);

    for (int i = 0; i < l_tam(linhas); i++) {
        s_destroi(l_dado_pos(linhas, i));
    }
    l_destroi(linhas);

    s_destroi(conteudo);
    s_destroi(sep_saida);
    s_destroi(sep_linhas);

    return 0;
}