#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"
#include "lib/tren_interface.h"

int main(int argc, char** argv) {

    if(argc != 3){
        printf("\nuso: ./tren <Nombre archivo Conf Tren> <Nombre de Estacion a conectarse>\n");
        exit(3);
    }

    system("clear");

    pthread_t Interfaz;
    pthread_create(&Interfaz, NULL,(void*) InterfazGrafica , (void*) argv);    

    pthread_join(Interfaz, NULL);

<<<<<<< HEAD
    //Aca empieza a correr ncurses
    ST_APP_WINDOW pWin;
    InitInterfazDeUsuario(&pWin);
    dibujarInterfazDeUsuario(&pWin);
    
    sprintf(mensaje, " Tren %d ",tren.ID);
    imprimirTituloDeVentana(pWin.pAppFrame, mensaje);
    imprimirTituloDeVentana(pWin.pLogFrame, "### Log ###");
    imprimirTituloDeVentana(pWin.pCmdFrame, "### Comandos ###");

    recv(client, mensaje, sizeMsj, 0);  // Recibo el mensaje de Bienvenido a la estacion <nombre estacion>
    imprimirMensaje(&pWin, mensaje, WHITE);
    wmove(pWin.pCmdWindow, 0,0);

    while(1)
    {
        memset(mensaje, '\0', sizeMsj);
        wgetnstr(pWin.pCmdWindow, mensaje, sizeMsj);

        if(! strcmp(mensaje, "help"))
        {
            limpiarVentanaDeLog(pWin.pLogWindow);
            printHelp(&pWin);
        }

        else if(!strcmp(mensaje, "registrarse"))
        {
            limpiarVentanaDeLog(pWin.pLogWindow);
            if (yaRegistrado)
            {
                imprimirMensaje(&pWin, "Ya te has registrado.", WHITE);
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
                imprimirMensaje(&pWin, token, WHITE);
            }
        }

        else if(!strcmp(mensaje, "anden"))
        {
            //solicitar anden
            limpiarVentanaDeLog(pWin.pLogWindow);
            imprimirMensaje(&pWin, "Todavia no implementado.", WHITE);
        }
        
        else if(!strcmp(mensaje, "partir"))
        {
            if (yaRegistrado)
            {
                armarMensajePartir(tren, mensaje);
                send(client, mensaje, sizeMsj, 0);

                recv(client, mensaje, sizeMsj, 0);

                limpiar_imprimirLog(&pWin, mensaje);

                if (strcmp(mensaje, "No hay estaciones disponibles"))
                {
                    limpiar_imprimirLog(&pWin, mensaje);

                    limpiarVentanaDeCmd(pWin.pCmdWindow);
                    wgetnstr(pWin.pCmdWindow, mensaje, sizeMsj);
                    send(client, mensaje, sizeMsj, 0);

                    recv(client, mensaje, sizeMsj, 0);
                    if (!strcmp(mensaje, "OK"))
                    {
                        recv(client, mensaje, sizeMsj, 0);
                        tren.tiempoRestante = atoi(mensaje);
                        tren.combustible -= restarCombustible(tren.tiempoRestante);
                        DibujarTrenViajando(pWin.pLogWindow, &tren.tiempoRestante);
                        
                        matarTren(tren, client, &pWin);
                    }
                    else
                    {
                        limpiar_imprimirLog(&pWin, mensaje);
                    }
                }
            }
            else 
            {
                limpiar_imprimirLog(&pWin, "Es necesario estar registrado en la estacion.");
            }
        }
        
        else  if(!strcmp(mensaje, "estado"))
        {
            armarMensajeEstadoDelTren(tren, mensaje);
            limpiar_imprimirLog(&pWin, mensaje);
        }


        else  if(!strcmp(mensaje, "exit"))
        {
            matarTren(tren, client, &pWin);
        }

        else
        {
            strncat(mensaje," no es un comando valido.", 26);
            limpiar_imprimirLog(&pWin, mensaje);
        }

        limpiarVentanaDeCmd(pWin.pCmdWindow);
    }
=======
>>>>>>> master
    return (EXIT_SUCCESS);
}