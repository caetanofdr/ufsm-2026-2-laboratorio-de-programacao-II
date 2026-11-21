#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "programa.h"
#include "som.h"

// defines

// este define escolhe entre afplay e aplay dependendo do SO em que
// o programa for compilado, usando a macro __APPLE__ (definida
// automaticamente pelo compilador so no macOS)
// afplay eh o programa nativo do macOS para tocar arquivos de audio
// aplay eh o programa usado para tocar audio no Linux (parte do
// alsa-utils, geralmente ja vem instalado por padrao)
// fiz isso pois programei o trabalho no macOS e quis ja adaptar para o Linux

#ifdef __APPLE__
    #define TOCAR_SOM "afplay"
#else
    #define TOCAR_SOM "aplay -q"
#endif

#define PASTA_SONS "sons/"

// funções do som

// toca o arquivo de som em segundo plano
void tocaSom(char *arquivo)
{
    char comando[10000];
    sprintf(comando, "%s %s%s &", TOCAR_SOM, PASTA_SONS, arquivo);
    system(comando);
}

// coloca em destino o nome do arquivo de som do tipo dado
void nomeArquivoSomTipo(int tipo, char *destino)
{
    if (tipo >= 0 && tipo <= 9) {
        sprintf(destino, "%d.3.wav", tipo);
    } else if (tipo == NAVE_GRANDE || tipo == NAVE_PEQUENA) {
        strcpy(destino, SOM_INIMIGO_NAVE);
    } else if (tipo == ESCUDO) {
        strcpy(destino, SOM_ESCUDO);
    } else if (tipo == VAZIO) {
        strcpy(destino, SOM_VAZIO);
    }
}

// toca em sequência o som de cada uma das posições do campo
void sonar(int campo[], int tamanho)
{
    char comandoCompleto[10000];
    strcpy(comandoCompleto, "(");

    for (int i = 0; i < tamanho; i++) {
        char nomeArquivo[20];
        char trecho[60];

        nomeArquivoSomTipo(campo[i], nomeArquivo);

        sprintf(trecho, "%s %s%s; ", TOCAR_SOM, PASTA_SONS, nomeArquivo);
        strcat(comandoCompleto, trecho);
    }

    strcat(comandoCompleto, ") &");
    system(comandoCompleto);
}
