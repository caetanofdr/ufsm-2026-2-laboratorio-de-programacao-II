#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// defines

// estados do programa

#define ESTADO_MENU_PRICIPAL 0
#define ESTADO_MENU_FIM_PARTIDA 1
#define ESTADO_MENU_MELHORES_POTUACOES 2
#define ESTADO_ONDA_DIURNA 3
#define ESTADO_ONDA_NOTURNA 4
#define ESTADO_ONDA_SORTEAR 5
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

// estruturas

// estrutura principal do jogo

typedef struct Partida
{
    unsigned char armaAtual;
    unsigned char quantidadeEscudos;
    unsigned char municao;
    unsigned char ataquesInativos;
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
} Programa;

typedef struct timespec crono;

// protopipos

void configura_terminal();
void normaliza_terminal();
char lechar();
void crono_inicia(crono *c);
double crono_parcial(crono *c);
void inicializarPrograma(Programa *programa);
void finalizarPrograma(Programa *programa);
void desenharMenuPrincipal();
void menuPrincipal(Programa *programa);
void iniciarNovaPartida(Programa *programa);
void desenharAtaqueDiurno(Programa *programa);
void alternarArma(Programa *programa);
int armaCorrespondeInimigo(Programa *programa, int i);
void derrotarInimigoDiurno(Programa *programa);
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

// funcoes

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

void inicializarPrograma(Programa *programa)
{
    configura_terminal();

    programa->estado = ESTADO_MENU_PRICIPAL;
}

void finalizarPrograma(Programa *programa)
{
    normaliza_terminal();
}

void desenharMenuPrincipal()
{
    system("clear");
    printf("Bem-vindo ao Invasores do Espaço!\n");
    printf("1 - Jogar nova partida\n");
    printf("2 - Placar das melhores pontuações\n");
    printf("3 - Sair do Invasozer do Espaço\n");

    printf("Sua escolha: ");
}

void menuPrincipal(Programa *programa)
{
    char areaTecla[2];
    int tamanhoAreaTecla = 0;
    areaTecla[0] = '\0';

    desenharMenuPrincipal();

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == '\r' || c == '\n')
        {
            break;
        }
        else if (c == 127 || c == 8)
        {
            if (tamanhoAreaTecla > 0)
            {
                tamanhoAreaTecla--;
                areaTecla[tamanhoAreaTecla] = '\0';
                printf("\b \b");
            }
        }
        else if (tamanhoAreaTecla < 1)
        {
            areaTecla[tamanhoAreaTecla] = c;
            tamanhoAreaTecla++;
            areaTecla[tamanhoAreaTecla] = '\0';
            putchar(c);
        }
    }

    int escolha = atoi(areaTecla);

    switch (escolha)
    {
    case 1:
        iniciarNovaPartida(programa);
        break;
    case 2:
        programa->estado = ESTADO_MENU_MELHORES_POTUACOES;
        break;
    case 3:
        programa->estado = ESTADO_FIM_PROGRAMA;
        break;
    default:
        programa->estado = ESTADO_MENU_PRICIPAL;
        break;
    }
}

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

void iniciarNovaPartida(Programa *programa)
{
    programa->partidaAtual.armaAtual = 0;
    programa->partidaAtual.pontuacao = 0;
    programa->partidaAtual.municao = 30;
    programa->partidaAtual.quantidadeEscudos = 3;
    programa->partidaAtual.ondasFinalizadas = 0;
    inicializarCampoAtaqueDiurno(programa);

    programa->estado = ESTADO_ONDA_SORTEAR;
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
    desenhaDadosDiurno(programa);
}

void alternarArma(Programa *programa) {
    if (programa->partidaAtual.armaAtual < ARMA_N)
    {
        programa->partidaAtual.armaAtual++;
    } else {
        programa->partidaAtual.armaAtual = 0;
    }
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

void derrotarInimigoDiurno(Programa *programa) {
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
            programa->partidaAtual.campoAtaquesDiurnos[i] = NAVE_PEQUENA;
            break;
        } else if (resultado == RESULTADO_DESTRUIU)
        {
            programa->partidaAtual.campoAtaquesDiurnos[i] = VAZIO;
            break;
        }
    }
}

void atirar(Programa *programa) {
    if (programa->partidaAtual.municao <= 0) return;

    programa->partidaAtual.municao--;

    derrotarInimigoDiurno(programa);
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
            programa->estado = ESTADO_MENU_FIM_PARTIDA;
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
        alternarArma(programa);
        break;

    case 13:
        atirar(programa);
        break;

    case 27:
        finalizarPrograma(programa);
        exit(0);

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

    programa->partidaAtual.campoAtaquesDiurnos[12] = gerarTipoAtaqueDiurno();
    programa->partidaAtual.ataquesInativos--;
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
    programa->partidaAtual.ataquesInativos = 20;
    programa->partidaAtual.intervaloMovimento = 2.0;

    for (int i = 3; i < 13; i++)
    {
        programa->partidaAtual.campoAtaquesDiurnos[i] = VAZIO;
    }
}

void ondaDiurna(Programa *programa)
{
    iniciarOndaDiurna(programa);

    crono cronometro;
    crono_inicia(&cronometro);

    system("clear");
    while (programa->partidaAtual.tipoOndaAtual == ONDA_DIURNA && !ondaDiurnaTerminou(programa))
    {
        desenharAtaqueDiurno(programa);
        printf("\r");
        fflush(stdout);

        char c = lechar();
        if (c != 0)
        {
            processarComandoDiurno(programa, c);
        }

        if (crono_parcial(&cronometro) >= programa->partidaAtual.intervaloMovimento)
        {
            tickOndaDiurna(programa);
            crono_inicia(&cronometro);
        }
    }
}

int sorteouDiurna(Programa *programa)
{
    int probabilidadeDiurna = 100 - (int)programa->partidaAtual.ondasFinalizadas * 20;
    if (probabilidadeDiurna < 20) probabilidadeDiurna = 20;

    int sorteio = rand() % 100;

    if (sorteio <= probabilidadeDiurna)
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

            case ESTADO_MENU_FIM_PARTIDA:

                break;

            case ESTADO_MENU_MELHORES_POTUACOES:

                break;

            case ESTADO_ONDA_DIURNA:
                ondaDiurna(&programa);
                break;

            case ESTADO_ONDA_NOTURNA:

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