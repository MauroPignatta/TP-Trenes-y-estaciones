all: estacion tren
	
SRCTREN = src/tren_commands.c src/tren_interface.c src/conexion.c src/funcTrenes.c

SRCEST = src/est_commands.c src/est_interface.c src/conexion.c src/funcEstaciones.c

HEADERS = $(wildcard lib/*.h)

LIB = -Ilib

CFLAG = -lncurses

CC = gcc

estacion: estacion.c $(SRCFILES) $(HEADERS)
	$(CC) $(LIB) estacion.c $(SRCEST) -o bin/estacion $(CFLAG)

tren: tren.c $(SRCFILES) $(HEADERS)
	$(CC) $(LIB) tren.c $(SRCTREN) -o bin/tren $(CFLAG)

clean: 
	rm -f bin/*