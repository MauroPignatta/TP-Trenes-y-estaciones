#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"
#include "lib/est_interface.h"
#include "lib/est_commands.h"

/* Variables globales que usa el hilo */
ST_APP_WINDOW pWin;
ESTACION estaciones[MAX_ESTACION];
int miPos;
int cantTrenes = 0;
int cantEstaciones = 0;
int trenesRegistrados = 0;
int server;

void getInput();

int main(int argc, char** argv) 
{ 
    system("clear");
    
    if (argc != 2)
    {
    	printf("Ingrese el nombre del archivo de conf. como parametro\n");
        exit(3);
    } 

    //Obtengo los datos de las estaciones. 
    char *nomArchivo = FormatearNombreArchivo(argv[1]);
    miPos = ObtenerDatosMiEstacion(nomArchivo, estaciones); 
    free(nomArchivo);
    ObtenerOtrasEstaciones(estaciones, miPos);
	
    char mensaje[sizeMsj];
    
    //Devuelve el socket ya configurado
    server = CrearSocketServer("../config/Red1.txt");
    
    int maxfd = server + MaxClientes;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    //Variables con los datos para la conexion
    int clientTrenes[MAX_TREN];
    int clientEst[MAX_ESTACION];
    int clientActual = -1;

    memset(clientTrenes , -1, sizeof(clientTrenes));
    memset(clientEst , -1, sizeof(clientEst));

    int regCorrecto;

    //Aca empieza a correr ncurses
    initUserInterface(&pWin);
    drawUserInterface(&pWin);

    // Uso el mensaje para ponerle el titulo con el nombre de la estacion
    sprintf(mensaje, " Estacion %s " , estaciones[miPos].nombre);
    
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
        int seQuienHablo = 0;
        int nready = select(maxfd, &copy, NULL, NULL, NULL); 
        
	   // si es un cliente nuevo
        if ( FD_ISSET(server, &copy))
        {
            /* acepta al nuevo cliente */
            clientActual = accept(server, 0, 0);
            recv(clientActual, mensaje, sizeof(mensaje), 0);

            if (!strcmp(mensaje, "tren"))
            {
            	printRegistro(&pWin,"Se conecto un nuevo tren", WHITE);
            	sprintf(mensaje,"Bienvenido a la estacion %s", estaciones[miPos].nombre);
            	clientTrenes[cantTrenes] = clientActual;
            	send(clientTrenes[cantTrenes], mensaje, strlen(mensaje), 0);
            	cantTrenes++;
            }
            else
            {
            	printRegistro(&pWin,"Se conecto un nuevo tren", WHITE);
            	clientEst[cantEstaciones] = clientActual;
            	cantEstaciones ++;
            }
            /* lo agrega al fd */
            FD_SET(clientActual ,&master);
            
        }
        else // si ya lo conoce
        {
        	seQuienHablo = 0;
            for(int i = 0; i < cantTrenes && !seQuienHablo; i ++) //Busco quien me hablo en los trenes
            {
                if (FD_ISSET(clientTrenes[i], &copy))
                {
                	seQuienHablo = 1;
                    memset(mensaje,'\0',sizeMsj);
                    // Recibo el mensaje
                    int bytes = recv(clientTrenes[i], mensaje, sizeof(mensaje), 0 );
                    
		            //  Para saber si el cliente se desconecto 
                    if (bytes <= 0)
                    {
                        printRegistro(&pWin,"Se desconecto un tren", WHITE);
                        estaciones[miPos].tren[i].ID = 0;
                        trenesRegistrados --;
                        FD_CLR(clientTrenes[i],&master);
                    }
                    else
                    {
                        char opcion = mensaje[0];
                        switch (opcion)
                        {
                            case '1':
                                /*Registro al tren*/
                                regCorrecto = registrarTren(&estaciones[miPos], mensaje);
                                sprintf(mensaje,"1;%s;Te has registrado correctamente", estaciones[miPos].nombre);

                                /*Comprueba que el tren se haya registrado*/
                                if (!regCorrecto)
                                {
                                    printRegistro(&pWin,"No se pudo registrar a un\ntren: Ya registrado.", WHITE);
                                    strcpy(mensaje,"2;No te has podido registrar: Ya existe un tren con el mismo ID.");
                                }
                                else{
                                    printRegistro(&pWin,"Un tren se ha registrado", WHITE);
                                    trenesRegistrados++;
                                }
                                /*Envio una respuesta al tren*/
                                send(clientTrenes[i], mensaje, sizeMsj, 0);
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
                                send(clientTrenes[i], mensaje, strlen(mensaje), 0);
                                */
                                break;
                                
                            default:
                                printf("Nunca deberia salir por el default\n");
                                printf("Si ves este mensaje, hay algo que esta mal\n");
                        }
                    }
                }
            }
            for(int i = 0; i < cantEstaciones && !seQuienHablo; i ++) //Busco quien me hablo entre las estaciones
            {
            	if (FD_ISSET(clientEst[i], &copy))
                {

                }	
            }
        }
    }
    return (EXIT_SUCCESS);
}

/* Esta funcion la hice para probar si andaba bien la funcion
ObtenerOtrasEstaciones, la dejo porque puede llegar a servir */
void mostrarEstaciones(ST_APP_WINDOW *pWin, ESTACION est[])
{
	char aux[sizeMsj];
	for(int i = 0; i < MAX_ESTACION; i++)
	{
		clearWindow(pWin->pLogWindow);
		sprintf(aux,"ID:%d\nnom:%sDis:%d",est[i].ID, est[i].nombre, est[i].distancia);
		printLog(pWin, aux, WHITE);
		sleep(2);
	}
	printLog(pWin, "", WHITE);
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
      		clearWindow(pWin.pLogWindow);
      		if(trenesRegistrados > 0)
      		{
      			printEstadoTrenes(&pWin, estaciones[miPos].tren, trenesRegistrados);
      			printLog(&pWin,"", WHITE);
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

      	else if (!strcmp(comandos, "mostrar"))
      	{
      		mostrarEstaciones(&pWin, estaciones);
      	}

      	else if (!strcmp(comandos, "buscar"))
      	{
      		int cont = 0;
			char IP[16];
			int Puerto;
      		clearWindow(pWin.pLogWindow);
			struct sockaddr_in conectarEstaciones;
		    conectarEstaciones.sin_family = AF_INET;
      		for(int i  = 0; i < MAX_ESTACION; i ++)
      		{
      			if (i != miPos)
      			{
      				sprintf(comandos, "../config/Red%d.txt", i + 1);
      				obtenerDatosRed(IP, &Puerto, comandos);
      				conectarEstaciones.sin_addr.s_addr = inet_addr(IP);
    				conectarEstaciones.sin_port = htons(Puerto);
    				if (connect(server, (void *) &conectarEstaciones, sizeof(conectarEstaciones)) == 0)
    				{
    					printLog(&pWin, "Estacion encontrada", WHITE);
    					cont ++;
    				}
      			}
      		}
      		if (cont == 0)
      			printLog(&pWin, "No se encontraron Estaciones", WHITE);
      	}

      	else
      	{
      		clearWindow(pWin.pLogWindow);
      		strcat(comandos, " no es un comando valido.");
      		printLog(&pWin, comandos, WHITE);
      	}
    }
}
