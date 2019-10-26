#ifndef CONEXION_H
#define CONEXION_H

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

char * FormatearNombreArchivo( char * Palabra);

void obtenerDatosRed(char* IP, int *Puerto, char * confRed);

int CrearSocketServer(char * confRed);

int CrearSocketCliente(char * confRed);

int conectarEstacion(char * confRed);

void ConexionServer();
/* Funcion para el hilo que se encarga de la conexion servidor-cliente */

#endif	// CONEXION_H

