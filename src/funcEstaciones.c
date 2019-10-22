#include "../lib/funcEstaciones.h"
#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ObtenerOtrasEstaciones(ESTACION est[],int miPos)
{
    char nombreArchivo[30];
    FILE* configEst;
    for (int i = 0; i < MAX_ESTACION; i++)
    {
        if( i != miPos)
        {
            sprintf(nombreArchivo, "../config/Estacion%d.conf", i + 1);
            configEst = fopen(nombreArchivo, "r");
            fscanf(configEst ,"Nombre: %s\n",est[i].nombre);
            fscanf(configEst ,"Distancia: %d\n", &est[i].distancia);
            fscanf(configEst ,"ID: %d\n", &est[i].ID);
            fclose(configEst);
            est[i].online = 0;
        }
    }
}

int ObtenerDatosMiEstacion(char * nomArchivo, ESTACION est[])
{
    FILE * configEst = fopen(nomArchivo,"r") ;
    ESTACION estAux;
    if(!configEst)
    {
        printf("Error al abrir archivo de configuracion.\n");
        exit(1);
    }
    
    fscanf(configEst ,"Nombre: %s\n",estAux.nombre);
    fscanf(configEst ,"Distancia: %d\n", &estAux.distancia);
    fscanf(configEst ,"ID: %d\n", &estAux.ID);
    for(int i = 0; i < MAX_TREN; i++)
    { 
        memset(&estAux.tren[i], 0,sizeof(TREN));
    }
    int MyPos = estAux.ID -1;
    est[MyPos] = estAux;
    est[MyPos].online = 2;
    fclose(configEst);
    return MyPos;
}

/* Devuelve un vector con las posiciones del vector de trenes
en las que se encuentran */
int buscarTrenes( TREN trenes[] ,int posTrenes[])
{
    int cantEncontada = 0;
    for(int i = 0; i < MAX_TREN; i++)
    {
        if(trenes[i].ID != 0)
        {
            posTrenes[cantEncontada] = i;
            cantEncontada++;
        }
    }
    return cantEncontada;
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
        return 0;
    }
    mensaje = mensaje + 4;
    
    //token ahora contiene el ID
    char * token = strtok(mensaje, ";");
    int ID = atoi(token);
    int yaExisteID = BuscarTrenPorID(*estacion, ID);
    if (yaExisteID != -1)
    {
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

void cargarCombustible(int *combustible){
    
    *combustible=500;
}


