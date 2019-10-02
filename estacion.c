#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"

int main(int argc, char** argv) {
    
    ESTACION Estacion = ObtenerDatosEstacion();
    
    char mensaje[sizeMsj]= "";
    
    //Devuelve el socket ya configurado
    int server = CrearSocketServer();
    
    int maxfd = server + MaxClientes;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    int client[MaxClientes]; 
    memset(client,-1,sizeof(client));
    int n = 0;
    
    while (1)
    {
        fd_set copy = master;
        int nready = select(maxfd, &copy, NULL, NULL, NULL); 
        
        if ( FD_ISSET(server, &copy))
        {
            /**/
            strncpy(mensaje, "Bienvenido a la estacion", sizeMsj);
            puts("Un nuevo tren se ha conectado");
            client[n] = accept(server, 0, 0);
            send(client[n], mensaje, sizeMsj, 0);
            FD_SET(client[n],&master);
            n++;
        }
        else {
            for(int i = 0; i < n; i ++)
            {
                if (FD_ISSET(client[i], &copy))
                {
                    memset(mensaje,'\0',sizeof(mensaje));
                    int bytes = recv(client[i], mensaje, sizeof(mensaje), 0 );
                    if (bytes <= 0)
                    {
                        memset(mensaje,'\0', sizeof(mensaje));
                        //printf("Se desconecto el cliente %d.\n", client[i]);
                        FD_CLR(client[i],&master);
                    }
                    else{
                        
                        /* Aca iria el switch enorme cuando recibe un mensaje del tren*/
                        
                    }
                }
            }
        }
	}
    return (EXIT_SUCCESS);
}
