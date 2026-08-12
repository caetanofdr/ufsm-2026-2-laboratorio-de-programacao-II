#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// defines

// estados do programa

#define ESTADO_MENU_PRICIPAL 0
#define ESTADO_MENU_FIM_PARTIDA 1
#define ESTADO_MENU_MELHORES_POTUACOES 2
#define ESTADO_RODADA_DIURNA 3
#define ESTADO_RODADA_NOTURNA 4
#define ESTADO_FIM_PROGRAMA -1

// elementos do campo

#define ESCUDO -1
#define VAZIO -2

// estruturas

// estrutura principal do jogo

typedef struct  Partida
{
    unsigned char armaAtual;
    unsigned char tipoOrda;
    unsigned char quantidadeEscudos;
    unsigned char municao;

    unsigned int pontuacao;

    int campoAtaquesDiurnos[13];
    int campoAtaquesNoturnos[8];
} Partida;

typedef struct Programa
{
    char estado;
    Partida partidaAtual;
} Programa;

typedef struct timespec crono;

void configura_terminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
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
    if (fread(&c, 1, 1, stdin) == 1) return c;
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
    char buffer[2];
    int tam = 0;
    buffer[0] = '\0';

    desenharMenuPrincipal();

    for (;;)
    {
        char c = lechar();
        if (c == 0) continue;

        if (c == '\r' || c == '\n')
        {
            break;
        } else if (c == 127 || c == 8)
        {
            if (tam > 0)
            {
                tam--;
                buffer[tam] = '\0';
                printf("\b \b");
            }
        } else if (tam < 1)
        {
            buffer[tam] = c;
            tam++;
            buffer[tam] = '\0';
            putchar(c);
        }
    }

    int escolha = atoi(buffer);

    switch (escolha)
    {
    case 1:
        programa->estado = ESTADO_RODADA_DIURNA;
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

int main()
{
    Programa programa;
    inicializarPrograma(&programa);
    
    do
    {
        switch (programa.estado)
        {
        case ESTADO_MENU_PRICIPAL:
            menuPrincipal(&programa);
            break;
        
        case ESTADO_MENU_FIM_PARTIDA:
            
            break;

        case ESTADO_MENU_MELHORES_POTUACOES:

            break;

        case ESTADO_RODADA_DIURNA:

            break;

        case ESTADO_RODADA_NOTURNA:

            break;
        default:
            programa.estado = 0;
            break;
        }
    } while (programa.estado != ESTADO_FIM_PROGRAMA);
    
    finalizarPrograma(&programa);
    return 0;
}