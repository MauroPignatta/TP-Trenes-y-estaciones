#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char** argv) {
    struct sockaddr_in dirServer;
    dirServer.sin_family = AF_INET;
    dirServer.sin_addr.s_addr = inet_addr("192.168.1.11");
    dirServer.sin_port = htons(8080);
    
    int client = socket(AF_INET, SOCK_STREAM, 0);
    
    if (connect(client, (void *) &dirServer, sizeof(dirServer)) != 0 )
    {
        perror("No se pudo conectar con el servidor");
        return 1;
    }
    
    char mensaje[256];
    recv(client, &mensaje, sizeof(mensaje), 0 );
    puts(mensaje);
    
    while(1)
    {
        fgets(mensaje,sizeof(mensaje),stdin);
        send (client, mensaje, strlen(mensaje), 0);
    }
    
    return (EXIT_SUCCESS);
}

