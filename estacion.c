#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/funcEstaciones.h"
#include "lib/Conexion.h"

int main(int argc, char** argv) {
    
    ESTACION estacion = ObtenerDatosEstacion();
    
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
    int regCorrecto;
    
    while (1)
    {
        fd_set copy = master;
        int nready = select(maxfd, &copy, NULL, NULL, NULL); 
        
	// si es un cliente nuevo
        if ( FD_ISSET(server, &copy))
        {
            /* Falta preparar / mejorar el mensaje de bienvenida */
            strncpy(mensaje, "Bienvenido a la estacion", sizeMsj);
            puts("Un nuevo tren se ha conectado");

            /* acepta al nuevo tren y le envia el mensaje de bienvenida*/
            client[n] = accept(server, 0, 0);
            send(client[n], mensaje, sizeMsj, 0);

            /* lo agrega al fd */
            FD_SET(client[n],&master);
            n++;
        }

	// si ya lo conoce
        else {
            for(int i = 0; i < n; i ++)
            {
                if (FD_ISSET(client[i], &copy))
                {
                    memset(mensaje,'\0',sizeof(mensaje));
                    // Recibo el mensaje
                    int bytes = recv(client[i], mensaje, sizeof(mensaje), 0 );
                    
		    //  Para saber si el cliente se desconecto 
                    if (bytes <= 0)
                    {
                        memset(mensaje,'\0', sizeof(mensaje));
                        printf("Se desconecto el cliente %d.\n", client[i]);
                        FD_CLR(client[i],&master);
                    }
                    else{
                        char opcion = mensaje[0];
                        
                        switch (opcion)
                        {
                            case '1':
                                /*Registro al tren*/
                            	puts("Registrando tren");
                                regCorrecto = registrarTren(&estacion, mensaje);
                                strcpy(mensaje,"Te has registrado correctamente");
                                
                                /*Comprueba que el tren se haya registrado*/
                                if (!regCorrecto)
                                {
                                	puts("No se pudo registrar al tren");
                                    strcpy(mensaje,"No te has podido registrar");
                                }
                                //puts("Registro de tren correcto");

                                /*Envio una respuesta al tren*/
                                send(client[i], mensaje, strlen(mensaje), 0);
                                break;
                               
                            case '2':
                                // solicitar anden
                                break;
                                
                            case '3':
                                // partir
                                break;
                                
                            case '4':
                                estadoDelTren(estacion ,mensaje);
                                puts(mensaje);
                                send(client[i], mensaje, strlen(mensaje), 0);
                                break;
                                
                            default:
                                printf("Nunca deberia salir por el default\n");
                                printf("Si ves este mensaje, hay algo que esta mal\n");
                        }
                    }
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}
