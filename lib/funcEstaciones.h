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

void registrarTren(ESTACION *estacion, char * mensaje);

void enviarTren();

void finalizarTren();

void estadoEstacion ();
#endif	// FUNCESTACIONES_H

