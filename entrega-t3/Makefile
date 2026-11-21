CC = cc
CFLAGS = -Wall -Wextra -O2 -Istring -Ilista -Idicionario -Iutf8 -Icalculadora
FONTES = main.c string/str.c utf8/utf8.c dicionario/dic_simples.c lista/lista.c calculadora/calc.c
EXEC = programa

$(EXEC): $(FONTES)
	$(CC) $(CFLAGS) $(FONTES) -o $(EXEC) -lm

debug: $(FONTES)
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined $(FONTES) -o $(EXEC) -lm

clean:
	rm -f $(EXEC)
