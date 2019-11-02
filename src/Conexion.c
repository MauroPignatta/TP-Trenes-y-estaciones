#include "../lib/Conexion.h"
#include "../lib/est_interface.h"
#include <stdio.h>
#include <stdlib.h>

void obtenerConfRed(char * nombreEstacion , char * archConfigRed)
{
    strcpy(archConfigRed, "../config/red/");
    
    if(!strcmp(nombreEstacion, "Retiro"))
        strcat(archConfigRed,"Red1.conf");
    else if(!strcmp(nombreEstacion, "Munro"))
        strcat(archConfigRed,"Red2.conf");
    else if(!strcmp(nombreEstacion, "Carapachay"))
        strcat(archConfigRed,"Red3.conf"); 
    else if(!strcmp(nombreEstacion, "Boulogne"))
        strcat(archConfigRed,"Red4.conf");
    else if(!strcmp(nombreEstacion, "Tokyo"))
        strcat(archConfigRed,"Red5.conf");
}

char * FormatearNombre(char * Palabra)
{	
    int primerLetra = 1; 
    char * aux = Palabra;
    while (*Palabra)
	{
		if ( primerLetra )
		{
			*Palabra = toupper(*Palabra);
		}
		else 
		{
			*Palabra = tolower(*Palabra);
		}
		primerLetra = 0;
		Palabra++;
	}
    return aux;
}


void obtenerDatosRed(char* IP, int *Puerto, char * confRed)
{
    /* Abro el archivo */
    FILE * Red = fopen(confRed,"r");
    if (!Red)
    {
        printf("Error al abrir archivo de configuracion de red\n");
        exit(2);
    }
    
    /* Obtengo los datos */
    fscanf(Red, "IP: %s\n", IP);

    fscanf(Red, "Puerto: %d", Puerto);
    
    /* Cierro el archivo*/
    fclose(Red);
}

int CrearSocketServer(char * confRed)
{
    char * IP = (char*) malloc(sizeIP);
    int Puerto;
    
    /* Obtiene los datos del archivo de config. y los almacenas en las variables*/
    obtenerDatosRed(IP, &Puerto, confRed);
    
    //crea el socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    
    //libera el socket al cerrar la aplicacion
    int activado = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
    
    //Estructura que contiene datos del servidor 
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr(IP);
    dirServer.sin_port = htons(Puerto);
    free(IP);
    
    //comprobar error
    if ( bind(server, (struct sockaddr*) &dirServer,sizeof (dirServer)) != 0)
    {
        perror("Error de conexion");
        exit(2);
    }
    
    listen(server, MaxClientes + 1);
    
    return server;
}

int CrearSocketCliente(char * confRed)
{
    char * IP = (char*) malloc(sizeIP);
    int Puerto;
    
    /* Obtiene los datos del archivo de config. y los almacenas en las variables*/
    obtenerDatosRed(IP, &Puerto, confRed);
    
    /* Creamos el socket*/
    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    
    /* En esta struct se guardan los datos del servidor*/
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr(IP);
    dirServer.sin_port = htons(Puerto);
    free(IP);
    
    /* Conectamos al servidor*/
    if (connect(cliente, (void *)&dirServer, sizeof(dirServer)) != 0)
    {
        perror("No se pudo conectar con el servidor");
        exit(2);
    }
    
    return cliente;
}

int conectarEstacion(char * confRed)
{
    char * IP = (char*) malloc(sizeIP);
    int Puerto;
    
    /* Obtiene los datos del archivo de config. y los almacenas en las variables*/
    obtenerDatosRed(IP, &Puerto, confRed);
    
    /* Creamos el socket*/
    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    
    /* En esta struct se guardan los datos del servidor*/
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr(IP);
    dirServer.sin_port = htons(Puerto);
    free(IP);
    
    /* Conectamos al servidor*/
    if (connect(cliente, (void *) &dirServer, sizeof(dirServer)) != 0)
    {
        return 0;
    }
    
    return cliente;
}
