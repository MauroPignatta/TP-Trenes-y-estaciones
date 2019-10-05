#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {
    
    TREN tren = inicializarTren();
    
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
                registrarse(mensaje,tren,client);
                break;
            case '2':
                //solicitar anden
                break;
            case '3':
               //partir();
                break;
            case '4':
		send(client, mensaje, strlen(mensaje), 0);
                //estadoTren();
                break;
        }   
    }
    
    return (EXIT_SUCCESS);
}

