#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {
    
    TREN tren = pasarTren();

    /* Devuelve el socket ya configurado*/
    int client = CrearSocketCliente();
    
    char mensaje[sizeMsj];
    recv(client, &mensaje, sizeMsj, 0 );
    puts(mensaje);
    
    while(1)
    {
        
        fgets(mensaje, sizeMsj, stdin);
        //armarMensaje(&tren, mensaje);
        send (client, mensaje, strlen(mensaje), 0);
    }
    
    return (EXIT_SUCCESS);
}

