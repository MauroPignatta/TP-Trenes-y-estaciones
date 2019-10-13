/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "user_commands.h"
#include <string.h>

#define HELP_MSG_LENGHT 255

ERROR printHelp(ST_APP_WINDOW *pAppWin){
    char msg[HELP_MSG_LENGHT+1];
    memset(msg, '\0', HELP_MSG_LENGHT+1);
    strncpy(msg, "Ejemplo de uso de la biblioteca ncurses\n", 41);
    strncat(msg, "Comandos permitidos:\n", 22);
    strncat(msg, "\t * exit: permite salir de la aplicación\n", 43);
    strncat(msg, "\t * clear: limpia ventana de Mensajes\n", 40);
    strncat(msg, "\t * copy: copia el texto a la ventana de Mensajes", 50);
    printMessage(pAppWin, msg, GREEN);
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
    int maxRow = 0;
    int maxCol = 0;
    int row = 0;
    int col = 0;
    getyx(pWin, row, col);
    getmaxyx(pWin, maxRow, maxCol);
    if(row == maxRow-1){
        werase(pWin);
    }
    
    return ERR_OK;
}

ERROR copyCommand(ST_APP_WINDOW *pWin, const char *cmdLine){
    char line[CMD_LINE_LENGHT + 1];
    memset(line, '\0', CMD_LINE_LENGHT + 1);
    strncpy(line, cmdLine+5, CMD_LINE_LENGHT + 1);
    printMessage(pWin, line, GREEN);
    return ERR_OK;
}

ERROR processCommand(ST_APP_WINDOW *pAppWin, const char *commandLine){
    if(strncmp(commandLine, "help", 5)==0){
        printHelp(pAppWin);
    }
    
    if(strncmp(commandLine, "clear", 6)==0){
        clearLogWindow(pAppWin->pLogWindow);
    }
    
    if(strncmp(commandLine, "copy", 4)==0){
        copyCommand(pAppWin, commandLine);
    }
    
    clearCmdWindow(pAppWin->pCmdWindow);
    return ERR_OK;
}

