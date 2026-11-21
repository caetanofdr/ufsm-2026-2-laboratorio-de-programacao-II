#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "programa.h"
#include "onda.h"
#include "som.h"
#include "terminal.h"

// defines

// campo

#define TAMANHO_CAMPO_DIURNO 13
#define TAMANHO_CAMPO_NOTURNO 8
#define QUANTIDADE_ESCUDOS 3

// tiros

#define MUNICAO_POR_ONDA 30
#define RESULTADO_ERROU 0
#define RESULTADO_DESTRUIU 1
#define RESULTADO_CONVERTEU 2

// armas

#define ARMA_N 10

// estruturas

// o que muda entre a onda diurna e a noturna

struct ConfiguracaoOnda {
    int tamanhoCampo;
    int quantidadeAtaques;
    int multiplicadorPontos;
    double intervaloInicial;
    int passoArma;
    int quantidadeTipos;
    int tipos[10];
    unsigned char mostraCampo;
};

static const ConfiguracaoOnda CONFIGURACAO_DIURNA = {
    TAMANHO_CAMPO_DIURNO, 20, 1, 2.0, 1,
    10, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 1
};

static const ConfiguracaoOnda CONFIGURACAO_NOTURNA = {
    TAMANHO_CAMPO_NOTURNO, 15, 2, 6.0, 2,
    5, {0, 2, 4, 6, 8}, 0
};

// funções da onda

// coloca os escudos e esvazia as outras posições do campo
static void inicializarCampo(Programa *programa)
{
    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;

    for (int i = 0; i < tamanho; i++) {
        if (i < QUANTIDADE_ESCUDOS) {
            programa->partidaAtual.campoAtaques[i] = ESCUDO;
        } else {
            programa->partidaAtual.campoAtaques[i] = VAZIO;
        }
    }
}

// desenha a pontuação, os tiros e a arma
static void desenharDados(Programa *programa)
{
    if (programa->partidaAtual.armaAtual <= 9) {
        printf("%3d %2d %d",
            programa->partidaAtual.pontuacao,
            programa->partidaAtual.municao,
            programa->partidaAtual.armaAtual);
    } else if (programa->partidaAtual.armaAtual == ARMA_N) {
        printf("%3d %2d n",
            programa->partidaAtual.pontuacao,
            programa->partidaAtual.municao
        );
    }
}

// desenha os escudos e os ataques do campo
static void desenharCampo(Programa *programa)
{
    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;

    for (int i = 0; i < tamanho; i++) {
        int tipo = programa->partidaAtual.campoAtaques[i];

        if (tipo == ESCUDO) {
            printf(")");
        } else if (tipo == VAZIO) {
            printf(" ");
        } else if (tipo >= 0 && tipo <= 9) {
            printf("%d", tipo);
        } else if (tipo == NAVE_GRANDE) {
            printf("N");
        } else if (tipo == NAVE_PEQUENA) {
            printf("n");
        }
    }
}

// desenha a linha de estado da onda; na noturna, só a pontuação
static void desenharOnda(Programa *programa)
{
    if (programa->partidaAtual.configuracao->mostraCampo) {
        desenharDados(programa);
        desenharCampo(programa);
    } else {
        printf("%d", programa->partidaAtual.pontuacao);
    }
    printf("\033[K"); // serve para resolver um bug que caracteres comecavam a sumir
}

// troca para a próxima arma da onda e toca o som dela
static void alternarArma(Programa *programa)
{
    int passo = programa->partidaAtual.configuracao->passoArma;

    if (programa->partidaAtual.armaAtual < ARMA_N) {
        programa->partidaAtual.armaAtual += passo;
    } else {
        programa->partidaAtual.armaAtual = 0;
    }

    char arquivo[20];
    nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
    tocaSom(arquivo);
}

// diz se a arma atual destrói, converte ou erra o inimigo
// na posição i do campo
static int armaCorrespondeInimigo(Programa *programa, int i)
{
    int arma = programa->partidaAtual.armaAtual;
    int inimigo = programa->partidaAtual.campoAtaques[i];

    if (arma == ARMA_N && inimigo == NAVE_GRANDE) {
        return RESULTADO_CONVERTEU;
    }

    if (arma == ARMA_N && inimigo == NAVE_PEQUENA) {
        return RESULTADO_DESTRUIU;
    }

    if (arma == inimigo) {
        return RESULTADO_DESTRUIU;
    }

    return RESULTADO_ERROU;
}

