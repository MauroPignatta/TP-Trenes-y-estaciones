#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
    
    fscanf(tren, "ID: %d", &aux.ID);
    
    fscanf(tren, "Combustible: %d",&aux.combustible);

    fscanf(tren, "Modelo: %s", aux.modelo);
   
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
