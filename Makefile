all: Estacion Tren
	
srcfiles = src/funcEstaciones.c src/funcTrenes.c src/Conexion.c 

Estacion: estacion.c
	gcc  -Ilib -o bin/estacion estacion.c $(srcfiles)

Tren: tren.c
	gcc -Ilib -o bin/tren tren.c $(srcfiles)