#ifndef FUNCTRENES_H
#define FUNCTRENES_H

/* 
* Integrantes:
* Alejandro Ugobono, Ezequiel Pirrota, Facundo Palacios,
* Gonzalo Fernandez, Mauro Pignata, Mariano Wiñar.
*/

#define max_nombre_est 30
#define esTren(x) x == '1'
#define noHayEstacionesDisponible(x) x == '0'

/* Estructura de datos para Trenes */

typedef struct{
    int ID;
    int combustible;
    char modelo[10];
    char estOrigen[max_nombre_est];
    char estDestino[max_nombre_est];
    int tiempoRestante;
    int nCliente;
    int registrado;
}TREN;

/*
* Funcion Inicializa el Tren con todas las variables
* @param char * arch puntero a
* @return devuelve la variable tipo Tren con toda la info cargada
*/

TREN inicializarTren(char *arch);

/*
* Funcion Solicita el Tren a la Estacion si tiene Anden libre o no
* @param 
*/

void solicitarPasoAnden();

/*
* Funcion Dibuja el Tren y resta el tiempo 
* @param TREN tren variable tipo tren
*/

void partir(TREN tren);

/*
* Funcion Carga Combustible al tren que lo solicite
* @param int * combustible variable para indicar cantidad
*/

void cargarCombustible(int *combustible);

/*
* Funcion Arma en una variable el mensaje con los datos del tren
* @param TREN tren una variable del tipo TREN
* @param char * mensaje la variable del mensaje a enviar
*/

void armarMensajeRegistrarse(TREN tren,char *mensaje);

/*
* Funcion Arma el mensaje con el estado del tren 
* @param TREN tren una variable del tipo TREN
* @param char * mensaje la variable del mensaje a enviar
*/

void armarMensajeEstadoDelTren(TREN t, char * mensaje);

/*
* Funcion Arma el mensaje para salir del programa
* @param TREN tren una variable del tipo TREN
* @param char * mensaje la variable del mensaje a enviar
*/

void armarMensajeExit(TREN tren, char * mensaje);

/*
* Funcion Arma el mensaje para indicarle a la estacion que quiere partir el tren
* @param TREN tren una variable del tipo TREN
* @param char * mensaje la variable del mensaje a enviar
*/
void armarMensajePartir(TREN tren ,char * mensaje);

#endif	// FUNCTRENES_H

