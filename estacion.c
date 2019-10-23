#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"
#include "lib/est_interface.h"
#include "lib/est_commands.h"

/* Variables globales que usan los hilos */
ST_APP_WINDOW pWin;
ESTACION estaciones[MAX_ESTACION];
int miPos;

/* Funcion para el hilo que se encarga de la interfaz grafica */
void InterfazGrafica();

/* Funcion para el hilo que se encarga de la conexion servidor-cliente */
void ConexionServer();

int main(int argc, char** argv) 
{ 
    if (argc != 2)
    {
    	printf("\nuso: ./estacion <Nombre archivo Conf Estacion>\n");
        exit(3);
    } 

    system("clear");

    //Obtengo los datos de las estaciones. 
    char *nomArchivoEst = FormatearNombreArchivo(argv[1]);
    miPos = ObtenerDatosMiEstacion(nomArchivoEst, estaciones); 
    ObtenerOtrasEstaciones(estaciones, miPos);
    free(nomArchivoEst);

    /* Hilo para la interfaz grafica */
    pthread_t Interfaz;
    wmove(pWin.pCmdWindow, 0,0);
    pthread_create(&Interfaz, NULL,(void*) InterfazGrafica ,NULL);

    /* Hilo para la conexion */
    pthread_t Conexion;
    wmove(pWin.pCmdWindow, 0,0);
    pthread_create(&Conexion, NULL,(void*) ConexionServer ,NULL);

    /* Espero a que los hilos "terminen" cosa que no va a pasar nunca porque son infinitos,
    pero si no pongo esto el main sigue viaje, llega al return y termina la ejecucion */ 
    pthread_join(Conexion, NULL);
    pthread_join(Interfaz, NULL);
 
    return (EXIT_SUCCESS);
}

