#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {

    system("clear");

    if(argc!=2){
        printf("Ingrese el nombre del archivo de conf. como parametro\n");
        exit(3);
    }
    FormatearNombreArchivo(argv[1]);
    char nomArchivo[20] = "../config/";
    strcat(nomArchivo, argv[1]);
    TREN tren = inicializarTren(nomArchivo);
    
    /* Devuelve el socket ya configurado */
    int client = CrearSocketCliente();
    
    char* mensaje = (char*) malloc(sizeMsj);

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
        system("clear");
        switch(mensaje[0]){
            case '1':
                if (!yaRegistrado)
                {
                    puts("Ya te has registrado.");
                    break;
                }
                registrarse(mensaje,tren);
                send (client, mensaje, strlen(mensaje), 0);

                recv(client, mensaje, sizeMsj, 0);
                if (*mensaje == '1')
                {
                    yaRegistrado = 0;
		    sprintf(mensaje,"1;%s;",tren.estOrigen);	
                }
		else{
		    mensaje += 2;
                    puts(mensaje);
                    mensaje -= 2;  	
		}
                         
                break;
            case '2':
                //solicitar anden
                printf("Todavia no implementado.\n");
                break;
            case '3':
		printf("A dónde desea viajar?\n");
		gets(tren.estDestino);                
		partir(&tren);//partir
                //printf("Todavia no implementado.\n");
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

