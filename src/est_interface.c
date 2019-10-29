#include "../lib/est_interface.h"
#include "../lib/Conexion.h"
#include "../lib/funcEstaciones.h"
#include <string.h>

void initUserInterface(ST_APP_WINDOW *pWindow)
{
    initscr();
    clear();

    if(has_colors()){
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
    }

    // Marco general de la App. Ocupa toda la pantalla
    pWindow->pAppFrame = newwin(0, 0, 0, 0);
    
    // obtiene alto y ancho del marco (dependiente de la resolución)
    int frameWidth=0;
    int frameHeight=0;
    getmaxyx(pWindow->pAppFrame, frameHeight, frameWidth);
       
    // calcula ancho y alto de las ventanas internas
    int msgWinWidth = (frameWidth * 0.60); //0.985)
    int msgWinHeigth = (frameHeight * 0.6);

    int regWinWidth = (frameWidth * 0.385);
    int regWinHeigth = (frameHeight * 0.6);
    
    int viewWinWidth = (frameWidth * 0.985);
    int viewWinHeigth = (frameHeight * 0.37);
    
    // crea la ventana interna de Log
    pWindow->pLogFrame = derwin(pWindow->pAppFrame, msgWinHeigth, msgWinWidth, 1, 1);
    pWindow->pLogWindow = derwin(pWindow->pLogFrame, msgWinHeigth-2, msgWinWidth-2, 1, 1);
    
    // crea la ventana interna de Cmd
    pWindow->pCmdFrame = derwin(pWindow->pAppFrame, viewWinHeigth, viewWinWidth, msgWinHeigth + SPACE, 1);
    pWindow->pCmdWindow = derwin(pWindow->pCmdFrame, viewWinHeigth-2, viewWinWidth-2, 1, 1);

    //crea la ventana interde de Reg
    pWindow->pRegFrame = derwin(pWindow->pAppFrame, regWinHeigth, regWinWidth, 1, msgWinWidth + 1);
    pWindow->pRegWindow = derwin(pWindow->pRegFrame, regWinHeigth-2, regWinWidth-2, 1, 1);
    
    // asocia colores con las ventanas
    wbkgd(pWindow->pAppFrame, COLOR_PAIR(RED));
    wbkgd(pWindow->pRegFrame, COLOR_PAIR(WHITE));
    wbkgd(pWindow->pLogFrame, COLOR_PAIR(WHITE));
    wbkgd(pWindow->pCmdFrame, COLOR_PAIR(WHITE));
    
    // activa el scroll en la ventana de Log
    scrollok(pWindow->pRegWindow, TRUE);
    cbreak();
}

void drawUserInterface(ST_APP_WINDOW *pWindow){
    refresh();
   
    // Dibuja el marco de la App
    box(pWindow->pAppFrame, ACS_VLINE, ACS_HLINE);
    wrefresh(pWindow->pAppFrame);
    
    // Dibuja el marco de la ventana de Log
    box(pWindow->pLogFrame, ACS_VLINE, ACS_HLINE);
    wrefresh(pWindow->pLogFrame);
    
    // Dibuja el marco de la ventana de Cmd
    box(pWindow->pCmdFrame, ACS_VLINE, ACS_HLINE);
    wrefresh(pWindow->pCmdFrame);

    // Dibuja el marco de la ventana de Reg
    box(pWindow->pRegFrame, ACS_VLINE, ACS_HLINE);
    wrefresh(pWindow->pRegFrame);
}

void clearWindow(WINDOW *pWin){
    werase(pWin);
    wrefresh(pWin);
}

/**
 * Limpia la ventana de comandos.
 * 
 * @param pWin ventana de comandos
 * @return ERR_OK
 */
void clearCmdWindow(WINDOW *pWin){
    werase(pWin);
}

void printHelp(ST_APP_WINDOW *pAppWin){
    char msg[1024];
    memset(msg, '\0', 1024);
    strncpy(msg, "Comandos de la Estacion\n", 26);
    strncat(msg, "Comandos permitidos:\n\n", 25);
    strncat(msg, "* estado tren: Muestra estado de trenes.\n", 43);
    strncat(msg, "* estado est: Muestra estado de estaciones.\n", 46);
    strncat(msg, "* buscar est: Buscar y conecta estaciones.\n", 45);
    strncat(msg, "* partir tren: Permite migrar un tren.\n", 41);
    strncat(msg, "* clearlog: Limpia la pantalla de log.\n", 41);
    strncat(msg, "* clearreg: Limpia la pantalla de registros.\n", 45);
    strncat(msg, "* exit: Sale de la aplicacion.", 31);
    printLog(pAppWin, msg, WHITE);
}

void printWindowTitle(WINDOW *pWin, const char * message){
    int cols = getmaxx(pWin);
    int x = (cols-strlen(message))/2;
    mvwprintw(pWin, 0, x, message);
    wrefresh(pWin);
}

