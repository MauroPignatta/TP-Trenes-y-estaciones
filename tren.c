#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"
#include "lib/tren_interface.h"

int main(int argc, char** argv) {

    if(argc != 3){
        printf("\nuso: ./tren <Nombre archivo Conf Tren> <Nombre de Estacion a conectarse>\n");
        exit(3);
    }

    system("clear");

    char nomArchivo[40] = "../config/tren/";
    strcat(nomArchivo, FormatearNombre(argv[1]));
    TREN tren = inicializarTren(nomArchivo);
    
    /* Devuelve el socket ya configurado */
    obtenerConfRed(FormatearNombre(argv[2]) , nomArchivo);
    int client = CrearSocketCliente(nomArchivo);
    send(client, "1", sizeMsj, 0);
    
    char mensaje[sizeMsj];

    /* Flag para que un tren no pueda registrarse 2 veces*/
    int yaRegistrado = 0;

    //Aca empieza a correr ncurses
    ST_APP_WINDOW pWin;
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    
    sprintf(mensaje, " Tren %d ",tren.ID);
    printWindowTitle(pWin.pAppFrame, mensaje);
    printWindowTitle(pWin.pLogFrame, "### Log ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

    recv(client, mensaje, sizeMsj, 0);  // Recibo el mensaje de Bienvenido a la estacion <nombre estacion>
    printMessage(&pWin, mensaje, WHITE);
    wmove(pWin.pCmdWindow, 0,0);

    while(1)
    {
        memset(mensaje, '\0', sizeMsj);
        wgetnstr(pWin.pCmdWindow, mensaje, sizeMsj);

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
                armarMensajeRegistrarse(tren, mensaje);
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
            if (yaRegistrado)
            {
                armarMensajePartir(tren, mensaje);
                send(client, mensaje, sizeMsj, 0);

                recv(client, mensaje, sizeMsj, 0);

                clearLogWindow(pWin.pLogWindow);
                printMessage(&pWin, mensaje, WHITE);

                if (strcmp(mensaje, "No hay estaciones disponibles"))
                {
                    clearLogWindow(pWin.pLogWindow);
                    printMessage(&pWin, mensaje, WHITE);

                    clearCmdWindow(pWin.pCmdWindow);
                    wgetnstr(pWin.pCmdWindow, mensaje, sizeMsj);
                    send(client, mensaje, sizeMsj, 0);

                    recv(client, mensaje, sizeMsj, 0);
                    if (!strcmp(mensaje, "OK"))
                    {
                        recv(client, mensaje, sizeMsj, 0);
                        tren.tiempoRestante = atoi(mensaje);
                        tren.combustible -= restarCombustible(tren.tiempoRestante);
                        DibujarTrenViajando(pWin.pLogWindow, &tren.tiempoRestante);
                        
                        armarMensajeExit(tren, mensaje);
                        send(client, mensaje, strlen(mensaje), 0);
                        unInitUserInterface(&pWin);
                        exit(EXIT_SUCCESS);
                    }
                    else
                    {
                        clearLogWindow(pWin.pLogWindow);
                        printMessage(&pWin, mensaje, WHITE);
                    }
                }
            }
            else 
            {
                clearLogWindow(pWin.pLogWindow);
                printMessage(&pWin, "Es necesario estar registrado en la estacion.", WHITE);
            }
        }
        
        else  if(!strcmp(mensaje, "estado"))
        {
            clearLogWindow(pWin.pLogWindow);
            armarMensajeEstadoDelTren(tren, mensaje);
            printMessage(&pWin, mensaje, WHITE);
        }

        else  if(!strcmp(mensaje, "exit"))
        {
            armarMensajeExit(tren, mensaje);
            send(client, mensaje, strlen(mensaje), 0);
            unInitUserInterface(&pWin);
            exit(EXIT_SUCCESS);
        }

        else
        {
            clearLogWindow(pWin.pLogWindow);
            strncat(mensaje," no es un comando valido.", 26);
            printMessage(&pWin, mensaje, WHITE);
        }

        clearCmdWindow(pWin.pCmdWindow);
    }
    return (EXIT_SUCCESS);
}

