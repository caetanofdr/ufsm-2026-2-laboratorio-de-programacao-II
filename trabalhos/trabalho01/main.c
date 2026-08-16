#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

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

// sons

#define SOM_INIMIGO_NAVE "11.3.wav"
#define SOM_ESCUDO "12.3.wav"
#define SOM_VAZIO "x.3.wav"

// estados do programa

#define ESTADO_MENU_PRICIPAL 0
#define ESTADO_MENU_MELHORES_POTUACOES 1
#define ESTADO_FIM_ONDA 2
#define ESTADO_FIM_PARTIDA 3
#define ESTADO_ONDA_DIURNA 4
#define ESTADO_ONDA_NOTURNA 5
#define ESTADO_ONDA_SORTEAR 6
#define ESTADO_FIM_PROGRAMA -1

// tipo de ondas

#define ONDA_DIURNA 0
#define ONDA_NOTURNA 1

// elementos do campo

#define ESCUDO -1
#define VAZIO -2
#define NAVE_GRANDE 10
#define NAVE_PEQUENA 11

// armas

#define ARMA_N 10

// tiros

#define RESULTADO_ERROU 0
#define RESULTADO_DESTRUIU 1
#define RESULTADO_CONVERTEU 2

// arquivo

#define NOME_ARQUIVO_PLACAR "placar.txt"

// estruturas

// estrutura principal do jogo

typedef struct Partida
{
    unsigned char armaAtual;
    unsigned char quantidadeEscudos;
    unsigned int municao;
    unsigned int tirosAcertados;
    unsigned int tirosErrados;
    unsigned int ataquesInativos;
    unsigned char tipoOndaAtual;

    double intervaloMovimento;

    unsigned int pontuacao;
    unsigned int ondasFinalizadas;

    int campoAtaquesDiurnos[13];
    int campoAtaquesNoturnos[8];
} Partida;

typedef struct Programa
{
    char estado;
    Partida partidaAtual;
    unsigned int placar[3];
} Programa;

typedef struct timespec crono;

// protótipos

// funções de terminal e temporização

void configura_terminal();
void normaliza_terminal();
char lechar();
void crono_inicia(crono *c);
double crono_parcial(crono *c);

// funções do placar

void lerArquivoPlacar(Programa *programa);
void gravarMelhoresPontuacoes(Programa *programa);
void ordenarPlacar(Programa *programa);
void atualizarPlacar(Programa *programa);

// funções gerais do programa

void inicializarPrograma(Programa *programa);
void finalizarPrograma(Programa *programa);

// funções do menu principal

void desenharMenuPrincipal();
void menuPrincipal(Programa *programa);

// funções do placar de melhores pontuações

void desenharMelhoresPontuacoes(Programa *programa);
void melhoresPontuacoes(Programa *programa);

// funções gerais da partida

void iniciarNovaPartida(Programa *programa);
int sorteouDiurna(Programa *programa);
void sortearProximaOnda(Programa *programa);

// funções de fim de onda

void desenharFimOnda(Programa *programa);
void fimOnda(Programa *programa);

// funções de fim de partida

void desenharFimPartida(Programa *programa);
void fimPartida(Programa *programa);

// funções da onda diurna

void inicializarCampoAtaqueDiurno(Programa *programa);
void desenhaDadosDiurno(Programa *programa);
void desenhaCampoDiurno(Programa *programa);
void desenharAtaqueDiurno(Programa *programa);
void alternarArmaDiurno(Programa *programa);
int armaCorrespondeInimigo(Programa *programa, int i);
int derrotarInimigoDiurno(Programa *programa);
void atirar(Programa *programa);
int indexEscudoParaDestruir(Programa *programa);
void destruirEscudo(Programa *programa, int i);
void atualizarCampoDiurno(Programa *programa);
void processarComandoDiurno(Programa *programa, char c);
int gerarTipoAtaqueDiurno();
void nascerAtaqueDiurno(Programa *programa);
void tickOndaDiurna(Programa *programa);
int ondaDiurnaTerminou(Programa *programa);
void iniciarOndaDiurna(Programa *programa);
void ondaDiurna(Programa *programa);

