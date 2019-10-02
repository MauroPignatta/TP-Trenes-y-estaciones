all: Estacion Tren
	
srcfiles = src/funcEstaciones.c src/funcTrenes.c src/Conexion.c 

CFLAG = -Ilib 

Estacion: estacion.c
	gcc  $(CFLAG) -o bin/estacion estacion.c $(srcfiles)

Tren: tren.c
	gcc $(CFLAG) -o bin/tren tren.c $(srcfiles)

clean: 
	rm -f bin/*.exe