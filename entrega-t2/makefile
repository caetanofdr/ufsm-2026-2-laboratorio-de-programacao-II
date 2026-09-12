CC = gcc
CFLAGS = -g -Wall -Wextra

OBJS = teste_str.c str.c utf8.c

teste_str: $(OBJS) str.h utf8.h
	$(CC) $(CFLAGS) -o teste_str $(OBJS)

run: teste_str
	./teste_str