#ifndef CONEXION_H
#define CONEXION_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>

#define sizeIP sizeof(char) * 16
#define sizePORT sizeof(char) * 6
#define sizeMsj 1024
#define MaxClientes 15

void  FormatearNombreArchivo( char * Palabra);

void obtenerDatosRed(char* IP, int *Puerto);

int CrearSocketServer();

int CrearSocketCliente();

#endif	// CONEXION_H

