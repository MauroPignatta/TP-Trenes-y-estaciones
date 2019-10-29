#include "../lib/tren_interface.h"
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
    int msgWinWidth = (frameWidth * 0.985);
    int msgWinHeigth = (frameHeight * 0.6);
    
    int viewWinWidth = (frameWidth * 0.985);
    int viewWinHeigth = (frameHeight * 0.37);
    
    // crea la ventana interna de Log
    pWindow->pLogFrame = derwin(pWindow->pAppFrame, msgWinHeigth, msgWinWidth, 1, 1);
    pWindow->pLogWindow = derwin(pWindow->pLogFrame, msgWinHeigth-2, msgWinWidth-2, 1, 1);
    
    // crea la ventana interna de Cmd
    pWindow->pCmdFrame = derwin(pWindow->pAppFrame, viewWinHeigth, viewWinWidth, msgWinHeigth + SPACE, 1);
    pWindow->pCmdWindow = derwin(pWindow->pCmdFrame, viewWinHeigth-2, viewWinWidth-2, 1, 1);
    
    // asocia colores con las ventanas
    wbkgd(pWindow->pAppFrame, COLOR_PAIR(GREEN));
    wbkgd(pWindow->pLogFrame, COLOR_PAIR(WHITE));
    wbkgd(pWindow->pCmdFrame, COLOR_PAIR(WHITE));
    
    // activa el scroll en la ventana de Log
    scrollok(pWindow->pLogWindow, TRUE);
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
}

void printHelp(ST_APP_WINDOW *pAppWin){
    char msg[1024];
    memset(msg, '\0', 1024);
    strncpy(msg, "Comandos del Tren\n", 20);
    strncat(msg, "Comandos permitidos:\n\n", 24);
    strncat(msg, "* registrarse: Registra al tren en la estacion.\n", 53);
    strncat(msg, "* anden: Solicita el anden\n", 40);
    strncat(msg, "* partir: Viaja a la nueva estacion.\n", 42);
    strncat(msg, "* estado: Muestra el estado del tren.\n", 40);
    strncat(msg, "* exit: Sale de la apps", 23);
    printMessage(pAppWin, msg, WHITE);
}


void clearLogWindow(WINDOW *pWin){
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

void DibujarTrenViajando(WINDOW *pLogWindow, int * tiempoRestante)
{
    int y = getmaxy(pLogWindow)/ 2;
    int x = 18;
    int fixX = 0;
    while(*tiempoRestante > 0)
    {
        mvwprintw(pLogWindow, 0 , 0, "Viajando...\nTiempo Restante: %d", * tiempoRestante);
        mvwprintw(pLogWindow, y    , x, "    o x o x o x o . . .\n");
        mvwprintw(pLogWindow, y + 1, x, "   o      _____            _______________ ___=====__T___\n");
        mvwprintw(pLogWindow, y + 2, x, " .][__n_n_|DD[  ====_____  |    |.\\/.|   | |   |_|     |_\n");
        mvwprintw(pLogWindow, y + 3, x, ">(________|__|_[_________]_|____|_/\\_|___|_|___________|_|\n");
        mvwprintw(pLogWindow, y + 4, x, "_/oo OOOOO oo`  ooo   ooo   o^o       o^o   o^o     o^o\n");
        mvwprintw(pLogWindow, y + 5, fixX, "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        wrefresh(pLogWindow);
        x--;
        (*tiempoRestante) -- ;
        usleep(350000);
        werase(pLogWindow);
    }
    wrefresh(pLogWindow);
}

void printWindowTitle(WINDOW *pWin, const char * message){
    int cols = getmaxx(pWin);
    int x = (cols-strlen(message))/2;
    mvwprintw(pWin, 0, x, message);
    wrefresh(pWin);
}

void printMessage(ST_APP_WINDOW *pWindow, const char *message, COLOUR colour){
    int y = getmaxy(pWindow->pLogWindow);
    wattron(pWindow->pLogWindow, COLOR_PAIR(colour));
    wprintw(pWindow->pLogWindow, "%s\n", message);
    wattroff(pWindow->pLogWindow, COLOR_PAIR(colour));
    mvwprintw(pWindow->pLogWindow, y-1 , 0 , "Escriba \"help\" para obtener informacion.");
    wrefresh(pWindow->pLogWindow);
}

void unInitUserInterface(ST_APP_WINDOW *pWindow){
    delwin(pWindow->pLogWindow);
    delwin(pWindow->pCmdWindow);
    clear();
    endwin();
}
