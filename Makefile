all: Estacion Tren
	


Estacion: estacion.c
	gcc  -Ilib -o bin/estacion estacion.c src/funcEstaciones.c src/funcTrenes.c

Tren: tren.c
	gcc -Ilib -o bin/tren tren.c src/funcTrenes.c