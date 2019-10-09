all: Estacion Tren
	
srcfiles = src/funcEstaciones.c src/funcTrenes.c src/Conexion.c 

CFLAG = -Ilib 

CC = gcc

Estacion: estacion.c
	$(CC)  $(CFLAG) -o bin/estacion estacion.c $(srcfiles)

Tren: tren.c
	$(CC) $(CFLAG) -o bin/tren tren.c $(srcfiles)

clean: 
	rm -f bin/*