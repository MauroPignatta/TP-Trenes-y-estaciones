#include "../lib/est_commands.h"
#include <string.h>

#define HELP_MSG_LENGHT 1024

ERROR printHelp(ST_APP_WINDOW *pAppWin){
    char msg[HELP_MSG_LENGHT+1];
    memset(msg, '\0', HELP_MSG_LENGHT+1);
    strncpy(msg, "Comandos de la Estacion\n", 26);
    strncat(msg, "Comandos permitidos:\n\n", 25);
    strncat(msg, "* estado tren: Muestra estado de trenes.\n", 43);
    strncat(msg, "* estado est: Muestra estado de estaciones.\n", 46);
    strncat(msg, "* buscar est: Buscar y conecta estaciones.\n", 45);
    strncat(msg, "* clearlog: Limpia la pantalla de log.\n", 41);
    strncat(msg, "* clearreg: Limpia la pantalla de registros.\n", 45);
    strncat(msg, "* exit: Sale de la aplicacion.", 31);
    printLog(pAppWin, msg, WHITE);
    return ERR_OK;
}


ERROR clearWindow(WINDOW *pWin){
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