// funções da onda noturna

void inicializarCampoAtaqueNoturno(Programa *programa);
void desenharAtaqueNoturno(Programa *programa);
void alternarArmaNoturna(Programa *programa);
int armaCorrespondeInimigoNoturno(Programa *programa, int i);
int derrotarInimigoNoturno(Programa *programa);
void atirarNoturno(Programa *programa);
int indexEscudoParaDestruirNoturno(Programa *programa);
void destruirEscudoNoturno(Programa *programa, int i);
void atualizarCampoNoturno(Programa *programa);
void processarComandoNoturno(Programa *programa, char c);
int gerarTipoAtaqueNoturno();
void nascerAtaqueNoturno(Programa *programa);
void tickOndaNoturna(Programa *programa);
int ondaNoturnaTerminou(Programa *programa);
void iniciarOndaNoturna(Programa *programa);
void ondaNoturna(Programa *programa);

// funções do som

void tocaSom(char *arquivo);
void nomeArquivoSomTipo(int tipo, char *destino);
void sonarDiurno(Programa *programa);
void sonarNoturno(Programa *Programa);

// funções

// funções de terminal e temporização

void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0)
    {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0)
    {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

void normaliza_terminal()
{
    system("stty sane");
}

char lechar()
{
    fflush(stdout);
    clearerr(stdin); // coloquei isso aqui pois sem nao roda no meu pc
    char c;
    if (fread(&c, 1, 1, stdin) == 1)
        return c;
    return 0;
}

void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);
    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

// funções do placar

void lerArquivoPlacar(Programa *programa) {
    FILE *arquivo = fopen(NOME_ARQUIVO_PLACAR, "r");

    if (arquivo == NULL)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        if (fscanf(arquivo, "%u", &programa->placar[i]) != 1) break;
    }

    fclose(arquivo);
}

void gravarMelhoresPontuacoes(Programa *programa)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_PLACAR, "w");
    if (arquivo == NULL) {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        fprintf(arquivo, "%u\n", programa->placar[i]);
    }

    fclose(arquivo);
}

void ordenarPlacar(Programa *programa) {
    for (int passada = 0; passada < 2; passada++)
    {
        for (int i = 0; i < 2; i++)
        {
            if (programa->placar[i] < programa->placar[i + 1])
            {
                unsigned int tmp = programa->placar[i];
                programa->placar[i] = programa->placar[i + 1];
                programa->placar[i + 1] = tmp;
            }
        }
    }
}

void atualizarPlacar(Programa *programa)
{
    unsigned int pontuacao = programa->partidaAtual.pontuacao;

    if (pontuacao <= programa->placar[2]) return;

    int i = 2;
    while (i > 0 && programa->placar[i - 1] < pontuacao)
    {
        programa->placar[i] = programa->placar[i - 1];
        i--;
    }

    programa->placar[i] = pontuacao;
}

// funções gerais do programa

void inicializarPrograma(Programa *programa)
{
    configura_terminal();

    programa->placar[0] = 0;
    programa->placar[1] = 0;
    programa->placar[2] = 0;

    lerArquivoPlacar(programa);

    ordenarPlacar(programa);

    programa->estado = ESTADO_MENU_PRICIPAL;
}

void finalizarPrograma(Programa *programa)
{
    gravarMelhoresPontuacoes(programa);

    normaliza_terminal();
}

// funções do menu principal

void desenharMenuPrincipal()
{
    system("clear");
    printf("Bem-vindo ao Invasores do Espaço!\n");
    printf("[J] - Jogar nova partida\n");
    printf("[P] - Placar das melhores pontuações\n");
    printf("[ESC] - Sair do Invasores do Espaço\n");
}

