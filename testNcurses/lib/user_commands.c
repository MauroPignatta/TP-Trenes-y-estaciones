#include "user_commands.h"
#include <string.h>

#define HELP_MSG_LENGHT 1024

ERROR printHelp(ST_APP_WINDOW *pAppWin){
    char msg[HELP_MSG_LENGHT+1];
    memset(msg, '\0', HELP_MSG_LENGHT+1);
    strncpy(msg, "Comandos del Tren\n", 20);
    strncat(msg, "Comandos permitidos:\n\n", 24);
    strncat(msg, "* registrarse: Registra al tren en la estacion.\n", 53);
    strncat(msg, "* anden: Solicita el anden\n", 40);
    strncat(msg, "* partir: Viaja a la nueva estacion.\n", 42);
    strncat(msg, "* estado: Muestra el estado del tren.", 41);
    printMessage(pAppWin, msg, WHITE);
    return ERR_OK;
}


ERROR clearLogWindow(WINDOW *pWin){
    werase(pWin);
    wrefresh(pWin);
    return ERR_OK;
}

/**
 * Limpia la ventana de comandos cuando se llena.
 * 
 * @param pWin ventana de comandos
 * @return ERR_OK
 */
ERROR clearCmdWindow(WINDOW *pWin){
    werase(pWin);
    return ERR_OK;
}

ERROR copyCommand(ST_APP_WINDOW *pWin, const char *cmdLine){
    char line[CMD_LINE_LENGHT + 1];
    memset(line, '\0', CMD_LINE_LENGHT + 1);
    strncpy(line, cmdLine+5, CMD_LINE_LENGHT + 1);
    printMessage(pWin, line, GREEN);
    return ERR_OK;
}

void trencitoViajando(WINDOW *pLogWindow)
{
    int y = getmaxy(pLogWindow)/ 2 -3;
    int x = getmaxx(pLogWindow)/ 2 - 30;
    int fixX = x - 15;
    for(int i = 0 ; i < 20; i ++)
    {
        x--;
        mvwprintw(pLogWindow, y    , x, "           o x o x o x o . . .\n");
        mvwprintw(pLogWindow, y + 1, x, "         o      _____            _______________ ___=====__T___\n");
        mvwprintw(pLogWindow, y + 2, x, "       .][__n_n_|DD[  ====_____  |    |.\\/.|   | |   |_|     |_\n");
        mvwprintw(pLogWindow, y + 3, x, "      >(________|__|_[_________]_|____|_/\\_|___|_|___________|_|\n");
        mvwprintw(pLogWindow, y + 4, x, "      _/oo OOOOO oo`  ooo   ooo   o^o       o^o   o^o     o^o\n");
        mvwprintw(pLogWindow, y + 5, fixX, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        wrefresh(pLogWindow);
        sleep(1);
        werase(pLogWindow);
    }
    wrefresh(pLogWindow);
}

ERROR processCommand(ST_APP_WINDOW *pAppWin, const char *commandLine){
    if(strncmp(commandLine, "help", 5)==0){
        printHelp(pAppWin);
    }
    
    if(strncmp(commandLine, "clear", 6)==0){
        clearLogWindow(pAppWin->pLogWindow);
    }
    
    if(strncmp(commandLine, "copy", 5)==0){
        copyCommand(pAppWin, commandLine);
    }
    if(strncmp(commandLine, "partir", 7)==0){
        trencitoViajando(pAppWin->pLogWindow);
    }
    clearCmdWindow(pAppWin->pCmdWindow);
    return ERR_OK;
}