void printLog(ST_APP_WINDOW *pWindow, const char *message, COLOUR colour){
    wattron(pWindow->pLogWindow, COLOR_PAIR(colour));
    wprintw(pWindow->pLogWindow, "%s\n", message);
    wattroff(pWindow->pLogWindow, COLOR_PAIR(colour));

    int y = getmaxy(pWindow->pLogWindow);
    //mvwprintw(pWindow->pLogWindow, y-1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWindow->pLogWindow);
    wrefresh(pWindow->pCmdWindow);
}

void printRegistro(ST_APP_WINDOW *pWindow, const char *message, COLOUR colour){
    wattron(pWindow->pRegWindow, COLOR_PAIR(colour));
    wprintw(pWindow->pRegWindow, "%s\n", message);
    wattroff(pWindow->pRegWindow, COLOR_PAIR(colour));

    wrefresh(pWindow->pRegWindow);
    wrefresh(pWindow->pCmdWindow);
}

int printEstadoTrenes(ST_APP_WINDOW *pWin , TREN trenes[])
{
    int j = 0;
    char command[6] = " ";
    int posTrenes[MAX_TREN];
    int cantTrenes = buscarTrenes(trenes , posTrenes);

    if (cantTrenes == 0)
    {
        return 0;
    }

    int y = getmaxy(pWin->pLogWindow);

    while(strcmp(command, "back"))
    {
        if (j < cantTrenes)
        {
            werase(pWin->pLogWindow);
            mvwprintw(pWin->pLogWindow, 0, 0,"Estado: Tren %d\n\n", j + 1);
            wprintw(pWin->pLogWindow,"ID: %d\n",trenes[posTrenes[j]].ID);
            wprintw(pWin->pLogWindow,"Combustible restante: %d\n",trenes[posTrenes[j]].combustible);
            wprintw(pWin->pLogWindow,"Modelo: %s\n",trenes[posTrenes[j]].modelo);
            wprintw(pWin->pLogWindow,"Estacion Actual: %s\n",trenes[posTrenes[j]].estOrigen);
            wprintw(pWin->pLogWindow,"Estacion Destino: %s\n",trenes[posTrenes[j]].estDestino);
            wprintw(pWin->pLogWindow,"Tiempo de viaje restante: %d\n\n",trenes[posTrenes[j]].tiempoRestante);
            mvwprintw(pWin->pLogWindow, y-2 , 0,"<- ant\t\t Pagina %d/%d \t\tsig ->\n",j + 1, cantTrenes);
            mvwprintw(pWin->pLogWindow, y-1 , 0, "Escriba \"back\" para volver.");
            wrefresh(pWin->pLogWindow);

            wgetnstr(pWin->pCmdWindow, command, 5);
            wrefresh(pWin->pCmdWindow);

            if (!strcmp(command, "ant"))
            {
                if (j > 0)
                {
                    j--;
                }
            }

            else if (!strcmp(command, "sig"))
            {
                if( j < cantTrenes - 1)
                {
                    j++;
                }
            }
            werase(pWin->pCmdWindow);
        }
    }
    werase(pWin->pLogWindow);
    werase(pWin->pCmdWindow);
    printLog(pWin, "", WHITE);
    return cantTrenes;
}

void printEstadoEstaciones(ST_APP_WINDOW *pWin, ESTACION est[])
{
    int i = 0;
    char command[6] = " ";
    char EstConectada[6];
    int y = getmaxy(pWin->pLogWindow);

    while (strcmp(command, "back"))
    {
        if (est[i].online == 2)
            strcpy(EstConectada, "Local");
        else if(est[i].online == 1)
            strcpy(EstConectada, "Si");
        else if(est[i].online == 0)
            strcpy(EstConectada, "No");

        werase(pWin->pLogWindow);
        wprintw(pWin->pLogWindow,"Estado estaciones: \n\n");
        wprintw(pWin->pLogWindow,"Nombre: %s\n",est[i].nombre);
        wprintw(pWin->pLogWindow,"ID: %d\n",est[i].ID);
        wprintw(pWin->pLogWindow,"Distancia: %d KM\n",est[i].distancia);
        wprintw(pWin->pLogWindow,"Conectada: %s\n", EstConectada);
        mvwprintw(pWin->pLogWindow, y-2 , 0,"<- ant\t\t Pagina %d/%d \t\tsig ->\n",i + 1, MAX_ESTACION);
        mvwprintw(pWin->pLogWindow, y-1 , 0, "Escriba \"back\" para volver.");
        wrefresh(pWin->pLogWindow);

        wgetnstr(pWin->pCmdWindow, command, 5);
        wrefresh(pWin->pCmdWindow);

        if (!strcmp(command, "ant"))
        {
            if (i > 0)
            {
                i--;
            }
        }

        else if (!strcmp(command, "sig"))
        {
            if( i < MAX_ESTACION - 1)
            {
                i++;
            }
        }
        werase(pWin->pCmdWindow);
    }
    werase(pWin->pLogWindow);
    werase(pWin->pCmdWindow);
    printLog(pWin, "", WHITE);
}

void unInitUserInterface(ST_APP_WINDOW *pWindow){
    delwin(pWindow->pLogWindow);
    delwin(pWindow->pCmdWindow);
    clear();
    endwin();
}
               