void menuPrincipal(Programa *programa)
{
    desenharMenuPrincipal();

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == 27)
        {
            finalizarPrograma(programa);
            exit(0);
        }
        else if (c == 'j' || c == 'J')
        {
            iniciarNovaPartida(programa);
            return;
        }
        else if (c == 'p' || c == 'P')
        {
            programa->estado = ESTADO_MENU_MELHORES_POTUACOES;
            return;
        }
    }
}

// funções do placar de melhores pontuações

void desenharMelhoresPontuacoes(Programa *programa) {
    system("clear");
    printf("Placar de melhores pontuações\n");

    for (int i = 0; i < 3; i++)
    {
        printf("%dº - %d pontos\n", i + 1, programa->placar[i]);
    }
    
    printf("[J] - jogar nova partida\n");
    printf("[ESC] - sair do jogo\n");
}

void melhoresPontuacoes(Programa *programa)
{
    desenharMelhoresPontuacoes(programa);

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == 27)
        {
            finalizarPrograma(programa);
            exit(0);
        }
        else if (c == 'j' || c == 'J')
        {
            iniciarNovaPartida(programa);
            return;
        }
    }
}

// funções gerais da partida

void iniciarNovaPartida(Programa *programa)
{
    programa->partidaAtual.armaAtual = 0;
    programa->partidaAtual.pontuacao = 0;
    programa->partidaAtual.ondasFinalizadas = 0;

    programa->estado = ESTADO_ONDA_SORTEAR;
}

int sorteouDiurna(Programa *programa)
{
    int probabilidadeDiurna = 100 - (int)programa->partidaAtual.ondasFinalizadas * 20;
    if (probabilidadeDiurna < 20) probabilidadeDiurna = 20;

    int sorteio = rand() % 100;

    if (sorteio < probabilidadeDiurna)
    {
        return 1;
    } else {
        return 0;
    }
}

void sortearProximaOnda(Programa *programa)
{
    if (sorteouDiurna(programa))
    {
        programa->partidaAtual.tipoOndaAtual = ONDA_DIURNA;
        programa->estado = ESTADO_ONDA_DIURNA;
    }
    else
    {
        programa->partidaAtual.tipoOndaAtual = ONDA_NOTURNA;
        programa->estado = ESTADO_ONDA_NOTURNA;
    }
}

// funções de fim de onda

void desenharFimOnda(Programa *programa) {
    system("clear");
    printf("Você terminou a onda e sobreviveu! Parabéns!\n");
    printf("Tipo de onda jogada: ");
    if (programa->partidaAtual.tipoOndaAtual == ONDA_DIURNA)
    {
        printf("diurna\n");
    } else if (programa->partidaAtual.tipoOndaAtual == ONDA_NOTURNA) {
        printf("noturna\n");
    }
    printf("Pontuação: %d\n", programa->partidaAtual.pontuacao);
    printf("Tiros restantes: %d\n", programa->partidaAtual.municao);
    printf("Tiros acertados: %d\n", programa->partidaAtual.tirosAcertados);
    printf("Tiros errados: %d\n", programa->partidaAtual.tirosErrados);
    printf("Escudos restantes: %d\n", programa->partidaAtual.quantidadeEscudos);

    printf("[R] - continuar para a próxima onda\n");
    printf("[N] - reiniciar a partida do zero\n");
    printf("[ESC] - sair do jogo\n");
}

void fimOnda(Programa *programa)
{
    tocaSom(SOM_ESCUDO);
    desenharFimOnda(programa);

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == 27)
        {
            finalizarPrograma(programa);
            exit(0);
        }
        else if (c == 'r' || c == 'R')
        {
            programa->partidaAtual.ondasFinalizadas++;
            programa->estado = ESTADO_ONDA_SORTEAR;
            return;
        }
        else if (c == 'n' || c == 'N')
        {
            iniciarNovaPartida(programa);
            return;
        }
    }
}

// funções de fim de partida