void InterfazGrafica()
{
	 //Aca empieza a correr ncurses
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    mvwprintw(pWin.pLogWindow, getmaxy(pWin.pLogWindow) -1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWin.pLogWindow);

	char mensaje[sizeMsj];

    sprintf(mensaje, " Estacion %s " , estaciones[miPos].nombre);
    
    printWindowTitle(pWin.pAppFrame, mensaje);
    printWindowTitle(pWin.pLogFrame, "### Log ###");
    printWindowTitle(pWin.pRegFrame, "### Registro ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

	char comandos[20];
	int serverEst[MAX_ESTACION];
    
    while(1)
    {
        wgetnstr(pWin.pCmdWindow, comandos, 20);
        clearCmdWindow(pWin.pCmdWindow);

      	if (!strcmp(comandos, "help"))
      	{
      		clearWindow(pWin.pLogWindow);
      		printHelp(&pWin);
      	}

      	else if (!strcmp(comandos, "estado tren"))
      	{
      		if (printEstadoTrenes(&pWin, estaciones[miPos].tren) == 0)
      		{
      			clearWindow(pWin.pLogWindow);
      			printLog(&pWin,"No hay trenes registrados en la estacion.", WHITE);
      		}
      	}

      	else if (!strcmp(comandos, "estado est"))
      	{
      		printEstadoEstaciones(&pWin, estaciones);
      	}

      	else if (!strcmp(comandos, "buscar est"))
      	{
      		clearWindow(pWin.pLogWindow);
      		printLog(&pWin, "Buscando estaciones...", WHITE);
      		int cont = 0;
      		for(int i  = 0; i < MAX_ESTACION; i ++)
      		{
      			if (i != miPos)
      			{
      				sprintf(mensaje, "../config/Red%d.conf", i + 1);
      				if (serverEst[i] = conectarEstacion(mensaje))
      				{
      					cont ++ ;
      					send(serverEst[i], "2", sizeMsj, 0);
      					estaciones[i].online = 1;
      				}
      			}
      		}
      		clearWindow(pWin.pLogWindow);
      		if (cont == 0)
      			printLog(&pWin, "No se encontraron estaciones", WHITE);
      		else
      			printLog(&pWin, "Se encontraron estaciones", WHITE);
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

      	else if (!strcmp(comandos, "exit"))
      	{
      		clearWindow(pWin.pLogWindow);
      		printLog(&pWin, "Saliendo...", WHITE);
  			sprintf(mensaje, "2;5;%d", miPos);
      		for(int i = 0; i < MAX_ESTACION; i++)
      		{
      			if (i != miPos)
					send(serverEst[i], mensaje, sizeMsj, 0);
	  		}
	  		unInitUserInterface(&pWin);
      		exit(EXIT_SUCCESS);
      	}

      	else
      	{
      		clearWindow(pWin.pLogWindow);
      		strcat(comandos, " no es un comando valido.");
      		printLog(&pWin, comandos, WHITE);
      	}
    }
}

void ConexionServer()
{
    char mensaje[sizeMsj];
    sprintf(mensaje,"../config/Red%d.conf", miPos + 1);

    int server = CrearSocketServer(mensaje);  //Devuelve el socket ya configurado
    
    int maxfd = server + MaxClientes + 1;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    //Variables con los datos para la conexion
    int client[MaxClientes];
    memset(client , -1, sizeof(client));
	int n = 0;

    while (1)
    {
        fd_set copy = master;
        int seQuienHablo = 0;
        int bytesRecibidos = 0;
        int TrenID = 0;
        int posEst = -1;
    	int posTren = 0;
    	int regCorrecto = 0;
    	char opcion = ' '; 
    	memset(mensaje,'\0',sizeMsj);

        select(maxfd, &copy, NULL, NULL, NULL); 
        
	   // si es un cliente nuevo
        if ( FD_ISSET(server, &copy))
        {
            /* acepta al nuevo cliente */
            client[n] = accept(server, 0, 0);
            recv(client[n], mensaje, sizeMsj, 0);

            if ( esTren( mensaje[0]) )
            {
            	printRegistro(&pWin,"Se conecto un nuevo tren", WHITE);
            	sprintf(mensaje,"Bienvenido a la estacion %s", estaciones[miPos].nombre);
            	send(client[n], mensaje, strlen(mensaje), 0);
            }
            else if ( esEstacion( mensaje[0]) )
            {
            	printRegistro(&pWin,"Se conecto una Estacion", WHITE);
            }
            /* lo agrega al fd */
            FD_SET(client[n] ,&master);
            n++;
        }
        else // si ya lo conoce
        {
        	seQuienHablo = 0;
            for(int i = 0; i < MaxClientes && !seQuienHablo; i ++) //Busco quien me hablo
            {
                if (FD_ISSET(client[i], &copy))
                {
                	seQuienHablo = 1;
                    // Recibo el mensaje
                    bytesRecibidos = recv(client[i], mensaje, sizeMsj, 0 ); // recibo el mensaje que me dice si es tren o estacion.
                    
		            //  Para saber si el cliente se desconecto 
                    if (bytesRecibidos <= 0) // si el cliente se desconecta haciendo ctrl + c, no vamos a poder saber quien era
                    {
                        printRegistro(&pWin,"Se desconecto algo", WHITE);
                        FD_CLR(client[i], &master);
                    }

                    else
					{
	                    if ( esTren ( mensaje[0] )) //compruebo si me hablo un tren
	                    {
	                        opcion = mensaje[2];
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
	                                send(clientTrenes[i], mensaje, strlen(mensaje), 0);
	                                */
	                                break;
	                            case '5':	//Agrego este para cuando un tren quiere desconectarse
	                            	
	                            	sscanf(mensaje, "1;5;%d", &TrenID);
	                            	posTren = BuscarTrenPorID(estaciones[miPos], TrenID);

	                            	if (posTren != -1)
	                            		estaciones[miPos].tren[posTren].ID = 0;
	                            	
			                        printRegistro(&pWin,"Se desconecto un tren", WHITE);
			                        FD_CLR(client[i], &master);

	                            	break;
	                        }
	                    }
		                else if ( esEstacion( mensaje[0] )) // si el mensaje recibido es de una estacion
		                {
		                	opcion = mensaje[2];

		                	switch (opcion)
	                        {

	                        	case '5': //exit
	                        		sscanf(mensaje, "2;5;%d", &posEst);
	                            	estaciones[posEst].online = 0;

	                            	//bytesRecibidos = recv(client[i], mensaje, sizeMsj, 0);

	                            	//if (bytesRecibidos <= 0)
				                    //{
				                        printRegistro(&pWin,"Se desconecto una estacion", WHITE);
				                        FD_CLR(client[i], &master);
				                    //}
	                        		break;
                        	}
		                }
		            }
                }
            }
        }
    }
}