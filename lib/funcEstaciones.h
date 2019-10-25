#ifndef FUNCESTACIONES_H
#define FUNCESTACIONES_H

#define MAX_TREN 10
#define MAX_ESTACION 5
#define esEstacion(x) x == '2'

#include "funcTrenes.h"

typedef struct 
{
	int ID;
	char nombre[max_nombre_est];
	int distancia;
	int online;
	TREN tren[MAX_TREN];
}ESTACION;

ESTACION estaciones[MAX_ESTACION];
int miPos;

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
int buscarTrenes( TREN trenes[] ,int posTrenes[]);

/*Devuelve la posicion en la que se encuentra el tren
en el vector de trenes de la estacion, o -1 si no se
encuentra*/
int BuscarTrenPorID(ESTACION estacion, int idTren);

/* Funcion para el hilo que se encarga de la conexion servidor-cliente */
void ConexionServer();

#endif	// FUNCESTACIONES_H