void desenharFimPartida(Programa *programa) {
    system("clear");
    printf("Fim de jogo! Você foi derrotado.\n");
    printf("Pontuação final: %d\n", programa->partidaAtual.pontuacao);
    printf("Ondas concluídas: %d\n", programa->partidaAtual.ondasFinalizadas);

    printf("[R] - reiniciar a partida do zero\n");
    printf("[P] - ver placar das melhores pontuações\n");
    printf("[ESC] - sair do jogo\n");
}

void fimPartida(Programa *programa)
{
    atualizarPlacar(programa);
    tocaSom(SOM_INIMIGO_NAVE);
    desenharFimPartida(programa);

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == 27)
        {
            finalizarPrograma(programa);
            exit(0);
        }
        else if (c == 'r' || c == 'R')
        {
            iniciarNovaPartida(programa);
            return;
        }
        else if (c == 'p' || c == 'P')
        {
            programa->estado = ESTADO_MENU_MELHORES_POTUACOES;
            return;
        }
    }
}

// funções da onda diurna

void inicializarCampoAtaqueDiurno(Programa *programa) {
    for (int i = 0; i < 13; i++)
    {
        if (i < 3)
        {
            programa->partidaAtual.campoAtaquesDiurnos[i] = ESCUDO;
        }
        else if (i > 2 && i < 13)
        {
            programa->partidaAtual.campoAtaquesDiurnos[i] = VAZIO;
        }
    }
}

