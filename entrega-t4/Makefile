# ==========================================================================
# Jeitos de compilar
# --------------------------------------------------------------------------
#   make              compila tudo: ./cobra e ./testa_grafo
#   make cobra        compila só o jogo (com rede e treinamento)
#   make testa_grafo  compila só o teste do grafo
#   make debug        apaga e recompila tudo com -g -O0 e sanitizer
#                     (address e undefined), para achar erros de memória
#   make clean        apaga os executáveis gerados
#
# Jeitos de executar
# --------------------------------------------------------------------------
#   ./cobra                jogo normal, pelo teclado (k/e direita, j/n esquerda)
#   ./cobra rede/rede1     jogo controlado pela rede lida do arquivo
#   ./cobra treina         treina redes; a melhor de cada geração vai para G/
#   ./cobra treina 42      o segundo argumento é a semente dos números aleatórios
#   ./cobra G/0010-000     jogo controlado por uma rede gerada no treino
#   ./testa_grafo          testa o grafo (ordem topológica)
# ==========================================================================

CC = cc
MODULOS = main fila grafo jogo rede terminal treina geracao especie individuo auxiliar config
CFLAGS = -Wall -Wextra -O2 $(addprefix -I,$(MODULOS))
LDLIBS = -lm

FONTES_COMUNS = fila/fila.c grafo/grafo.c
FONTES_TREINO = treina/treina.c geracao/geracao.c especie/especie.c individuo/individuo.c auxiliar/auxiliar.c
FONTES = main/main.c jogo/jogo.c rede/rede.c terminal/terminal.c $(FONTES_TREINO) $(FONTES_COMUNS)
FONTES_TESTE = grafo/testa_grafo.c $(FONTES_COMUNS)
CABECALHOS = $(wildcard $(addsuffix /*.h,$(MODULOS)))

EXEC = cobra
EXEC_TESTE = testa_grafo

all: $(EXEC) $(EXEC_TESTE)

$(EXEC): $(FONTES) $(CABECALHOS)
	$(CC) $(CFLAGS) $(FONTES) -o $(EXEC) $(LDLIBS)

$(EXEC_TESTE): $(FONTES_TESTE) $(CABECALHOS)
	$(CC) $(CFLAGS) $(FONTES_TESTE) -o $(EXEC_TESTE) $(LDLIBS)

debug: CFLAGS += -g -O0 -fsanitize=address,undefined
debug: clean all

clean:
	rm -f $(EXEC) $(EXEC_TESTE)
	rm -rf $(EXEC).dSYM $(EXEC_TESTE).dSYM

.PHONY: all debug clean
