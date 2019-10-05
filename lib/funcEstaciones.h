#ifndef FUNCESTACIONES_H
#define FUNCESTACIONES_H

#include "funcTrenes.h"
#define MAX_TREN 10

typedef struct 
{
	char nombre[30];
	int distancia;	
	TREN tren[MAX_TREN];	
}ESTACION;

ESTACION ObtenerDatosEstacion();

/*Copia los datos del tren en la estacion
 *en caso que haya lugar disponible.
 *Devuelve 1 si el tren se registro correctamente y
 * 0 en caso de que no se haya registrado.*/
int registrarTren(ESTACION *estacion, char * mensaje);

void enviarTren();

void finalizarTren();

void estadoDelTren (ESTACION estacion, char * mensaje);
#endif	// FUNCESTACIONES_H