void desenhaDadosDiurno(Programa *programa) {
    if (programa->partidaAtual.armaAtual <= 9) {
        printf("%3d %2d %2d",
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

void desenhaCampoDiurno(Programa *programa) {
    for (int i = 0; i < 13; i++)
    {
        if (programa->partidaAtual.campoAtaquesDiurnos[i] == ESCUDO)
        {
            printf(")");
        }
        else if (programa->partidaAtual.campoAtaquesDiurnos[i] == VAZIO)
        {
            printf(" ");
        }
        else if (programa->partidaAtual.campoAtaquesDiurnos[i] >= 0 &&
                 programa->partidaAtual.campoAtaquesDiurnos[i] <= 9)
        {
            printf("%d", programa->partidaAtual.campoAtaquesDiurnos[i]);
        }
        else if (programa->partidaAtual.campoAtaquesDiurnos[i] == NAVE_GRANDE)
        {
            printf("N");
        }
        else if (programa->partidaAtual.campoAtaquesDiurnos[i] == NAVE_PEQUENA)
        {
            printf("n");
        }
    }
}

void desenharAtaqueDiurno(Programa *programa)
{
    desenhaDadosDiurno(programa);
    desenhaCampoDiurno(programa);
    printf("\033[K"); // serve para resolver um bug que caracteres comecavam a sumir
}

void alternarArmaDiurno(Programa *programa) {
    if (programa->partidaAtual.armaAtual < ARMA_N)
    {
        programa->partidaAtual.armaAtual++;
    } else {
        programa->partidaAtual.armaAtual = 0;
    }

    char arquivo[20];
    nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
    tocaSom(arquivo);
}

int armaCorrespondeInimigo(Programa *programa, int i)
{
    int arma = programa->partidaAtual.armaAtual;
    int inimigo = programa->partidaAtual.campoAtaquesDiurnos[i];

    if (arma == ARMA_N && inimigo == NAVE_GRANDE)
    {
        return RESULTADO_CONVERTEU;
    }
    if (arma == ARMA_N && inimigo == NAVE_PEQUENA)
    {
        return RESULTADO_DESTRUIU;
    }
    if (arma == inimigo)
    {
        return RESULTADO_DESTRUIU;
    }

    return RESULTADO_ERROU;
}

int derrotarInimigoDiurno(Programa *programa) {
    for (int i = 0; i < 13; i++)
    {
        if (programa->partidaAtual.campoAtaquesDiurnos[i] == ESCUDO ||
            programa->partidaAtual.campoAtaquesDiurnos[i] == VAZIO)
        {
            continue;
        }

        int resultado = armaCorrespondeInimigo(programa, i);

        if (resultado == RESULTADO_CONVERTEU)
        {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaquesDiurnos[i] = NAVE_PEQUENA;
            return resultado;
        } else if (resultado == RESULTADO_DESTRUIU)
        {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaquesDiurnos[i] = VAZIO;
            return resultado;
        }
    }

    programa->partidaAtual.tirosErrados++;
    return RESULTADO_ERROU;
}

void atirar(Programa *programa) {
    if (programa->partidaAtual.municao <= 0) return;

    programa->partidaAtual.municao--;

    int resultado = derrotarInimigoDiurno(programa);

    if (resultado == RESULTADO_ERROU)
    {
        tocaSom(SOM_VAZIO);
    }
    else
    {
        char arquivo[20];
        nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
        tocaSom(arquivo);
    }
}

int indexEscudoParaDestruir(Programa *programa)
{
    for (int i = 2; i >= 0; i--)
    {
        if (programa->partidaAtual.campoAtaquesDiurnos[i] == ESCUDO)
        {
            return i;
        }
    }
    return -1;
}

void destruirEscudo(Programa *programa, int i)
{
    programa->partidaAtual.campoAtaquesDiurnos[i] = VAZIO;
    programa->partidaAtual.quantidadeEscudos--;
}

void atualizarCampoDiurno(Programa *programa)
{
    int atacanteQueSai = programa->partidaAtual.campoAtaquesDiurnos[3];

    if (atacanteQueSai != VAZIO)
    {
        int indiceEscudo = indexEscudoParaDestruir(programa);

        if (indiceEscudo != -1)
        {
            destruirEscudo(programa, indiceEscudo);
        }
        else
        {
            programa->estado = ESTADO_FIM_PARTIDA;
        }
    }

    for (int i = 4; i < 13; i++)
    {
        programa->partidaAtual.campoAtaquesDiurnos[i - 1] = programa->partidaAtual.campoAtaquesDiurnos[i];
    }

    programa->partidaAtual.campoAtaquesDiurnos[12] = VAZIO;
}

void processarComandoDiurno(Programa *programa, char c)
{
    switch (c)
    {
    case 9:
        alternarArmaDiurno(programa);
        break;

    case 13:
        atirar(programa);
        break;

    case 27:
        programa->estado = ESTADO_FIM_PARTIDA;
        break;

    case 32:
        sonarDiurno(programa);
        break;

    default:
        break;
    }
}

int gerarTipoAtaqueDiurno()
{
    int r = rand() % 11;
    if (r == 10) return NAVE_GRANDE;
    return r;
}

void nascerAtaqueDiurno(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos == 0) return;

    int tipo = gerarTipoAtaqueDiurno();
    programa->partidaAtual.campoAtaquesDiurnos[12] = tipo;
    programa->partidaAtual.ataquesInativos--;

    char arquivo[20];
    nomeArquivoSomTipo(tipo, arquivo);
    tocaSom(arquivo);
}

void tickOndaDiurna(Programa *programa)
{
    atualizarCampoDiurno(programa);
    nascerAtaqueDiurno(programa);
}

int ondaDiurnaTerminou(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos > 0) return 0;

    for (int i = 3; i < 13; i++)
    {
        if (programa->partidaAtual.campoAtaquesDiurnos[i] != VAZIO) return 0;
    }

    return 1;
}

void iniciarOndaDiurna(Programa *programa)
{
    programa->partidaAtual.municao = 30;
    programa->partidaAtual.quantidadeEscudos = 3;
    programa->partidaAtual.tirosAcertados = 0;
    programa->partidaAtual.tirosErrados = 0;
    programa->partidaAtual.ataquesInativos = 20;
    programa->partidaAtual.intervaloMovimento = 2.0;
    programa->partidaAtual.intervaloMovimento = 2.0 * pow(0.9, programa->partidaAtual.ondasFinalizadas);

    inicializarCampoAtaqueDiurno(programa);
}

void ondaDiurna(Programa *programa)
{
    iniciarOndaDiurna(programa);

    crono cronometro;
    crono_inicia(&cronometro);

    system("clear");
    while (1)
    {
        desenharAtaqueDiurno(programa);
        printf("\r");
        fflush(stdout);

        if (ondaDiurnaTerminou(programa) == 1)
        {
            programa->estado = ESTADO_FIM_ONDA;
            return;
        }

        char c = lechar();
        if (c != 0)
        {
            processarComandoDiurno(programa, c);
            if (programa->estado == ESTADO_FIM_PARTIDA) return;
        }

        if (crono_parcial(&cronometro) >= programa->partidaAtual.intervaloMovimento)
        {
            tickOndaDiurna(programa);
            crono_inicia(&cronometro);
            if (programa->estado == ESTADO_FIM_PARTIDA) return;
        }
    }
}

// funções da onda noturna

void inicializarCampoAtaqueNoturno(Programa *programa) {
    for (int i = 0; i < 8; i++)
    {
        if (i < 3)
        {
            programa->partidaAtual.campoAtaquesNoturnos[i] = ESCUDO;
        }
        else if (i > 2 && i < 8)
        {
            programa->partidaAtual.campoAtaquesNoturnos[i] = VAZIO;
        }
    }
}

void desenharAtaqueNoturno(Programa *programa)
{
    printf("%d", programa->partidaAtual.pontuacao);
    printf("\033[K"); // serve para resolver um bug que caracteres comecavam a sumir
}

void alternarArmaNoturna(Programa *programa) {
    unsigned char atual = programa->partidaAtual.armaAtual;

    if (atual == ARMA_N)
    {
        programa->partidaAtual.armaAtual = 0;
    } else if (atual >= 8)
    {
        programa->partidaAtual.armaAtual = ARMA_N;
    } else {
        programa->partidaAtual.armaAtual = atual + 2;
    }

    char arquivo[20];
    nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
    tocaSom(arquivo);
}

int armaCorrespondeInimigoNoturno(Programa *programa, int i)
{
    int arma = programa->partidaAtual.armaAtual;
    int inimigo = programa->partidaAtual.campoAtaquesNoturnos[i];

    if (arma == ARMA_N && inimigo == NAVE_GRANDE)
    {
        return RESULTADO_CONVERTEU;
    }
    if (arma == ARMA_N && inimigo == NAVE_PEQUENA)
    {
        return RESULTADO_DESTRUIU;
    }
    if (arma == inimigo)
    {
        return RESULTADO_DESTRUIU;
    }

    return RESULTADO_ERROU;
}

int derrotarInimigoNoturno(Programa *programa) {
    for (int i = 0; i < 8; i++)
    {
        if (programa->partidaAtual.campoAtaquesNoturnos[i] == ESCUDO ||
            programa->partidaAtual.campoAtaquesNoturnos[i] == VAZIO)
        {
            continue;
        }

        int resultado = armaCorrespondeInimigoNoturno(programa, i);

        if (resultado == RESULTADO_CONVERTEU)
        {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaquesNoturnos[i] = NAVE_PEQUENA;
            return resultado;
        } else if (resultado == RESULTADO_DESTRUIU)
        {
            programa->partidaAtual.tirosAcertados++;
            programa->partidaAtual.campoAtaquesNoturnos[i] = VAZIO;
            return resultado;
        }
    }

    programa->partidaAtual.tirosErrados++;
    return RESULTADO_ERROU;
}

void atirarNoturno(Programa *programa) {
    if (programa->partidaAtual.municao <= 0) return;

    programa->partidaAtual.municao--;

    int resultado = derrotarInimigoNoturno(programa);

    if (resultado == RESULTADO_ERROU)
    {
        tocaSom(SOM_VAZIO);
    }
    else
    {
        char arquivo[20];
        nomeArquivoSomTipo(programa->partidaAtual.armaAtual, arquivo);
        tocaSom(arquivo);
    }
}

int indexEscudoParaDestruirNoturno(Programa *programa)
{
    for (int i = 2; i >= 0; i--)
    {
        if (programa->partidaAtual.campoAtaquesNoturnos[i] == ESCUDO)
        {
            return i;
        }
    }
    return -1;
}

void destruirEscudoNoturno(Programa *programa, int i)
{
    programa->partidaAtual.campoAtaquesNoturnos[i] = VAZIO;
    programa->partidaAtual.quantidadeEscudos--;
}

void atualizarCampoNoturno(Programa *programa)
{
    int atacanteQueSai = programa->partidaAtual.campoAtaquesNoturnos[3];

    if (atacanteQueSai != VAZIO)
    {
        int indiceEscudo = indexEscudoParaDestruirNoturno(programa);

        if (indiceEscudo != -1)
        {
            destruirEscudoNoturno(programa, indiceEscudo);
        }
        else
        {
            programa->estado = ESTADO_FIM_PARTIDA;
        }
    }

    for (int i = 4; i < 8; i++)
    {
        programa->partidaAtual.campoAtaquesNoturnos[i - 1] =
            programa->partidaAtual.campoAtaquesNoturnos[i];
    }

    programa->partidaAtual.campoAtaquesNoturnos[7] = VAZIO;
}

void processarComandoNoturno(Programa *programa, char c)
{
    switch (c)
    {
    case 9:
        alternarArmaNoturna(programa);
        break;

    case 13:
        atirarNoturno(programa);
        break;

    case 27:
        programa->estado = ESTADO_FIM_PARTIDA;
        break;

    case 32:
        sonarNoturno(programa);
        break;

    default:
        break;
    }
}

int gerarTipoAtaqueNoturno()
{
    int tipos[5] = {0, 2, 4, 6, 8};
    int r = rand() % 6;
    if (r == 5) return NAVE_GRANDE;
    return tipos[r];
}

void nascerAtaqueNoturno(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos == 0) return;

    int tipo = gerarTipoAtaqueNoturno();
    programa->partidaAtual.campoAtaquesNoturnos[7] = tipo;
    programa->partidaAtual.ataquesInativos--;

    char arquivo[20];
    nomeArquivoSomTipo(tipo, arquivo);
    tocaSom(arquivo);
}

