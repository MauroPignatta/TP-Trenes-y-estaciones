#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {

    system("cls");

    if(argc!=2){
        printf("Ingrese el nombre del archivo de conf. como parametro\n");
        exit(3);
    }
    
    char nomArchivo[20] = "../config/";
    strcat(nomArchivo, argv[1]);
    TREN tren = inicializarTren(nomArchivo);
    
    /* Devuelve el socket ya configurado */
    int client = CrearSocketCliente();
    
    char mensaje[sizeMsj];

    /* Recibe un mensaje de bienvenida */
    recv(client, mensaje, sizeMsj, 0 );
    puts(mensaje);
    
    /* Flag para que un tren no pueda registrarse 2 veces*/
    int yaRegistrado = 1;

    while(1)
    {
        printf("\n1. Registrarse    2. Solicitar Anden");
        printf("\n3. Partir         4. Estado\n");
        printf("Elija una opcion: ");
        gets(mensaje);
        printf("\n");
        system("cls");
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
                printf("Todavia no implementado.\n");
                break;
            case '3':
                //partir
                printf("Todavia no implementado.\n");
                break;
            case '4':
                if(yaRegistrado)
                {
                    puts("Todavia no estas registrado en la estacion.");
                    break;
                } 
                printf("Estado: \n\n");
		        sprintf(mensaje,"4;%d",tren.ID);
                send(client, mensaje, strlen(mensaje), 0);
                recv(client, mensaje, sizeMsj, 0);
                estadoTren(mensaje);
                break;
            default:
                printf("\nOpcion invalida.\n");
        }
    }
    
    return (EXIT_SUCCESS);
}

