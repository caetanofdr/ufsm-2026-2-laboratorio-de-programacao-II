#ifndef _SOM_H_
#define _SOM_H_

// sons

#define SOM_INIMIGO_NAVE "11.3.wav"
#define SOM_ESCUDO "12.3.wav"
#define SOM_VAZIO "x.3.wav"

// funções do som

void tocaSom(char *arquivo);
void nomeArquivoSomTipo(int tipo, char *destino);
void sonar(int campo[], int tamanho);

#endif
