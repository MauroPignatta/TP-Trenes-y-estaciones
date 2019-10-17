#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
    sprintf(ptr,"1;%d;%d;%s;%s;%s;",
        tren.ID, tren.combustible, tren.modelo,
            tren.estDestino, tren.estOrigen);
    return ptr;
}

TREN inicializarTren(char *arch){
    
    FILE *tren = openFile(arch);
    TREN aux;
    
    fscanf(tren, "ID: %d\n", &aux.ID);
    
    fscanf(tren, "Combustible: %d\n",&aux.combustible);

    fscanf(tren, "Modelo: %s", aux.modelo);
   
    aux.tiempoRestante = 0;
    strcpy(aux.estDestino,"A asignar");
    strcpy(aux.estOrigen,"A asignar");
    fclose(tren);
    return aux;
}

void registrarse(char *mensaje,TREN tren)
{
    armarMensaje(tren, mensaje);
}

void armarMensajeEstadoDelTren(TREN t, char * mensaje)
{
    sprintf(mensaje,"Estado:\n\n");
    mensaje += strlen(mensaje);
    sprintf(mensaje,"ID: %d\n",t.ID);
    mensaje += strlen(mensaje);
    sprintf(mensaje,"Combustible restante: %d\n",t.combustible);
    mensaje += strlen(mensaje);
    sprintf(mensaje,"Modelo: %s\n",t.modelo); 
    mensaje += strlen(mensaje);
    sprintf(mensaje,"Estacion Origen: %s\n",t.estOrigen);
    mensaje += strlen(mensaje);
    sprintf(mensaje,"Estacion Destino: %s\n",t.estDestino);
    mensaje += strlen(mensaje);
    sprintf(mensaje,"Tiempo de viaje restante: %d\n",t.tiempoRestante);
}
void solicitarPasoAnden(){
	
}
void partir(TREN tren){
    for(int i=tren.tiempoRestante;i>0;i--){
	sleep(1);       
	tren.tiempoRestante--;
    }	
}

