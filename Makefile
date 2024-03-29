CFLAGS= -g -Wall -Wextra -std=c11 -pedantic -O3 -march=native
LIBS= -lpthread
SRC= src/main.c src/recdir.c src/todotype.c
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

todofinder: $(SRC)
	$(CC) $(CFLAGS) -o todofinder $(SRC) $(LIBS)

install: todofinder
	sudo install -d $(DESTDIR)$(PREFIX)/bin/ || su -c="install -d $(DESTDIR)$(PREFIX)/bin/"
	sudo install -m 644 todofinder $(DESTDIR)$(PREFIX)/bin/ || su -c="install -m 644 todofinder $(DESTDIR)$(PREFIX)/bin/"
	sudo chmod +x $(DESTDIR)$(PREFIX)/bin/todofinder

clean:
	rm todofinder
