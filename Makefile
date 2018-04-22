CC=gcc
CFLAGS=-Wall -g

prueba: prueba.o parser.o funciones.o
	$(CC) $(CFLAGS) -o prueba prueba.c parser.c funciones.c

prueba.o: prueba.c parser.c funciones.c
	$(CC) $(CFLAGS) -c prueba.c parser.c funciones.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

funciones.o: funciones.c funciones.h parser.c
	$(CC) $(CFLAGS) -c funciones.c parser.c


.PHONY: clean
clean:
	rm -f prueba *.o
