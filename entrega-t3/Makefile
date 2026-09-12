INCLUDES = -Istring -Ilista -Idicionario -Iutf8 -Icalculadora

CFLAGS_COMUM = -Wall -Wextra -Wpedantic -Wshadow -std=c11 $(INCLUDES)
CFLAGS_DEBUG = $(CFLAGS_COMUM) -g -fsanitize=address,undefined
CFLAGS_RELEASE = $(CFLAGS_COMUM) -O2

FONTES = main.c string/str.c utf8/utf8.c dicionario/dic_simples.c lista/lista.c calculadora/calc.c
EXEC = calculadora

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(EXEC)

release: CFLAGS = $(CFLAGS_RELEASE)
release: $(EXEC)

$(EXEC): $(FONTES)
	gcc $(CFLAGS) $(FONTES) -o $(EXEC)

clean:
	rm -f $(EXEC)