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

/*Invierte la cadena*/
/*void strrev(char *str)
{
    char *aux =(char*) malloc(sizeof(char) * 11);
    strcpy(aux,str);
    int i = 0;
    int n = strlen(str) - 1;
    while(*str)
    {
        *str = *(aux + n - i);
        str++;
        i++;
    }
}*/

/* Pasa de entero a string */
/*char * inttostr(int n)
{
    char *str =(char*) malloc(sizeof(char) * 11);
    int i = 0;
    while(n > 0)
    {
        int aux = n % 10;
        n = n / 10;
        *(str + i) = '0' + aux;
        i++;
    }
    *(str + i) = '\0';
    strrev(str);
    return str;
}*/

char * armarMensaje(TREN tren,char *ptr){
    ptr+=1;
    sprintf(ptr,";%d;%d;%s;%s;%s;",tren.ID,tren.combustible,tren.modelo,tren.estDestino,tren.estOrigen);
    /*strcat(ptr,";");
    strcat(ptr,id);
    strcat(ptr,";");
    strcat(ptr,combustible);
    strcat(ptr,";");
    strcat(ptr,tren.modelo);
    strcat(ptr,";");
    strcat(ptr,tren.estDestino);
    strcat(ptr,";");
    strcat(ptr,tren.estOrigen);
    strcat(ptr,";");*/
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
    strcpy(aux.estDestino," ");
    strcpy(aux.estOrigen," ");
    fclose(tren);
    return aux;
}

void registrarse(char *mensaje,TREN tren,int client)
{
    armarMensaje(tren, mensaje);  //armarMensaje(&tren, mensaje);
    send (client, mensaje, strlen(mensaje), 0);
}

