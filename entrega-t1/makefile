CC = cc
CFLAGS = -Wall -Wextra
LDLIBS = -lm

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LDLIBS)

debug: main.c
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined -o main main.c $(LDLIBS)

run: main
	./main

clean:
	rm -rf main main.dSYM
