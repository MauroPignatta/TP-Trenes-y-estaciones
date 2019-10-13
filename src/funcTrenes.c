#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include 
#include "../lib/Conexion.h"

FILE * openFile(char * nombreArchivo){
    FILE *ptr = fopen(nombreArchivo,"r");
    if(ptr == NULL){
        printf("No se pudo abrir el archivo de configuracion\n");
        exit(1);
    }
    return ptr;
}

char * armarMensaje(TREN tren,char *ptr){
    ptr+=1;
    sprintf(ptr,";%d;%d;%s;%s;%s;",
        tren.ID, tren.combustible, tren.modelo,
        tren.estDestino, tren.estOrigen);
    return ptr;
}

TREN inicializarTren(char *arch){
    
    FILE *tren = openFile(arch);
    TREN aux;
    char ptr[max_est];
    fgets(ptr, max_est, tren);
    aux.ID = atoi(ptr);
    fgets(ptr, max_est, tren);
    aux.combustible = atoi(ptr);
    fscanf(tren,"%s",ptr);
    strcpy(aux.modelo,ptr);
    aux.tiempoRestante = 0;
    strcpy(aux.estDestino,"A asignar");
    strcpy(aux.estOrigen," ");
    fclose(tren);
    return aux;
}

void registrarse(char *mensaje,TREN tren)
{
    armarMensaje(tren, mensaje);
}

void estadoTren(char * mensaje){
    char * token;
    token=strtok(mensaje,";");
    printf("El identificador del tren es: %s\n", token);
    token=strtok(NULL,";");
    printf("El combustible restante es: %s\n", token);
    token=strtok(NULL,";");
    printf("El modelo del tren es: %s\n", token);
    token=strtok(NULL,";");
    printf("La estacion en la q se encuentra es: %s\n", token);
    token=strtok(NULL,";");
    printf("La estacion destino es: %s\n", token);
    token=strtok(NULL,";");
    printf("El tiempo restante de viaje es: %s\n", token);
}
void imprimirEstadoDelTren(TREN t)
{
    printf("%d",t.ID);
    printf("%d",t.combustible);
    printf("%s",t.modelo); 
    printf("%s",t.estOrigen);
    printf("%s",t.estDestino);
    printf("%d",t.tiempoRestante);
}
void solicitarPasoAnden(){
	
}
void partir(TREN tren){
    for(int i=tren.tiempoRestante;i>0;i--){
	sleep(1);       
	tren.tiempoRestante--;
    }	
}

