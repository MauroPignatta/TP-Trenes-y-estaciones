#ifndef FUNCESTACIONES_H
#define FUNCESTACIONES_H

/** 
 * Integrantes:
 * Alejandro Ugobono, 
 * Ezequiel Pirrota, 
 * Facundo Palacios,
 * Gonzalo Fernandez, 
 * Mauro Pignata, 
 * Mariano Wiñar.
 */

#define MAX_TREN 10
#define MAX_ESTACION 5

#define esEstacion(x) x == '2'
#define estacionConectada(y) y == 1

#include "funcTrenes.h"

/** 
 * Estructura de datos para Estaciones 
 */

typedef struct 
{
	int ID;
	char nombre[max_nombre_est];
	int distancia;
	int online;
	int nCliente;
	TREN tren[MAX_TREN];
}ESTACION;

int serverEst[MAX_ESTACION];
ESTACION estaciones[MAX_ESTACION];
int miPos;

/**
 * Funcion Abre el archivo de configuracion pasado como argumento y lo guarda 
 * en la posicion del vector de estaciones que correspanda,
 * @param * nomArchivo es el nombre del archivo
 * @param ESTACION est es el vector de estaciones
 * @return devuelve la posicion en la que se encuentra tu estacion
 */

int ObtenerDatosMiEstacion(char * nomArchivo, ESTACION est[]);

/**
 * Funcion Abre el resto de los archivos de configuracion, y 
 * obtiene los datos de las demas estaciones
 * @param * ESTACION est es el vector de estaciones
 * @param int miPos guarda la posicion de la estacion en el vector
 */

void ObtenerOtrasEstaciones(ESTACION est[],int miPos);

/**
 * Funcion Copia los datos del tren en la estacion en caso que haya lugar disponible.
 * @param ESTACION * estacion puntero a la estacion
 * @param char * mensaje contiene los datos para registrar el tren
 * @return Devuelve 1 si el tren se registro correctamente y 0 en caso de que no se haya registrado.
 */

int registrarTren(ESTACION *estacion, char * mensaje);

/**
 * Funcion Devuelve un vector con las posiciones del vector de trenes en las que se encuentran
 * @param TREN trenes es el vector de trenes
 * @param int posTrenes indica la posicion de los trenes encontrados
 * @return devuelve la cantidad
 */

int buscarTrenes( TREN trenes[] ,int posTrenes[]);

/**
 * Funcion Busca un Tren en el vector de trenes
 * @param ESTACION estacion variable del tipo ESTACION
 * @param int idTren es el numero de tren a buscar
 * @return Devuelve la posicion en la que se encuentra el tren en el vector de trenes de la estacion, o -1 si no se encuentra
 */

int BuscarTrenPorID(ESTACION estacion, int idTren);

/**
 * Funcion para el hilo que se encarga de la conexion servidor-cliente
 * @param
 */

void ConexionServer();

/**
 * Funcion Busca a la estacion por el nombre
 * @param char * mensaje donde tiene el nombre de la estacion a buscar
 * @return Devuelve la pos si la encuentra o -1 si no la encuentra
 */

int buscarEstacionPorNombre(char * mensaje);

/**
 * Funcion que Copia las estaciones disponibles para viajar
 * @param * mensaje puntero a char donde se van a copiar las estaciones disponibles
 * @return devuelve la cantidad de estaciones que estan disponibles para viajar
 */

int mensajeListadoEstDisp(char * mensaje);

/**
 * Funcion que Copia los trenes que migraron al mensaje* 
 * @param * mensaje puntero a char donde se van a copiar los trenes
 * @return devuelve la cantidad de trenes migrados
 */

int mostrarTrenesMigrados(char * mensaje);

/**
 * Funcion que Pide al usuario que ingrese el tren que quiere que viaje
 * @return posicion del tren elegido en el vector o -1 en caso de que el tren elegido no sea valido 
 */

int elegirTren();

/**
 * Funcion Pide al usuario que ingrese la estacion donde quiere viajar* 
 * @return posicion la estacion elegida o -1 en caso de que no sea valida
 */

int elegirEstDestino();

/**
 * Funcion Calcula el tiempo basandose en la distancia entre una estacion y otra* 
 * @param  posEstacionDestino La posicion en el vector de estacion a la cual se quiere dirigir el tren
 * @return Tiempo de viaje
 */

int calcularTiempoDeViaje(int posEstacionDestino);

/**
 * Funcion Prepara el mensaje para enviar un tren de una estacion a otra
 * @param * mensaje puntero a char copia el mensaje a enviar
 * @param * posTren Para saber que tren hay que enviar
 */

void prepararEnvioTren(char *mensaje , int posTren);


#endif	// FUNCESTACIONES_H