void tickOndaNoturna(Programa *programa)
{
    atualizarCampoNoturno(programa);
    nascerAtaqueNoturno(programa);
}

int ondaNoturnaTerminou(Programa *programa)
{
    if (programa->partidaAtual.ataquesInativos > 0) return 0;

    for (int i = 3; i < 8; i++)
    {
        if (programa->partidaAtual.campoAtaquesNoturnos[i] != VAZIO)
        {
            return 0;
        }
    }

    return 1;
}

void iniciarOndaNoturna(Programa *programa)
{
    programa->partidaAtual.municao = 30;
    programa->partidaAtual.quantidadeEscudos = 3;
    programa->partidaAtual.tirosAcertados = 0;
    programa->partidaAtual.tirosErrados = 0;
    programa->partidaAtual.ataquesInativos = 15;
    programa->partidaAtual.intervaloMovimento = 6.0 * pow(0.9, programa->partidaAtual.ondasFinalizadas);


    inicializarCampoAtaqueNoturno(programa);
}

void ondaNoturna(Programa *programa)
{
    iniciarOndaNoturna(programa);

    crono cronometro;
    crono_inicia(&cronometro);

    system("clear");
    while (1)
    {
        desenharAtaqueNoturno(programa);
        printf("\r");
        fflush(stdout);

        if (ondaNoturnaTerminou(programa) == 1)
        {
            programa->estado = ESTADO_FIM_ONDA;
            return;
        }

        char c = lechar();
        if (c != 0)
        {
            processarComandoNoturno(programa, c);
            if (programa->estado == ESTADO_FIM_PARTIDA) return;
        }

        if (crono_parcial(&cronometro) >=
            programa->partidaAtual.intervaloMovimento)
        {
            tickOndaNoturna(programa);
            crono_inicia(&cronometro);
            if (programa->estado == ESTADO_FIM_PARTIDA) return;
        }
    }
}

