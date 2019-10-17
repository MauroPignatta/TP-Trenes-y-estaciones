#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"
#include "lib/est_interface.h"
#include "lib/est_commands.h"

ST_APP_WINDOW pWin;
char comandos[20];

void getInput()
{
    while(1)
    {
        wgetnstr(pWin.pCmdWindow, comandos, 20);
        clearCmdWindow(pWin.pCmdWindow);
    }
}

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
    ESTACION estacion = ObtenerDatosEstacion(nomArchivo);
    
    char mensaje[sizeMsj];
    
    //Devuelve el socket ya configurado
    int server = CrearSocketServer();
    
    int maxfd = server + MaxClientes;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    int client[MaxClientes]; 
    memset(client,-1,sizeof(client));
    int n = 0;
    int regCorrecto;

    //Aca empieza a correr ncurses
    //ST_APP_WINDOW pWin;
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    
    printWindowTitle(pWin.pLogFrame, "### Estacion ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

    mvwprintw(pWin.pLogWindow, getmaxy(pWin.pLogWindow) -1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWin.pLogWindow);

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
            strcpy(mensaje, estacion.nombre);
            printMessage(&pWin,"Un nuevo tren se ha conectado", WHITE);

            /* acepta al nuevo tren y le envia el mensaje de bienvenida*/
            client[n] = accept(server, 0, 0);
            send(client[n], mensaje, sizeMsj, 0);

            /* lo agrega al fd */
            FD_SET(client[n],&master);
            n++;
        }
        else // si ya lo conoce
        {
            for(int i = 0; i < n; i ++)
            {
                if (FD_ISSET(client[i], &copy))
                {
                    memset(mensaje,'\0',sizeof(mensaje));
                    // Recibo el mensaje
                    int bytes = recv(client[i], mensaje, sizeof(mensaje), 0 );
                    
		            //  Para saber si el cliente se desconecto 
                    if (bytes <= 0)
                    {
                        memset(mensaje,'\0', sizeof(mensaje));
                        printMessage(&pWin,"Se ha ido un tren", WHITE);
                        FD_CLR(client[i],&master);
                    }
                    else
                    {
                        char opcion = mensaje[0];
                        switch (opcion)
                        {
                            case '1':
                                /*Registro al tren*/
                                printMessage(&pWin,"Registrando tren", WHITE);
                                regCorrecto = registrarTren(&estacion, mensaje);
                                sprintf(mensaje,"1;%s;Te has registrado correctamente", estacion.nombre);

                                /*Comprueba que el tren se haya registrado*/
                                if (!regCorrecto)
                                {
                                    printMessage(&pWin,"No se pudo registrar al tren", WHITE);
                                    strcpy(mensaje,"2;No te has podido registrar");
                                }
                                else{
                                    printMessage(&pWin,"Registro de tren correcto", WHITE);
                                }

                                /*Envio una respuesta al tren*/
                                send(client[i], mensaje, strlen(mensaje), 0);
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