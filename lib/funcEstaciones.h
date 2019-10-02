#ifndef FUNCESTACIONES_H
#define FUNCESTACIONES_H

#include "funcTrenes.h"
#define max_cad 50

typedef struct 
{
	char nombre[30];
	int distancia;	
	TREN tren[10];	
}ESTACION;

ESTACION ObtenerDatosEstacion();

void registrarTren();

void enviarTren();

void finalizarTren();

void estadoEstacion ();
#endif	// FUNCESTACIONES_H

