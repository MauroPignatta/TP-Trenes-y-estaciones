#include "../lib/funcEstaciones.h"
#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

ESTACION ObtenerDatosEstacion(char * nomArchivo)
{
    FILE * configEst = fopen(nomArchivo,"r") ;
    ESTACION est;
    if(!configEst)
    {
        printf("Error al abrir archivo de configuracion.\n");
        exit(1);
    }
    
    fscanf(configEst ,"Nombre: %s\n",est.nombre);
    fscanf(configEst ,"Distancia: %d", &est.distancia);
    
    for(int i = 0; i < MAX_TREN; i++)
    { 
        memset(&est.tren[i], 0,sizeof(TREN));
    }
    fclose(configEst);
    return est;
}

/* Devuelve un vector con las posiciones del vector de trenes
en las que se encuentran */
void buscarTrenes( TREN trenes[] ,int posTrenes[])
{
    int j = 0;
    for(int i = 0; i < MAX_TREN; i++)
    {
        if(trenes[i].ID != 0)
        {
            posTrenes[j] = i;
            j++;
        }
    }
}

/*Devuelve la posicion en la que se encuentra el tren
en el vector de trenes de la estacion, o -1 si no se
encuentra*/
int BuscarTrenPorID(ESTACION estacion, int idTren)
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

/*Busca la primer posicion vacia en el vector de trenes
y devuelve la posicion, o -1 en caso de no haber
lugares vacios.*/
int BuscarPosVacia(ESTACION* estacion)
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
    int i = BuscarPosVacia(estacion);
    if(i == -1)
    {
        printf("No queda espacio para mas trenes");
        return 0;
    }
    mensaje = mensaje + 2;
    
    //token ahora contiene el ID
    char * token = strtok(mensaje, ";");
    int ID = atoi(token);
    int yaExisteID = BuscarTrenPorID(*estacion, ID);
    if (yaExisteID != -1)
    {
    	printf("\nYa hay un tren con ese ID registrado.");
    	return 0;
    }
    
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
    strcpy(estacion->tren[i].estDestino, "A asignar");
    return 1;
}

void enviarTren()
{
	
}
	


void finalizarTren()
{

}

void estadoDelTren (ESTACION estacion, char * mensaje)
{
    int idTren;
    mensaje = mensaje + 2;
    sscanf(mensaje, "%d", &idTren);
    int pos = BuscarTrenPorID(estacion, idTren);
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
    
    *combustible=500;
}


