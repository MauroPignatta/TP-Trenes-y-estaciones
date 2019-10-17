/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   commands.h
 * Author: alejandro
 *
 * Created on September 8, 2019, 2:33 PM
 */

#ifndef USER_COMMANDS_H
#define USER_COMMANDS_H

#include "user_interface.h"
 #include <unistd.h>
    
#define CMD_LINE_LENGHT 500
    
typedef enum {ERR_OK=0, ERR_UNKNOWN_CMD} ERROR;
    
ERROR processCommand(ST_APP_WINDOW *pWin, const char *commandLine);

#endif /* COMMANDS_H */

