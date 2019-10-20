#include "../lib/est_interface.h"
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
    scrollok(pWindow->pLogWindow, TRUE);
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
    mvwprintw(pWindow->pLogWindow, y-1 , 0 , "Escriba \"help\" para obtener informacion.");
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

void printEstadoTrenes(ST_APP_WINDOW *pWin , TREN trenes[], int cantTrenes)
{
    int j = 0;
    char command[6] = " ";

    int posTrenes[cantTrenes];
    buscarTrenes(trenes , posTrenes);

    int y = getmaxy(pWin->pLogWindow);

    while(strcmp(command, "back"))
    {
        if (j < cantTrenes)
        {
            wprintw(pWin->pLogWindow,"Estado: Tren %d\n\n", j + 1);
            wprintw(pWin->pLogWindow,"ID: %d\n",trenes[posTrenes[j]].ID);
            wprintw(pWin->pLogWindow,"Combustible restante: %d\n",trenes[posTrenes[j]].combustible);
            wprintw(pWin->pLogWindow,"Modelo: %s\n",trenes[posTrenes[j]].modelo);
            wprintw(pWin->pLogWindow,"Estacion Actual: %s\n",trenes[posTrenes[j]].estOrigen);
            wprintw(pWin->pLogWindow,"Estacion Destino: %s\n",trenes[posTrenes[j]].estDestino);
            wprintw(pWin->pLogWindow,"Tiempo de viaje restante: %d\n\n",trenes[posTrenes[j]].tiempoRestante);
            wprintw(pWin->pLogWindow,"<- ant\t\t Pagina %d/%d \t\tsig ->\n",j + 1, cantTrenes);
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
            werase(pWin->pLogWindow);
            werase(pWin->pCmdWindow);
        }
    }
}


void unInitUserInterface(ST_APP_WINDOW *pWindow){
    delwin(pWindow->pLogWindow);
    delwin(pWindow->pCmdWindow);
    clear();
    endwin();
}

