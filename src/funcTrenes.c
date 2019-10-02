#include "../lib/funcTrenes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE * openFile(){
    FILE *ptr = fopen("../config/configTrenes.txt","r");
    if(ptr == NULL){
        printf("No se pudo abrir el archivo de configuracion\n");
        exit(1);
    }
    return ptr;
}

/*Invierte la cadena*/
void strrev(char *str)
{
    char *aux =(char*) malloc(sizeof(char) * 11);
    strcpy(aux,str);
    int i = 0;
    int n = strlen(str) - 1;
    while(*aux)
    {
        *(str + i) = *(aux + n - i);
        i++;
    }
}

/* Pasa de entero a string */
char * strint(int n)
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
}

char * armarMensaje(TREN tren,char *ptr){
    char *id = strint(tren.ID);
    char *combustible = strint(tren.combustible);
    strcat(ptr,";");
    strcat(ptr,id);
    strcat(ptr,";");
    strcat(ptr,combustible);
    strcat(ptr,";");
    strcat(ptr,tren.modelo);
    strcat(ptr,";");
    strcat(ptr,tren.estDestino);
    strcat(ptr,";");
    strcat(ptr,tren.estOrigen);
}

TREN pasarTren(){
    FILE *tren = openFile();
    TREN aux;
    char ptr[max_est];
    fgets(ptr, max_est, tren);
    aux.ID = atoi(ptr);
    fgets(ptr, max_est, tren);
    aux.combustible = atoi(ptr);
    fgets(ptr, max_est, tren);
    strcpy(aux.modelo,ptr);
    memset(aux.estDestino, '\0', max_est);
    memset(aux.estOrigen, '\0', max_est);
    fclose(tren);
    return aux;
}

void registrarse()
{
    
}