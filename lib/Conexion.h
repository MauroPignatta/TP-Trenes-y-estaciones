#ifndef CONEXION_H
#define CONEXION_H

/* 
* Integrantes:
* Alejandro Ugobono, Ezequiel Pirrota, Facundo Palacios,
* Gonzalo Fernandez, Mauro Pignata, Mariano Wiñar.
*/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>

#define sizeIP sizeof(char) * 16
#define sizePORT sizeof(char) * 6
#define sizeMsj 1024
#define MaxClientes 15

/*
* Funcion que organiza la palabra ingresada
* @param * palabra es la palabra ingresada en el comando
* return el nombre formateado primera en Mayuscula 
* y las demas en minusculas
*/
char * FormatearNombre( char * Palabra);

/*
* Funcion que obtiene IP y Puerto de la red de servicios.
* @param char * IP es la direccion de IP que identifica la interfaz de red
* @param char * Puerto es el puerto de comunicacion
* @param char * confRed el archivo de configuracion de red
* return * red puntero al archivo de configuracion de red
*/
void obtenerDatosRed(char* IP, int *Puerto, char * confRed);

/*
* Funcion que crea el Socket de comunicacion tipo Servidor
* @param * confRed es el puntero del archivo de configuracion del Servidor
* return un int server formateado para la conexion
*/
int CrearSocketServer(char * confRed);

/*
* Funcion que crea el Socket de comunicacion tipo Cliente
* @param * confRed es el puntero al archivo de configuracion del Cliente
* return un int cliente formateado para la conexion.
*/
int CrearSocketCliente(char * confRed);

/*
* Funcion que crea la Conexion de la Estacion
* @param * confRed es el puntero del archivo de configuracion del Servidor
* return int cliente formateado para la conexion
*/
int conectarEstacion(char * confRed);

#endif	// CONEXION_H