// soma os pontos do inimigo destruído na posição i: quanto mais
// longe da base, mais pontos, multiplicados conforme a onda
static void pontuarInimigoDerrotado(Programa *programa, int i, int tipo)
{
    const ConfiguracaoOnda *config = programa->partidaAtual.configuracao;
    int distancia = config->tamanhoCampo - i;
    int pontuacao = 0;

    if (tipo <= 9) {
        pontuacao = distancia * config->multiplicadorPontos;
    } else if (tipo == NAVE_PEQUENA) {
        pontuacao = distancia * 2 * config->multiplicadorPontos;
    }

    programa->partidaAtual.pontuacao += pontuacao;
}

// acerta o inimigo mais à esquerda compatível com a arma;
// retorna o resultado do tiro
static int derrotarInimigo(Programa *programa)
{
    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;

    for (int i = 0; i < tamanho; i++) {
        int tipo = programa->partidaAtual.campoAtaques[i];

        if (tipo == ESCUDO || tipo == VAZIO) continue;

        int resultado = armaCorrespondeInimigo(programa, i);

        if (resultado == RESULTADO_CONVERTEU) {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaques[i] = NAVE_PEQUENA;
            return resultado;
        } else if (resultado == RESULTADO_DESTRUIU) {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaques[i] = VAZIO;
            pontuarInimigoDerrotado(programa, i, tipo);
            return resultado;
        }
    }

    programa->partidaAtual.tirosErrados++;
    return RESULTADO_ERROU;
}

// gasta um tiro e toca o som do resultado
static void atirar(Programa *programa)
{
    if (programa->partidaAtual.municao <= 0) {
        tocaSom(SOM_VAZIO);
        return;
    }

    programa->partidaAtual.municao--;

    int resultado = derrotarInimigo(programa);

    if (resultado == RESULTADO_ERROU) {
        tocaSom(SOM_VAZIO);
    } else {
        char arquivo[20];
        nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
        tocaSom(arquivo);
    }
}

// retorna a posição do escudo mais à direita, ou -1 se não houver
static int indexEscudoParaDestruir(Programa *programa)
{
    for (int i = QUANTIDADE_ESCUDOS - 1; i >= 0; i--) {
        if (programa->partidaAtual.campoAtaques[i] == ESCUDO) {
            return i;
        }
    }

    return -1;
}

// destrói o escudo da posição i do campo
static void destruirEscudo(Programa *programa, int i)
{
    programa->partidaAtual.campoAtaques[i] = VAZIO;
    programa->partidaAtual.quantidadeEscudos--;
}

// move os ataques para a esquerda, tratando colisões com escudos
// e com a base; retorna 0 se um ataque chegou na base
static int atualizarCampo(Programa *programa)
{
    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;
    int frente = programa->partidaAtual.quantidadeEscudos;
    int atacanteQueSai = programa->partidaAtual.campoAtaques[frente];

    if (atacanteQueSai != VAZIO) {
        int indiceEscudo = indexEscudoParaDestruir(programa);

        if (indiceEscudo == -1) return 0;

        destruirEscudo(programa, indiceEscudo);
    }

    for (int i = frente + 1; i < tamanho; i++) {
        programa->partidaAtual.campoAtaques[i - 1] =
            programa->partidaAtual.campoAtaques[i];
    }

    programa->partidaAtual.campoAtaques[tamanho - 1] = VAZIO;
    return 1;
}

// executa o comando da tecla digitada; retorna 0 se o jogador
// desistiu da partida
static int processarComando(Programa *programa, char tecla)
{
    switch (tecla) {
    case 9:
        alternarArma(programa);
        break;

    case 13:
        atirar(programa);
        break;

    case 27:
        programa->partidaAtual.desistiu = 1;
        return 0;

    case 32:
        sonar(programa->partidaAtual.campoAtaques,
            programa->partidaAtual.configuracao->tamanhoCampo);
        break;
    }

    return 1;
}

// sorteia o tipo de um novo ataque entre os tipos da onda
static int gerarTipoAtaque(Programa *programa)
{
    const ConfiguracaoOnda *config = programa->partidaAtual.configuracao;
    int sorteio = rand() % (config->quantidadeTipos + 1);

    if (sorteio == config->quantidadeTipos) return NAVE_GRANDE;
    return config->tipos[sorteio];
}

