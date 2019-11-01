#include "../lib/funcEstaciones.h"
#include "../lib/funcTrenes.h"
#include "../lib/est_interface.h"
#include "../lib/Conexion.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ObtenerOtrasEstaciones(ESTACION est[],int miPos)
{
    char nombreArchivo[40];
    FILE* configEst;
    for (int i = 0; i < MAX_ESTACION; i++)
    {
        if( i != miPos)
        {
            sprintf(nombreArchivo, "../config/estacion/Estacion%d.conf", i + 1);
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
int BuscarPosVacia(ESTACION * estacion)
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

int mostrarTrenesMigrados(char * mensaje)
{   
    char ID[13];
    int cantTrenesMigrados = 0;
    strcpy(mensaje, "Elija el ID del tren que quiere viajar: \n\n");
    for(int i = 0; i < MAX_TREN; i++)
    {
        if(estaciones[miPos].tren[i].migrado != 0)
        {
            cantTrenesMigrados ++ ;
            sprintf(ID,"Tren ID: %d\n", estaciones[miPos].tren[i].ID);
            strcat(mensaje, ID);
        }
    }
    return cantTrenesMigrados;
}

/*Pide al usuario que ingrese el tren que quiere que viaje
y devuelve la posicion del tren en el vector, o -1 en caso de que el tren
elegido no sea valido.*/
int elegirTren()
{
    char opcion[4];
    clearCmdWindow(pWin.pCmdWindow);
    wgetnstr(pWin.pCmdWindow, opcion, 3);
    clearCmdWindow(pWin.pCmdWindow);
    int ID = atoi(opcion);
    int pos = BuscarTrenPorID(estaciones[miPos] , ID);

    if (pos == -1 || estaciones[miPos].tren[pos].migrado == 0)
    {
        return -1;
    }

    return pos;
}

int elegirEstDestino()
{
    char opcion[30];
    clearCmdWindow(pWin.pCmdWindow);
    wgetnstr(pWin.pCmdWindow, opcion, 29);
    clearCmdWindow(pWin.pCmdWindow);
    FormatearNombre(opcion);
    int pos = buscarEstacionPorNombre(opcion);

    if (pos == -1 || estaciones[pos].online == 0)
        return -1;

    return pos;
}

int registrarTren(ESTACION * estacion, char * mensaje)
{
    int i = BuscarPosVacia(estacion);
    if(i == -1)
    {
        return -1;
    }
    mensaje = mensaje + 4;
    
    //token ahora contiene el ID
    char * token = strtok(mensaje, ";");
    int ID = atoi(token);
    int yaExisteID = BuscarTrenPorID(*estacion, ID);
    if (yaExisteID != -1)
    {
    	return -1;
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
    return i;
}

int mensajeListadoEstDisp(char * mensaje)
{
    strcpy(mensaje, "Elija la estacion a la cual quiere viajar:\n\n");
    strcat(mensaje, "Las estaciones disponibles son:\n");
    int cont = 0;
    for(int i = 0; i < MAX_ESTACION; i++)
    {
        if (i != miPos)
        {
            if ( estacionConectada (estaciones[i].online))
            {
                cont ++;
                strcat(mensaje, estaciones[i].nombre);
                strcat(mensaje, "\n");
            }
        }
    }
    if (cont == 0)
    {
        strcpy(mensaje, "No hay estaciones disponibles");
    }
    return cont;
}

/* Busca a la estacion por el nombre
Devuelve la pos si la encuentrao -1 si no la encuentra*/
int buscarEstacionPorNombre(char * mensaje)
{
    int pos = -1;
    for(int i = 0; i < MAX_ESTACION; i++)
    {
        if (i != miPos)
        {
            if ( !strcmp(mensaje, estaciones[i].nombre ))
            {
                pos = i;
            }
        }
    }
    return pos;
}

void prepararEnvioTren(char *mensaje , int posTren)
{
    TREN tren = estaciones[miPos].tren[posTren];
    sprintf(mensaje, "2;3;%d;%d;%s", tren.ID, tren.combustible, tren.modelo);
}

int calcularTiempoDeViaje(int posEstacionDestino)
{
    int tiempo = 0;
    int direccion = miPos < posEstacionDestino ? 1 : -1;
    int i = miPos;
    while (i != posEstacionDestino)
    {
        tiempo += estaciones[i += direccion].distancia;
    }
    return tiempo;
}

void ConexionServer(void * argumento)
{
    char ** argv = (char **) argumento;
    char mensaje[sizeMsj];
    sprintf(mensaje,"../config/red/Red%d.conf", miPos + 1);

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
                memset(mensaje,'\0',sizeMsj);
                sprintf(mensaje,"Bienvenido a la estacion %s", estaciones[miPos].nombre);
                send(client[n], mensaje, sizeMsj, 0);
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
                                    if (regCorrecto == -1)
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
                                    sscanf(mensaje, "1;3;%d", &TrenID); //recibo el id del tren
                                    posTren = BuscarTrenPorID(estaciones[miPos], TrenID); //busco al tren

                                    int cantEstDisp = mensajeListadoEstDisp(mensaje); //armo un listado de las estaciones disponibles
                                    send(client[i], mensaje, sizeMsj, 0); // envio el listado

                                    if (cantEstDisp != 0)
                                    {
                                        printRegistro(&pWin,"Un tren quiere partir", WHITE);
                                        recv(client[i], mensaje, sizeMsj, 0); // recibo la estacion donde quiere ir
                                        FormatearNombre(mensaje);
                                        posEst = buscarEstacionPorNombre(mensaje);

                                        if( posEst >= 0 && estacionConectada(estaciones[posEst].online) )
                                        {
                                            strcpy(mensaje, "OK");
                                            send(client[i], mensaje, sizeMsj, 0);

                                            int tiempo = calcularTiempoDeViaje( posEst );
                                            estaciones[miPos].tren[posTren].combustible -= restarCombustible(tiempo);
                                            sprintf(mensaje, "%d", tiempo);
                                            send(client[i], mensaje, sizeMsj, 0);

                                            prepararEnvioTren(mensaje, posTren);
                                            send(serverEst[posEst], mensaje, sizeMsj, 0);
                                        }
                                        else //Si eligio una estacion que no es valida
                                        {
                                            strcpy(mensaje, "La estacion elegida no es valida, intente nuevamente.");
                                            send(client[i], mensaje, sizeMsj, 0);
                                        }
                                    }
                                    break;
                                /*   
                                case '4':
                                     Esto va a haber que cambiarlo, no borrar por ahora.

                                    estadoDelTren(estacion ,mensaje);
                                    puts("Estado enviado");
                                    send(clientTrenes[i], mensaje, strlen(mensaje), 0);
                                    
                                    break;*/
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
                                case '3': //Recibo un tren
                                    printRegistro(&pWin,"Ha llegado un nuevo tren", WHITE);
                                    posTren = registrarTren(&estaciones[miPos], mensaje);
                                    sprintf(mensaje, "./tren %d",estaciones[miPos].tren[posTren].ID);

                                    pid_t pid = fork();
                                    if (pid == 0)
                                    {
                                        strcpy(argv[0], mensaje);
                                        sprintf(mensaje, "%s        ",estaciones[miPos].nombre);
                                        strcpy(argv[1], mensaje);
                                        pause();
                                        _Exit(1);
                                    }
                                    else
                                    {
                                        estaciones[miPos].tren[posTren].migrado = pid;
                                    }

                                break;

                                case '5': //exit
                                    sscanf(mensaje, "2;5;%d", &posEst);
                                    estaciones[posEst].online = 0;
                                    printRegistro(&pWin,"Se desconecto una estacion", WHITE);
                                    FD_CLR(client[i], &master);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}