#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "funcEstaciones.h"
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_LENGTH 80
#define VIEW_WIN_WIDTH 0
#define VIEW_WIN_HEIGTH 30
#define MSG_WIN_WIDTH 0
#define MSG_WIN_HEIGTH 50
#define SPACE 1

typedef struct {
    WINDOW *pAppFrame;
    WINDOW *pLogFrame;
    WINDOW *pLogWindow;
    WINDOW *pCmdFrame;
    WINDOW *pCmdWindow;
   	WINDOW *pRegFrame;
    WINDOW *pRegWindow; 
} ST_APP_WINDOW;

/* Variable global para la interfaz grafica */
ST_APP_WINDOW pWin;

typedef enum {RED=1, GREEN, BLUE, WHITE,} COLOUR;

/**
 * Crea las ventanas de la app. Asocia colores con las ventanas
 * Crea en pantalla un grupo de ventanas con la siguiente estructura:
 * Log: muestra mensajes
 * Cmd: permite el ingreso de comnandos al usuario.
 * 
 * |---------------|
 * ||-----Log-----||
 * ||             ||
 * ||             ||
 * ||             ||
 * ||             ||
 * ||-------------||
 * |               |
 * ||-----Cmd-----||
 * ||             ||
 * ||-------------||
 * |---------------|
 * 
 * @param ST_APP_WINDOW * puntero a estructura que contiene las ventanas
 */
void initUserInterface(ST_APP_WINDOW *);

void drawUserInterface(ST_APP_WINDOW *);

void printWindowTitle(WINDOW *pWin, const char * message);

void printLog(ST_APP_WINDOW *pWindow, const char * message, COLOUR colour);

void printRegistro(ST_APP_WINDOW *pWindow, const char *message, COLOUR colour);

int printEstadoTrenes(ST_APP_WINDOW *pWin , TREN trenes[]);

void unInitUserInterface(ST_APP_WINDOW *);

void printEstadoEstaciones(ST_APP_WINDOW *pWin, ESTACION est[]);

void printHelp(ST_APP_WINDOW *pAppWin);

void clearWindow(WINDOW *pWin);

void clearCmdWindow(WINDOW *pWin);

/* Funcion para el hilo que se encarga de la interfaz grafica */
void InterfazGrafica();

#endif
