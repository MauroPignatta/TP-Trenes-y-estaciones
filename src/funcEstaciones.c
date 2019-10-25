#include "../lib/funcEstaciones.h"
#include "../lib/funcTrenes.h"
#include "../lib/est_interface.h"
#include "../lib/Conexion.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ObtenerOtrasEstaciones(ESTACION est[],int miPos)
{
    char nombreArchivo[30];
    FILE* configEst;
    for (int i = 0; i < MAX_ESTACION; i++)
    {
        if( i != miPos)
        {
            sprintf(nombreArchivo, "../config/Estacion%d.conf", i + 1);
            configEst = fopen(nombreArchivo, "r");
            fscanf(configEst ,"Nombre: %s\n",est[i].nombre);
            fscanf(configEst ,"Distancia: %d\n", &est[i].distancia);
            fscanf(configEst ,"ID: %d\n", &est[i].ID);
            fclose(configEst);
            est[i].online = 0;
        }
    }
}

int ObtenerDatosMiEstacion(char * nomArchivo, ESTACION est[])
{
    FILE * configEst = fopen(nomArchivo,"r") ;
    ESTACION estAux;
    if(!configEst)
    {
        printf("Error al abrir archivo de configuracion.\n");
        exit(1);
    }
    
    fscanf(configEst ,"Nombre: %s\n",estAux.nombre);
    fscanf(configEst ,"Distancia: %d\n", &estAux.distancia);
    fscanf(configEst ,"ID: %d\n", &estAux.ID);
    for(int i = 0; i < MAX_TREN; i++)
    { 
        memset(&estAux.tren[i], 0,sizeof(TREN));
    }
    int MyPos = estAux.ID -1;
    est[MyPos] = estAux;
    est[MyPos].online = 2;
    fclose(configEst);
    return MyPos;
}

/* Devuelve un vector con las posiciones del vector de trenes
en las que se encuentran */
int buscarTrenes( TREN trenes[] ,int posTrenes[])
{
    int cantEncontada = 0;
    for(int i = 0; i < MAX_TREN; i++)
    {
        if(trenes[i].ID != 0)
        {
            posTrenes[cantEncontada] = i;
            cantEncontada++;
        }
    }
    return cantEncontada;
}

/*Devuelve la posicion en la que se encuentra el tren
en el vector de trenes de la estacion, o -1 si no se
encuentra*/
int BuscarTrenPorID(ESTACION estacion, int idTren)
{
    int i = 0;
    while(i < MAX_TREN)
    {
        if (idTren == estacion.tren[i].ID)
            return i;
        i++;
    }
    return -1;
}

/*Busca la primer posicion vacia en el vector de trenes
y devuelve la posicion, o -1 en caso de no haber
lugares vacios.*/
int BuscarPosVacia(ESTACION* estacion)
{
    int encontrado = -1;
    int i = 0;
    while (i < MAX_TREN && encontrado == -1)
    {
        encontrado = estacion->tren[i].ID == 0? i : -1;
        i++;
    }
    return encontrado;
}

int registrarTren(ESTACION * estacion, char * mensaje)
{
    int i = BuscarPosVacia(estacion);
    if(i == -1)
    {
        return 0;
    }
    mensaje = mensaje + 4;
    
    //token ahora contiene el ID
    char * token = strtok(mensaje, ";");
    int ID = atoi(token);
    int yaExisteID = BuscarTrenPorID(*estacion, ID);
    if (yaExisteID != -1)
    {
    	return 0;
    }
    
    estacion->tren[i].ID = atoi(token);
    
    //token ahora contiene el combustible
    token = strtok(NULL, ";");
    estacion->tren[i].combustible = atoi(token);
    
    //token ahora contiene el modelo
    token = strtok(NULL, ";");
    strcpy(estacion->tren[i].modelo, token);
    
    //copio el nombre de la estacion en la estacion origen del tren
    strcpy(estacion->tren[i].estOrigen, estacion->nombre);
    estacion->tren[i].tiempoRestante = 0;
    strcpy(estacion->tren[i].estDestino, "A asignar");
    return 1;
}

void enviarTren()
{
	
}
	


void finalizarTren()
{

}

void cargarCombustible(int *combustible){
    
    *combustible=500;
}

