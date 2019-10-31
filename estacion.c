#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"
#include "lib/est_interface.h"

int main(int argc, char** argv) 
{ 
    if (argc != 2)
    {
    	printf("\nuso: ./estacion <Nombre archivo Conf Estacion>\n");
        exit(3);
    } 

    system("clear");

    //Obtengo los datos de las estaciones. 
    char nomArchivoEst[40] = "../config/estacion/";
    strcat(nomArchivoEst, FormatearNombre(argv[1]));
    miPos = ObtenerDatosMiEstacion( nomArchivoEst, estaciones); 
    ObtenerOtrasEstaciones(estaciones, miPos);

    /* Hilo para la interfaz grafica */
    pthread_t Interfaz;
    wmove(pWin.pCmdWindow, 0,0);
    pthread_create(&Interfaz, NULL,(void*) InterfazGrafica ,(void *) argv);

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