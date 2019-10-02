#include "../lib/funcEstaciones.h"
#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

ESTACION ObtenerDatosEstacion()
{
    FILE * configEst = fopen("../config/configEstacion.txt","r") ;
    ESTACION est;
    if(!configEst)
    {
        printf("Error al abrir archivo de configuracion.\n");
        exit(1);
    }
    fscanf(configEst ,"%s %d",est.nombre, &est.distancia);
    
    for(int i = 0; i < 10; i++)
    {
        memset(est.tren + i,'\0',sizeof(TREN));
    }
    
    return est;
}

void leerMensaje()
{


}

void registrarTren()
{

}

void enviarTren()
{
	
}
	


void finalizarTren()
{

}


void estadoEstacion ()
{

}