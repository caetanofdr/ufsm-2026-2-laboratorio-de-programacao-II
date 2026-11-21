CC = cc
CFLAGS = -g -Wall -Wextra -Istr -Iutf8

OBJS = teste_str.c str/str.c utf8/utf8.c

teste_str: $(OBJS) str/str.h utf8/utf8.h
	$(CC) $(CFLAGS) -o teste_str $(OBJS)

run: teste_str
	./teste_str

clean:
	rm -rf teste_str teste_str.dSYM