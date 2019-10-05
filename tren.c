#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/Conexion.h"
#include "lib/funcTrenes.h"

int main(int argc, char** argv) {
    
    TREN tren = inicializarTren();

    /* Devuelve el socket ya configurado */
    int client = CrearSocketCliente();
    
    char *mensaje=(char*)malloc(sizeof(char)*sizeMsj);

    /* Recibe un mensaje de bienvenida */
    recv(client, &mensaje, sizeMsj, 0 );
    puts(mensaje);
    
    while(1)
    {
        fgets(mensaje, sizeMsj, stdin);
        
        registrarse(mensaje,tren,client);
    }
    
    return (EXIT_SUCCESS);
}

