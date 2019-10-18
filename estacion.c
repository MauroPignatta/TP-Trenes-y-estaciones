#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"
#include "lib/est_interface.h"
#include "lib/est_commands.h"

/* Variables globales que usa el hilo */
ST_APP_WINDOW pWin;
ESTACION estacion;
int cantTrenes = 0;

void getInput();

int main(int argc, char** argv) 
{ 
    system("clear");
    
    if (argc != 2)
    {
    	printf("Ingrese el nombre del archivo de conf. como parametro\n");
        exit(3);
    } 

    FormatearNombreArchivo(argv[1]);
    char nomArchivo[20] = "../config/";
    strcat(nomArchivo, argv[1]);

    //Declaro la estacion y le asigno los datos segun el archivo de conf.
    estacion = ObtenerDatosEstacion(nomArchivo);
    
    char mensaje[sizeMsj];
    
    //Devuelve el socket ya configurado
    int server = CrearSocketServer();
    
    int maxfd = server + MaxClientes;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    //vector con los datos para la conexion
    int client[MaxClientes]; 
    memset(client , -1, sizeof(client));
    int regCorrecto;

    //Aca empieza a correr ncurses
    initUserInterface(&pWin);
    drawUserInterface(&pWin);

    // Uso el mensaje para ponerle el titulo con el nombre de la estacion
    sprintf(mensaje, " Estacion %s " , estacion.nombre);
    
    printWindowTitle(pWin.pAppFrame, mensaje);
    printWindowTitle(pWin.pLogFrame, "### Log ###");
    printWindowTitle(pWin.pRegFrame, "### Registro ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

    mvwprintw(pWin.pLogWindow, getmaxy(pWin.pLogWindow) -1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWin.pLogWindow);

    /* Hilo para recibir los comandos de la estacion */
    pthread_t input;
    wmove(pWin.pCmdWindow, 0,0);
    pthread_create(&input, NULL,(void*) getInput ,NULL);
    
    while (1)
    {
        fd_set copy = master;
        int nready = select(maxfd, &copy, NULL, NULL, NULL); 
        
	   // si es un cliente nuevo
        if ( FD_ISSET(server, &copy))
        {
            /* Falta preparar / mejorar el mensaje de bienvenida */
            sprintf(mensaje,"Bienvenido a la estacion %s", estacion.nombre);
            printRegistro(&pWin,"Se conecto un nuevo tren", WHITE);

            /* acepta al nuevo tren y le envia el mensaje de bienvenida*/
            client[n] = accept(server, 0, 0);
            send(client[n], mensaje, strlen(mensaje), 0);

            /* lo agrega al fd */
            FD_SET(client[n],&master);
            cantTrenes++;
        }
        else // si ya lo conoce
        {
            for(int i = 0; i < cantTrenes; i ++)
            {
                if (FD_ISSET(client[i], &copy))
                {
                    memset(mensaje,'\0',sizeMsj);
                    // Recibo el mensaje
                    int bytes = recv(client[i], mensaje, sizeof(mensaje), 0 );
                    
		            //  Para saber si el cliente se desconecto 
                    if (bytes <= 0)
                    {
                        printRegistro(&pWin,"Se desconecto un tren", WHITE);
                        FD_CLR(client[i],&master);
                    }
                    else
                    {
                        char opcion = mensaje[0];
                        switch (opcion)
                        {
                            case '1':
                                /*Registro al tren*/
                                regCorrecto = registrarTren(&estacion, mensaje);
                                sprintf(mensaje,"1;%s;Te has registrado correctamente", estacion.nombre);

                                /*Comprueba que el tren se haya registrado*/
                                if (!regCorrecto)
                                {
                                    printRegistro(&pWin,"No se pudo registrar a un\ntren: Ya registrado.", WHITE);
                                    strcpy(mensaje,"2;No te has podido registrar: Ya existe un tren con el mismo ID.");
                                }
                                else{
                                    printRegistro(&pWin,"Un tren se ha registrado", WHITE);
                                }
                                /*Envio una respuesta al tren*/
                                send(client[i], mensaje, sizeMsj, 0);
                                break;
                               
                            case '2':
                                // solicitar anden
                                break;
                                
                            case '3':
                                // partir
                                break;
                                
                            case '4':
                                /* Esto va a haber que cambiarlo, no borrar por ahora.

                                estadoDelTren(estacion ,mensaje);
                                puts("Estado enviado");
                                send(client[i], mensaje, strlen(mensaje), 0);
                                */
                                break;
                                
                            default:
                                printf("Nunca deberia salir por el default\n");
                                printf("Si ves este mensaje, hay algo que esta mal\n");
                        }
                    }
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}

void getInput()
{
	char comandos[46];
    while(1)
    {
        wgetnstr(pWin.pCmdWindow, comandos, 20);
        clearCmdWindow(pWin.pCmdWindow);

      	if (!strcmp(comandos, "help"))
      	{
      		clearWindow(pWin.pLogWindow);
      		printHelp(&pWin);
      	}

      	else if (!strcmp(comandos, "estado"))
      	{
      		if(cantTrenes > 0)
      		{
      			clearWindow(pWin.pLogWindow);
      			printEstadoTrenes(&pWin, estacion.tren[], cantTrenes);
      		}
      		else
      		{
      			printLog(&pWin,"No hay trenes en la estacion.", WHITE);
      		}
      	}

      	else if (!strcmp(comandos, "clearlog"))
      	{
      		clearWindow(pWin.pLogWindow);
      		printLog(&pWin,"", WHITE);
      	}

      	else if (!strcmp(comandos, "clearreg"))
      	{
      		clearWindow(pWin.pRegWindow);
      	}

      	else
      	{
      		clearWindow(pWin.pLogWindow);
      		strcat(comandos, " no es un comando valido.");
      		printLog(&pWin, comandos, WHITE);
      	}
    }
}