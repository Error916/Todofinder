CFLAGS=-Wall -Wextra -std=c11 -pedantic -O3
LIBS=
SRC= src/main.c src/recdir.c src/todotype.c

todofinder: $(SRC)
	$(CC) $(CFLAGS) -o todofinder $(SRC) $(LIBS)
