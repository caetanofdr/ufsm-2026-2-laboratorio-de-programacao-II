#ifndef _PROGRAMA_H_
#define _PROGRAMA_H_

// estados do programa

#define ESTADO_MENU_PRINCIPAL 0
#define ESTADO_MENU_MELHORES_PONTUACOES 1
#define ESTADO_PARTIDA 2
#define ESTADO_FIM_PROGRAMA -1

// tipo de ondas

#define ONDA_DIURNA 0
#define ONDA_NOTURNA 1

// elementos do campo

#define ESCUDO -1
#define VAZIO -2
#define NAVE_GRANDE 10
#define NAVE_PEQUENA 11

// campo

#define TAMANHO_MAXIMO_CAMPO 13

// estruturas

// configuração de um tipo de onda (definida em onda.c)

typedef struct ConfiguracaoOnda ConfiguracaoOnda;

// estrutura principal do jogo

typedef struct Partida {
    unsigned char armaAtual;
    unsigned char quantidadeEscudos;
    unsigned char tipoOndaAtual;
    unsigned int municao;
    unsigned int tirosAcertados;
    unsigned int tirosErrados;
    unsigned int ataquesInativos;

    double intervaloMovimento;

    unsigned int pontuacao;
    unsigned int ondasFinalizadas;
    unsigned char entrouTop3;
    unsigned char desistiu;

    const ConfiguracaoOnda *configuracao;
    int campoAtaques[TAMANHO_MAXIMO_CAMPO];
} Partida;

typedef struct Programa {
    int estado;
    Partida partidaAtual;
    unsigned int placar[3];
} Programa;

// funções gerais do programa

void inicializarPrograma(Programa *programa);
void finalizarPrograma(Programa *programa);

#endif