// funções do som

void tocaSom(char *arquivo)
{
    char comando[10000];
    sprintf(comando, "%s %s%s &", TOCAR_SOM, PASTA_SONS, arquivo);
    system(comando);
}

void nomeArquivoSomTipo(int tipo, char *destino) {
    if (tipo >= 0 && tipo <= 9)
        sprintf(destino, "%d.3.wav", tipo);
    else if (tipo == NAVE_GRANDE || tipo == NAVE_PEQUENA || tipo == ARMA_N)
        strcpy(destino, SOM_INIMIGO_NAVE);
    else if (tipo == ESCUDO)
        strcpy(destino, SOM_ESCUDO);
    else if (tipo == VAZIO) {
        strcpy(destino, SOM_VAZIO);
    }
}

void sonarDiurno(Programa *programa) {
    char comandoCompleto[10000];
    strcpy(comandoCompleto, "(");

    for (int i = 0; i < 13; i++)
    {
        int valor = programa->partidaAtual.campoAtaquesDiurnos[i];
        char nomeArquivo[20];
        char trecho[60];

        if (valor >= 0 && valor <= 9)
            sprintf(nomeArquivo, "%d.3.wav", valor);
        else if (valor == NAVE_GRANDE || valor == NAVE_PEQUENA)
            strcpy(nomeArquivo, SOM_INIMIGO_NAVE);
        else if (valor == ESCUDO)
            strcpy(nomeArquivo, SOM_ESCUDO);
        else if (valor == VAZIO)
            strcpy(nomeArquivo, SOM_VAZIO);
        else
            continue;

        sprintf(trecho, "%s %s%s; ", TOCAR_SOM, PASTA_SONS, nomeArquivo);
        strcat(comandoCompleto, trecho);
    }

    strcat(comandoCompleto, ") &");
    system(comandoCompleto);
}

