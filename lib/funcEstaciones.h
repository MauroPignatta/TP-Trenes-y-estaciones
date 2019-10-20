#ifndef FUNCESTACIONES_H
#define FUNCESTACIONES_H

#include "funcTrenes.h"
#define MAX_TREN 10
#define MAX_ESTACION 5

typedef struct 
{
	int ID;
	char nombre[30];
	int distancia;
	TREN tren[MAX_TREN];	
}ESTACION;

/* Abre el archivo de configuracion pasado como argumento y lo guarda 
en la posicion del vector de estaciones que correspanda, 
La funcion devuelve la posicion en la que se encuentra 
tu estacion */
int ObtenerDatosMiEstacion(char * nomArchivo, ESTACION est[]);

/* Abre el resto de los archivos de configuracion, y 
obtiene los datos de las demas estaciones */
void ObtenerOtrasEstaciones(ESTACION est[],int miPos);

/*Copia los datos del tren en la estacion
 *en caso que haya lugar disponible.
 *Devuelve 1 si el tren se registro correctamente y
 * 0 en caso de que no se haya registrado.*/
int registrarTren(ESTACION *estacion, char * mensaje);

void enviarTren();

void finalizarTren();

void estadoDelTren (ESTACION estacion, char * mensaje);

/* Devuelve un vector con las posiciones del vector de trenes
en las que se encuentran */
void buscarTrenes( TREN trenes[] ,int posTrenes[]);



#endif	// FUNCESTACIONES_H
