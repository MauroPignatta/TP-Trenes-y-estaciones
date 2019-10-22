#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"
#include "lib/tren_interface.h"
#include "lib/tren_commands.h"

int main(int argc, char** argv) {

    system("clear");

    if(argc!=2){
        printf("Ingrese el nombre del archivo de conf. como parametro\n");
        exit(3);
    }

    FormatearNombreArchivo(argv[1]);
    char nomArchivo[20] = "../config/";
    strcat(nomArchivo, argv[1]);
    TREN tren = inicializarTren(nomArchivo);
    
    /* Devuelve el socket ya configurado */
    int client = CrearSocketCliente();

    char mensaje[sizeMsj];

    /* Flag para que un tren no pueda registrarse 2 veces*/
    int yaRegistrado = 0;

    //Aca empieza a correr ncurses
    ST_APP_WINDOW pWin;
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    
    printWindowTitle(pWin.pAppFrame, " Tren ");
    printWindowTitle(pWin.pLogFrame, "### Log ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");


    recv(client, mensaje, sizeMsj, 0);
    printMessage(&pWin, mensaje, WHITE);
    wmove(pWin.pCmdWindow, 0,0);

    while(1)
    {
        memset(mensaje, '\0', sizeMsj);
        wgetnstr(pWin.pCmdWindow, mensaje, 20);

        if(! strcmp(mensaje, "help"))
        {
           	clearLogWindow(pWin.pLogWindow);
        	printHelp(&pWin);
        }

        else if(!strcmp(mensaje, "registrarse"))
        {
            clearLogWindow(pWin.pLogWindow);
            if (yaRegistrado)
            {
                printMessage(&pWin, "Ya te has registrado.", WHITE);
            }
            else
            {
                registrarse(mensaje, tren);
                send(client, mensaje, strlen(mensaje), 0);
                recv(client, mensaje, sizeMsj, 0);
			
                char * token = NULL;
                token = strtok(mensaje,";");

                if (*token == '1')
                {
                    yaRegistrado = 1;
                    token = strtok(NULL,";");
                    strcpy(tren.estOrigen, token);
                }

                token = strtok(NULL,";");
                printMessage(&pWin, token, WHITE);
            }
        }
        
        else if(!strcmp(mensaje, "anden"))
        {
            //solicitar anden
            clearLogWindow(pWin.pLogWindow);
            printMessage(&pWin, "Todavia no implementado.", WHITE);
        }
        
        else if(!strcmp(mensaje, "partir"))
        {
            clearLogWindow(pWin.pLogWindow);
            printMessage(&pWin,"A donde desea viajar?",WHITE);
			wgetnstr(pWin.pLogWindow, tren.estDestino, 20);
			trencitoViajando(pWin.pLogWindow);
			/*char solicitud[sizeMsj]="3;";
	    	send(client,solicitud,strlen(solicitud),0);
	    	recv(client,solicitud,sizeMsj,0);
			tren.tiempoRestante=atoi(solicitud);*/
			partir(&tren);
            
            printMessage(&pWin, "", WHITE);
            clearLogWindow(pWin.pLogWindow);
            return(EXIT_SUCCESS);
        }

        else  if(!strcmp(mensaje, "estado"))
        {
            clearLogWindow(pWin.pLogWindow);
            armarMensajeEstadoDelTren(tren, mensaje);
            printMessage(&pWin, mensaje, WHITE);
        }

        else
        {
            clearLogWindow(pWin.pLogWindow);
            strncat(mensaje," no es un comando valido.", 26);
            printMessage(&pWin, mensaje, WHITE);
        }

        clearCmdWindow(pWin.pCmdWindow);

    }

    unInitUserInterface(&pWin);
    return (EXIT_SUCCESS);
}

