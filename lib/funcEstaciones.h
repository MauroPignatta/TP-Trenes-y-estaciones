#include "funcTrenes.h"
#define max_cad 50

typedef struct 
{
	char nombre[30];
	int distancia;	
	TREN tren[10];	
}ESTACION;

void registrarTren(TREN tren);

void enviarTren();

void finalizarTren();

void estadoEstacion ();