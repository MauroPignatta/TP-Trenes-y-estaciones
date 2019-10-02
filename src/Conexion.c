#include "../lib/Conexion.h"
#include <stdio.h>
#include <stdlib.h>

void obtenerDatosRed(char* IP, int *Puerto)
{
    /* Abro el archivo */
    FILE * Red = fopen("../config/configRed.txt","r");
    if (!Red)
    {
        printf("Error al abrir archivo de configuracion de red\n");
        exit(2);
    }
    
    /* Obtengo los datos */
    fscanf(Red, "%s %d", IP, Puerto);
    
    /* Cierro el archivo*/
    fclose(Red);
}

int CrearSocketServer()
{
    char * IP = (char*) malloc(sizeIP);
    int Puerto;
    
    /* Obtiene los datos del archivo de config. y los almacenas en las variables*/
    obtenerDatosRed(IP, &Puerto);
    
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
    
    //comprobar error
    if ( bind(server, (struct sockaddr*) &dirServer,sizeof (dirServer)) != 0)
    {
        perror("Error de conexion");
        return 1;
    }
    
    listen(server, MaxClientes);
    printf("Esperando conexiones entrantes...\n");
    
    return server;
}

int CrearSocketCliente()
{
    char * IP = (char*) malloc(sizeIP);
    int Puerto;
    
    /* Obtiene los datos del archivo de config. y los almacenas en las variables*/
    obtenerDatosRed(IP, &Puerto);
    
    /* Creamos el socket*/
    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    
    /* En esta struct se guardan los datos del servidor*/
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr(IP);
    dirServer.sin_port = htons(Puerto);
    
    /* Conectamos al servidor*/
    if (connect(cliente, (void *)&dirServer, sizeof(dirServer)))
    {
        perror("No se pudo conectar con el servidor");
        exit(2);
    }
    
    return cliente;
}