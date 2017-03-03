CC=gcc
CFLAGS=-W -Wall -std=c99 -pedantic -O3
LDFLAGS=
EXEC= Futoshiki

all: $(EXEC)

Futoshiki: main.o CSP.o resolution.o
	$(CC) -o Futoshiki main.o CSP.o resolution.o$(LDFLAGS)
	
main.o: main.c CSP.h
	$(CC) -o main.o -c main.c $(CFLAGS)

CSP.o: CSP.c CSP.h
	$(CC) -o CSP.o -c CSP.c $(CFLAGS)

resolution.o: CSP.h resolution.c resolution.h
	$(CC) -o resolution.o -c resolution.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