// coloca um novo ataque na última posição do campo
static void nascerAtaque(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos == 0) return;

    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;
    int tipo = gerarTipoAtaque(programa);
    programa->partidaAtual.campoAtaques[tamanho - 1] = tipo;
    programa->partidaAtual.ataquesInativos--;

    char arquivo[20];
    nomeArquivoSomTipo(tipo, arquivo);
    tocaSom(arquivo);
}

// executa um passo de tempo da onda; retorna 0 se um ataque
// chegou na base, sem fazer nascer um novo ataque nesse caso
static int tickOnda(Programa *programa)
{
    if (!atualizarCampo(programa)) return 0;

    nascerAtaque(programa);
    return 1;
}

// retorna 1 se não há mais ataques ativos nem inativos
static int ondaTerminou(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos > 0) return 0;

    int tamanho = programa->partidaAtual.configuracao->tamanhoCampo;

    for (int i = 0; i < tamanho; i++) {
        int tipo = programa->partidaAtual.campoAtaques[i];
        if (tipo != VAZIO && tipo != ESCUDO) return 0;
    }

    return 1;
}

// soma os pontos de fim de onda pelos tiros e escudos restantes,
// multiplicados conforme a onda
static void pontuarFimOnda(Programa *programa)
{
    const ConfiguracaoOnda *config = programa->partidaAtual.configuracao;
    unsigned int pontuacao = programa->partidaAtual.pontuacao;
    unsigned int escudos = programa->partidaAtual.quantidadeEscudos;
    unsigned int multiplicador = config->multiplicadorPontos;

    pontuacao += programa->partidaAtual.municao * 2 * multiplicador;
    pontuacao += escudos * 10 * multiplicador;

    programa->partidaAtual.pontuacao = pontuacao;
}

// calcula o intervalo entre movimentos da onda
static double calcularIntervaloMovimento(Programa *programa)
{
    const ConfiguracaoOnda *config = programa->partidaAtual.configuracao;
    unsigned int ondas = programa->partidaAtual.ondasFinalizadas;

    return config->intervaloInicial * pow(0.9, ondas);
}

// escolhe a configuração da onda sorteada, recarrega tiros, ataques
// e escudos e prepara o campo
static void iniciarOnda(Programa *programa)
{
    if (programa->partidaAtual.tipoOndaAtual == ONDA_DIURNA) {
        programa->partidaAtual.configuracao = &CONFIGURACAO_DIURNA;
    } else {
        programa->partidaAtual.configuracao = &CONFIGURACAO_NOTURNA;
    }

    const ConfiguracaoOnda *config = programa->partidaAtual.configuracao;
    programa->partidaAtual.armaAtual = 0;
    programa->partidaAtual.municao = MUNICAO_POR_ONDA;
    programa->partidaAtual.quantidadeEscudos = QUANTIDADE_ESCUDOS;
    programa->partidaAtual.tirosAcertados = 0;
    programa->partidaAtual.tirosErrados = 0;
    programa->partidaAtual.ataquesInativos = config->quantidadeAtaques;

    double novoIntervalo = calcularIntervaloMovimento(programa);
    programa->partidaAtual.intervaloMovimento = novoIntervalo;

    inicializarCampo(programa);
}

// executa um passo de tempo se o intervalo passou;
// retorna 1 se a partida terminou
static int processarTempo(Programa *programa, crono *cronometro)
{
    double passado = crono_parcial(cronometro);
    if (passado < programa->partidaAtual.intervaloMovimento) return 0;

    int sobreviveu = tickOnda(programa);
    crono_inicia(cronometro);
    return !sobreviveu;
}

// executa a onda sorteada até ela terminar; retorna 1 se o jogador
// sobreviveu à onda e 0 se a partida terminou
int jogarOnda(Programa *programa)
{
    iniciarOnda(programa);

    crono cronometro;
    crono_inicia(&cronometro);

    system("clear");
    while (1) {
        desenharOnda(programa);
        printf("\r");
        fflush(stdout);

        if (ondaTerminou(programa) == 1) {
            pontuarFimOnda(programa);
            return 1;
        }

        char tecla = lerTecla();
        if (tecla != 0 && !processarComando(programa, tecla)) return 0;

        if (processarTempo(programa, &cronometro)) return 0;
    }
}
