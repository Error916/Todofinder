CFLAGS=-Wall -Wextra -std=c11 -pedantic -O3 -ggdb
LIBS=
SRC= src/main.c src/recdir.c
SRCEXP= src/experimental.c src/recdir.c

todofinder: $(SRC)
	$(CC) $(CFLAGS) -o todofinder $(SRC) $(LIBS)

exp: $(SRCEXP)
	$(CC) $(CFLAGS) -o exp $(SRCEXP) $(LIBS)
