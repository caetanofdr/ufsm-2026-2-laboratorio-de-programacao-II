CC = cc
CFLAGS = -Wall -Wextra -Iprograma -Iterminal -Iplacar -Imenus -Ipartida \
	-Ionda -Isom
LDLIBS = -lm

FONTES = main.c programa/programa.c terminal/terminal.c placar/placar.c \
	menus/menus.c partida/partida.c onda/onda.c som/som.c
CABECALHOS = programa/programa.h terminal/terminal.h placar/placar.h \
	menus/menus.h partida/partida.h onda/onda.h som/som.h

main: $(FONTES) $(CABECALHOS)
	$(CC) $(CFLAGS) -o main $(FONTES) $(LDLIBS)

debug: $(FONTES) $(CABECALHOS)
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined -o main $(FONTES) $(LDLIBS)

run: main
	./main

clean:
	rm -rf main main.dSYM
