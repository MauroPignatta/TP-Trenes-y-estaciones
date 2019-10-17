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

    char * mensaje = (char*) malloc(sizeMsj);
    char * token = malloc(sizeMsj);
    if (!mensaje || !token)
    {
        puts("Error al asignar memoria.");
        exit(EXIT_FAILURE);
    }
    recv(client, token, sizeMsj, 0);
    sprintf(mensaje,"Bienvenido a la estacion %s", token);

    /* Flag para que un tren no pueda registrarse 2 veces*/
    int yaRegistrado = 1;

    //Aca empieza a correr ncurses
    ST_APP_WINDOW pWin;
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    
    printWindowTitle(pWin.pLogFrame, "### Tren ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

    printMessage(&pWin, mensaje, WHITE);
    wrefresh(pWin.pLogWindow);

    wmove(pWin.pCmdWindow, 0,0);


    while(1)
    {
        memset(mensaje, '\0', sizeMsj);
        wgetnstr(pWin.pCmdWindow, mensaje, sizeMsj + 1);

        if(! strcmp(mensaje, "help"))
        {
            clearLogWindow(pWin.pLogWindow);
            printHelp(&pWin);
        }

        else if(!strcmp(mensaje, "registrarse"))
        {
            clearLogWindow(pWin.pLogWindow);
            if (!yaRegistrado)
            {
                printMessage(&pWin, "Ya te has registrado.", WHITE);
            }
            else
            {
                registrarse(mensaje,tren);
                send (client, mensaje, strlen(mensaje), 0);

                recv(client, mensaje, sizeMsj, 0);

                char * token = malloc(sizeMsj);
                if (!token)
                {
                    printMessage(&pWin, "No se pudo asignar memoria.", WHITE);
                    exit(EXIT_FAILURE);
                }

                token = strtok(mensaje,";");

                if (*token == '1')
                {
                    yaRegistrado = 0;
                    token = strtok(NULL,";");
                    strcpy(tren.estOrigen, token);
                }
                token = strtok(NULL,";");
                printMessage(&pWin, token, WHITE);
                free(token);
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
            trencitoViajando(pWin.pLogWindow);
            printMessage(&pWin, "", WHITE);
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

        /*
        switch(mensaje[0]){
            case '3':
				printf("A dónde desea viajar?\n");
				gets(tren.estDestino);
				char solicitud[sizeMsj]="3;";
	    		send(client,solicitud,strlen(solicitud),0);
	    		recv(client,solicitud,sizeMsj,0);
				tren.tiempoRestante=atoi(solicitud);
				partir(&tren);//partir
                break;
        }
        */
    }


    free(mensaje);
    unInitUserInterface(&pWin);
    return (EXIT_SUCCESS);
}

