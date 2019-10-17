#include "../lib/est_commands.h"
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
    printLog(pAppWin, msg, WHITE);
    return ERR_OK;
}


ERROR clearLogWindow(WINDOW *pWin){
    werase(pWin);
    wrefresh(pWin);
    return ERR_OK;
}

/**
 * Limpia la ventana de comandos.
 * 
 * @param pWin ventana de comandos
 * @return ERR_OK
 */
ERROR clearCmdWindow(WINDOW *pWin){
    werase(pWin);
    return ERR_OK;
}