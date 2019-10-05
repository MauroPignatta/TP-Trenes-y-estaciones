#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {
    if(argc!=2){
        printf("Ingrese la ruta al archivo de conf. como parametro\n");
        exit(3);
    }
    
    int yaRegistrado = 1;
    
    char *nomArchivo=argv[1];
    TREN tren = inicializarTren(nomArchivo);
    
    /* Devuelve el socket ya configurado */
    int client = CrearSocketCliente();
    
    char mensaje[sizeMsj];

    /* Recibe un mensaje de bienvenida */
    recv(client, mensaje, sizeMsj, 0 );
    puts(mensaje);
    
    while(1)
    {
        gets(mensaje);
        switch(mensaje[0]){
            case '1':
                if (!yaRegistrado)
                {
                    puts("Ya te has registrado.");
                    break;
                }
                yaRegistrado = 0;
                registrarse(mensaje,tren,client);
                recv(client, mensaje, sizeMsj, 0);
                puts(mensaje);            
                break;
            case '2':
                //solicitar anden
                break;
            case '3':
               //partir();
                break;
            case '4':
		sprintf(mensaje,"4;%d",tren.ID);
                send(client, mensaje, strlen(mensaje), 0);
                recv(client, mensaje, sizeMsj, 0);
                estadoTren(mensaje);
                break;
        }   
    }
    
    return (EXIT_SUCCESS);
}

