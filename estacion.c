#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/funcEstaciones.h"

#define MaxClientes 3



int main(int argc, char** argv) {
    
    ESTACION Estacion;

    int nready;
    //estructura que contiene datos del servidor 
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    dirServer.sin_port = htons(8080);

    char msjServer[256]= "Bienvenido al servidor";
    
    //crea el socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    
    //libera el socket al cerrar la aplicacion
    int activado = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
    
    //comprobar error
    if ( bind(server, (struct sockaddr*) &dirServer,sizeof (dirServer)) != 0)
    {
        perror("Error de conexion");
        return 1;
    }
    
    puts("Escuchando...");
    listen(server, MaxClientes);
    
    int maxfd = server + MaxClientes;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    //-------------------------------------------------------------------------
    
    // Aceptar conexion y enviar msj bienvenida 
    int client[MaxClientes]; 
    memset(client,-1,sizeof(client));
    
    //--------------------------------------------------------------------------
    
    int n = 0;
    
    while (1)
    {
	fd_set copy = master;
        nready = select(maxfd, &copy, NULL, NULL, NULL); 
        
        if ( FD_ISSET(server, &copy))
        {
            strcpy(msjServer,"Bienvenido");
            puts("Cliente nuevo");
            client[n] = accept(server, 0, 0 );
            send(client[n], msjServer, sizeof(msjServer), 0);
            FD_SET(client[n],&master);
		n++;
        }
        else {
            for(int i = 0; i < n; i ++)
            {
                if (FD_ISSET(client[i], &copy))
                {
                    memset(msjServer,'\0',sizeof(msjServer));
                    int bytes = recv(client[i], msjServer, sizeof(msjServer), 0 );
                    if (bytes <= 0)
                    {
			memset(msjServer,'\0', sizeof(msjServer));
                        printf("Se desconecto el cliente %d.\n", client[i]);
                        FD_CLR(client[i],&master);
                    }
                    else{
			printf("Mensaje Recibido del cliente %d: %s", client[i], msjServer);
                    }
                }
            }
        }
	}
    
    return (EXIT_SUCCESS);
}