void sonarNoturno(Programa *programa) {
    char comandoCompleto[10000];
    strcpy(comandoCompleto, "(");

    for (int i = 0; i < 8; i++)
    {
        int valor = programa->partidaAtual.campoAtaquesNoturnos[i];
        char nomeArquivo[20];
        char trecho[60];

        if (valor >= 0 && valor <= 8)
            sprintf(nomeArquivo, "%d.3.wav", valor);
        else if (valor == NAVE_GRANDE || valor == NAVE_PEQUENA)
            strcpy(nomeArquivo, SOM_INIMIGO_NAVE);
        else if (valor == ESCUDO)
            strcpy(nomeArquivo, SOM_ESCUDO);
        else if (valor == VAZIO)
            strcpy(nomeArquivo, SOM_VAZIO);
        else
            continue;

        sprintf(trecho, "%s %s%s; ", TOCAR_SOM, PASTA_SONS, nomeArquivo);
        strcat(comandoCompleto, trecho);
    }

    strcat(comandoCompleto, ") &");
    system(comandoCompleto);
}

// main

int main()
{
    Programa programa;
    inicializarPrograma(&programa);
    srand(time(NULL));

    do {
        switch (programa.estado) 
        {
            case ESTADO_MENU_PRICIPAL:
                menuPrincipal(&programa);
                break;

            case ESTADO_MENU_MELHORES_POTUACOES:
                melhoresPontuacoes(&programa);
                break;

            case ESTADO_FIM_ONDA:
                fimOnda(&programa);
                break;

            case ESTADO_FIM_PARTIDA:
                fimPartida(&programa);
                break;

            case ESTADO_ONDA_DIURNA:
                ondaDiurna(&programa);
                break;

            case ESTADO_ONDA_NOTURNA:
                ondaNoturna(&programa);
                break;

            case ESTADO_ONDA_SORTEAR:
                sortearProximaOnda(&programa);
                break;

            default:
                programa.estado = ESTADO_FIM_PROGRAMA;
                break;
        }
    } while (programa.estado != ESTADO_FIM_PROGRAMA);

    finalizarPrograma(&programa);
    return 0;
}