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
    
    for(int i = 0; i < MAX_TREN; i++)
    {
        memset(est.tren + i,'\0',sizeof(TREN));
    }
    
    return est;
}

/* Busca en la estacion el primer espacio 
 * libre en el vector de trenes y devuelve
 * su posicion o -1 si no hay lugar disponible. */
int BuscarLugarDesocupado(ESTACION* estacion)
{
    int encontrado = -1;
    int i = 0;
    while (i < MAX_TREN && encontrado == -1)
    {
        encontrado = estacion->tren[i].ID == 0? i : -1;
        i++;
    }
    return encontrado;
}

int registrarTren(ESTACION * estacion, char * mensaje)
{
    int i = BuscarLugarDesocupado(estacion);
    if(i == -1)
    {
        printf("No queda espacio para mas trenes");
        return 0;
    }
    mensaje = mensaje + 2;
    
    //token ahora contiene el ID
    char * token = strtok(mensaje, ";");
    estacion->tren[i].ID = atoi(token);
    
    //token ahora contiene el combustible
    token = strtok(NULL, ";");
    estacion->tren[i].combustible = atoi(token);
    
    //token ahora contiene el modelo
    token = strtok(NULL, ";");
    strcpy(estacion->tren[i].modelo, token);
    
    //copio el nombre de la estacion en la estacion origen del tren
    strcpy(estacion->tren[i].estOrigen, estacion->nombre);
    estacion->tren[i].tiempoRestante = 0;
    
    return 1;
}

void enviarTren()
{
	
}
	


void finalizarTren()
{

}

/*Devuelve la posicion en la que se encuentra el tren
en el vector de trenes de la estacion, o -1 si no se
encuentra*/
int BuscarTrenEnEstacion(ESTACION estacion, int idTren)
{
    int i = 0;
    while(i < MAX_TREN)
    {
        if (idTren == estacion.tren[i].ID)
            return i;
        i++;
    }
    return -1;
}

void estadoDelTren (ESTACION estacion, char * mensaje)
{
    int idTren;
    mensaje = mensaje + 2;
    sscanf(mensaje, "%d", &idTren);
    int pos = BuscarTrenEnEstacion(estacion, idTren);
    if (pos != -1)
    {
        mensaje = mensaje - 2;
        sprintf(mensaje,"%d;%d;%s;%s;%s;%d;",
                estacion.tren[pos].ID, estacion.tren[pos].combustible,
                estacion.tren[pos].modelo, estacion.tren[pos].estOrigen,
                estacion.tren[pos].estDestino, estacion.tren[pos].tiempoRestante);
    }
    else
    {
        strcpy(mensaje, "No se encuentra registrado.");
    }
}
void cargarCombustible(int *combustible){
    
    int resto=500-(*combustible);
    *combustible+=resto;
}