void InterfazGrafica()
{
     //Aca empieza a correr ncurses
    initUserInterface(&pWin);
    drawUserInterface(&pWin);
    mvwprintw(pWin.pLogWindow, getmaxy(pWin.pLogWindow) -1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWin.pLogWindow);

    char mensaje[sizeMsj];

    sprintf(mensaje, " Estacion %s " , estaciones[miPos].nombre);
    
    printWindowTitle(pWin.pAppFrame, mensaje);
    printWindowTitle(pWin.pLogFrame, "### Log ###");
    printWindowTitle(pWin.pRegFrame, "### Registro ###");
    printWindowTitle(pWin.pCmdFrame, "### Comandos ###");

    char comandos[20];
    
    while(1)
    {
        memset(mensaje ,'\0', sizeMsj);
        wgetnstr(pWin.pCmdWindow, comandos, 20);
        clearCmdWindow(pWin.pCmdWindow);

        if (!strcmp(comandos, "help"))
        {
            clearWindow(pWin.pLogWindow);
            printHelp(&pWin);
        }

        else if (!strcmp(comandos, "estado tren"))
        {
            if (printEstadoTrenes(&pWin, estaciones[miPos].tren) == 0)
            {
                clearWindow(pWin.pLogWindow);
                printLog(&pWin,"No hay trenes registrados en la estacion.", WHITE);
            }
        }

        else if (!strcmp(comandos, "estado est"))
        {
            printEstadoEstaciones(&pWin, estaciones);
        }

        else if (!strcmp(comandos, "buscar est"))
        {
            clearWindow(pWin.pLogWindow);
            char nombreArchivo[25];
            printLog(&pWin, "Buscando estaciones...", WHITE);
            int cont = 0;

            char mensajeIdEst[6];
            sprintf(mensajeIdEst, "2;%d", miPos);

            for(int i  = 0; i < MAX_ESTACION; i ++)
            {
                if (i != miPos)
                {
                    sprintf(nombreArchivo, "../config/red/Red%d.conf", i + 1);
                    if (serverEst[i] = conectarEstacion(nombreArchivo))
                    {
                        cont ++;
                        send(serverEst[i], mensajeIdEst, sizeMsj, 0);
                        estaciones[i].online = 1;
                        strcat(mensaje, estaciones[i].nombre);
                        strcat(mensaje, "\n");
                    }
                }
            }
            clearWindow(pWin.pLogWindow);
            if (cont == 0)
                printLog(&pWin, "No se encontraron estaciones", WHITE);
            else
            {
                printLog(&pWin, "Se encontraron estaciones", WHITE);
                printLog(&pWin, "Las estaciones disponibles son: \n", WHITE);      
            	printLog(&pWin, mensaje, WHITE);   	
            }

        }


        else if (!strcmp(comandos, "clearlog"))
        {
            clearWindow(pWin.pLogWindow);
            printLog(&pWin,"", WHITE);
        }

        else if (!strcmp(comandos, "clearreg"))
        {
            clearWindow(pWin.pRegWindow);
        }

        else if (!strcmp(comandos, "exit"))
        {
            clearWindow(pWin.pLogWindow);
            printLog(&pWin, "Saliendo...", WHITE);
            sprintf(mensaje, "2;5;%d", miPos);
            for(int i = 0; i < MAX_ESTACION; i++)
            {
                if (i != miPos)
                    send(serverEst[i], mensaje, sizeMsj, 0);
            }
            unInitUserInterface(&pWin);
            exit(EXIT_SUCCESS);
        }

/*
        else if (!strcmp(comandos, "partir tren"))
        {

            clearLogWindow(pWin.pLogWindow);                
            int trenTime=0;
            char solicitud[50];
            send(client,solicitud,50,0);
            recv(client,solicitud,50,0);
            trenTime=atoi(solicitud);

            printLog(&pWin, "A dónde desea viajar?", WHITE);
            wgetnstr(pWin.pCmdWindow, solicitud, 20);
            for(int i  = 0; i < MAX_ESTACION; i ++)
            {
                if (i != miPos)
                {
                    if (strcmp(solicitud,estaciones[i].nombre) && estaciones[i].online == 1)
                    {
                        trenTime+=estaciones[i].distancia;

                    }
                }
            }
            send(client,atoi(trenTime),strlen(solicitud),0);
            //char solicitud[sizeMsj]="3;";
            
            //send(serverEst[i], "2", sizeMsj, 0);
            
            //no borrar por las dudas
            send(client,solicitud,strlen(solicitud),0);
            recv(client,solicitud,sizeMsj,0);
            tiempoRestante=atoi(solicitud);*/

            //esto de abajo va en algun lugar de tren
            /*partir(&tren);
            trencitoViajando(pWin.pLogWindow);
            printMessage(&pWin, "", WHITE);
            break;
        }
*/
        else
        {
            clearWindow(pWin.pLogWindow);
            strcat(comandos, " no es un comando valido.");
            printLog(&pWin, comandos, WHITE);
        }
    }
}