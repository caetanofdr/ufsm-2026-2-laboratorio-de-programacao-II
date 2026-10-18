CC = cc
CFLAGS = -Wall -Wextra -O2 -Ifila -Igrafo -Irede -Ijogo -Iterminal
LDLIBS = -lm

FONTES_COMUNS = fila/fila.c grafo/grafo.c
FONTES = main.c jogo/jogo.c rede/rede.c terminal/terminal.c $(FONTES_COMUNS)
FONTES_TESTE = grafo/testa_grafo.c $(FONTES_COMUNS)
CABECALHOS = fila/fila.h grafo/grafo.h rede/rede.h jogo/jogo.h terminal/terminal.h

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
