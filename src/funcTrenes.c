#include "../lib/funcTrenes.h"

FILE * openFile(){
    FILE *ptr = fopen("configTrenes.txt","r");
    if(ptr == NULL){
        printf("No se pudo abrir el archivo de configuracion\n");
        exit(1);
    }
    return ptr;
}
char * armarMensaje(TREN aux,char *ptr){
    char *id=strint(aux.ID);
    char *combustible=strint(aux.combustible);
    strcat(ptr,";");
    strcat(ptr,id);
    strcat(ptr,";");
    strcat(ptr,combustible);
    strcat(ptr,";");
    strcat(ptr,aux.modelo);
    strcat(ptr,";");
    strcat(ptr,aux.estDestino);
    strcat(ptr,";");
    strcat(ptr,aux.estOrigen);
}
TREN pasarTren(){
    FILE *tren=openFile();
    TREN aux;
    char ptr[max_est];
    fgets(ptr,max_est,tren);
    aux.ID=atoi(ptr);
    fgets(ptr,max_est,tren);
    aux.combustible=atoi(ptr);
    fgets(ptr,max_est,tren);
    strcpy(aux.modelo,ptr);
    memset(aux.estDestino,'\0',max_est);
    memset(aux.estOrigen,'\0',max_est);
    return aux;
}
void registrarse(TREN aux){
    char *ptr=armarMensaje(aux);
    
}