void ConexionServer()
{
    char mensaje[sizeMsj];
    sprintf(mensaje,"../config/Red%d.conf", miPos + 1);

    int server = CrearSocketServer(mensaje);  //Devuelve el socket ya configurado
    
    int maxfd = server + MaxClientes + 1;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    
    //Variables con los datos para la conexion
    int client[MaxClientes];
    memset(client , -1, sizeof(client));
    int n = 0;

    while (1)
    {
        fd_set copy = master;
        int seQuienHablo = 0;
        int bytesRecibidos = 0;
        int TrenID = 0;
        int posEst = -1;
        int posTren = 0;
        int regCorrecto = 0;
        char opcion = ' '; 
        memset(mensaje,'\0',sizeMsj);

        select(maxfd, &copy, NULL, NULL, NULL); 
        
       // si es un cliente nuevo
        if ( FD_ISSET(server, &copy))
        {
            /* acepta al nuevo cliente */
            client[n] = accept(server, 0, 0);
            recv(client[n], mensaje, sizeMsj, 0);

            if ( esTren( mensaje[0]) )
            {
                printRegistro(&pWin,"Se conecto un nuevo tren", WHITE);
                sprintf(mensaje,"Bienvenido a la estacion %s", estaciones[miPos].nombre);
                send(client[n], mensaje, strlen(mensaje), 0);
            }
            else if ( esEstacion( mensaje[0]) )
            {
                printRegistro(&pWin,"Se conecto una Estacion", WHITE);
            }
            /* lo agrega al fd */
            FD_SET(client[n] ,&master);
            n++;
        }
        else // si ya lo conoce
        {
            seQuienHablo = 0;
            for(int i = 0; i < MaxClientes && !seQuienHablo; i ++) //Busco quien me hablo
            {
                if (FD_ISSET(client[i], &copy))
                {
                    seQuienHablo = 1;
                    // Recibo el mensaje
                    bytesRecibidos = recv(client[i], mensaje, sizeMsj, 0 ); // recibo el mensaje que me dice si es tren o estacion.
                    
                    //  Para saber si el cliente se desconecto 
                    if (bytesRecibidos <= 0) // si el cliente se desconecta haciendo ctrl + c, no vamos a poder saber quien era
                    {
                        printRegistro(&pWin,"Se desconecto algo", WHITE);
                        FD_CLR(client[i], &master);
                    }

                    else
                    {
                        if ( esTren ( mensaje[0] )) //compruebo si me hablo un tren
                        {
                            opcion = mensaje[2];
                            switch (opcion)
                            {
                                case '1':
                                    /*Registro al tren*/
                                    regCorrecto = registrarTren(&estaciones[miPos], mensaje);
                                    sprintf(mensaje,"1;%s;Te has registrado correctamente", estaciones[miPos].nombre);

                                    /*Comprueba que el tren se haya registrado*/
                                    if (!regCorrecto)
                                    {
                                        printRegistro(&pWin,"No se pudo registrar a un\ntren: Ya registrado.", WHITE);
                                        strcpy(mensaje,"2;No te has podido registrar: Ya existe un tren con el mismo ID.");
                                    }
                                    else{
                                        printRegistro(&pWin,"Un tren se ha registrado", WHITE);
                                    }
                                    /*Envio una respuesta al tren*/
                                    send(client[i], mensaje, sizeMsj, 0);
                                    break;
                                   
                                case '2':
                                    // solicitar anden
                                    break;
                                    
                                case '3':
                                    // partir
                                    break;
                                    
                                case '4':
                                    /* Esto va a haber que cambiarlo, no borrar por ahora.

                                    estadoDelTren(estacion ,mensaje);
                                    puts("Estado enviado");
                                    send(clientTrenes[i], mensaje, strlen(mensaje), 0);
                                    */
                                    break;
                                case '5':   //Agrego este para cuando un tren quiere desconectarse
                                    
                                    sscanf(mensaje, "1;5;%d", &TrenID);
                                    posTren = BuscarTrenPorID(estaciones[miPos], TrenID);

                                    if (posTren != -1)
                                        estaciones[miPos].tren[posTren].ID = 0;
                                    
                                    printRegistro(&pWin,"Se desconecto un tren", WHITE);
                                    FD_CLR(client[i], &master);

                                    break;
                            }
                        }
                        else if ( esEstacion( mensaje[0] )) // si el mensaje recibido es de una estacion
                        {
                            opcion = mensaje[2];

                            switch (opcion)
                            {

                                case '5': //exit
                                    sscanf(mensaje, "2;5;%d", &posEst);
                                    estaciones[posEst].online = 0;

                                    //bytesRecibidos = recv(client[i], mensaje, sizeMsj, 0);

                                    //if (bytesRecibidos <= 0)
                                    //{
                                        printRegistro(&pWin,"Se desconecto una estacion", WHITE);
                                        FD_CLR(client[i], &master);
                                    //}
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}