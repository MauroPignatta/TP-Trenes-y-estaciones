/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   user_commands.h
 * Author: ezequiel
 *
 * Created on 26 de septiembre de 2019, 08:54
 */

#ifndef USER_COMMANDS_H
#define USER_COMMANDS_H

#include "user_interface.h"
    
#define CMD_LINE_LENGHT 50
    
typedef enum {ERR_OK=0, ERR_UNKNOWN_CMD} ERROR;
    
ERROR processCommand(ST_APP_WINDOW *pWin, const char *commandLine);

#endif /* COMMANDS_H */





#ifdef __cplusplus
}
#endif



