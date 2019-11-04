#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

/** 
 * Integrantes:
 * Alejandro Ugobono, 
 * Ezequiel Pirrota, 
 * Facundo Palacios,
 * Gonzalo Fernandez, 
 * Mauro Pignata, 
 * Mariano Wiñar.
 */

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

/**
 *Estructura de Datos para la interfaz grafica 
 */
typedef struct {
    WINDOW *pAppFrame;
    WINDOW *pLogFrame;
    WINDOW *pLogWindow;
    WINDOW *pCmdFrame;
    WINDOW *pCmdWindow;
   	WINDOW *pRegFrame;
    WINDOW *pRegWindow; 
} ST_APP_WINDOW;

/**
 *Variable global para la interfaz grafica 
 */
ST_APP_WINDOW pWin;

/**
 *Estructura del tipo enum para los colores a utilizar
 */
typedef enum {RED=1, GREEN, BLUE, WHITE, YELLOW, CYAN,} COLOUR;

/**
 * Funcion que:
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

/**
 * Funcion que Dibuja el Marco de la App, Ventana Log, Ventana Cmd, Ventana Reg,
 * @param ST_APP_WINDOW * puntero a estructura que contiene las ventanas
 */

void drawUserInterface(ST_APP_WINDOW *);

/**
 * Funcion que toma el Titulo a imprimir
 * @param * pWin Variable global para la interfaz grafica
 * @param * message es la Estacion y su numero como titulo
 */

void printWindowTitle(WINDOW *pWin, const char * message);

/**
 * Funcion que imprime en la pantalla tipo Log en la parte superior
 * @param * pWindow puntero a la estructura que contiene las ventanas
 * @param * message contiene el mensaje a imprimir
 * @param colour de la estructura tipo enum con los colores a utilizar
 */

void printLog(ST_APP_WINDOW *pWindow, const char * message, COLOUR colour);

/**
 * Funcion que imprime en la parte derecha de la ventana si se conecto el tren
 * Fucnion se utiliza en funcEstaciones.c
 * @param * pWindow puntero a la estructura que contiene las ventanas
 * @param * message contiene el mensaje a imprimir
 * @param COLOUR colour es de la estructura tipo enum con los colores a utilizar
 */

void printRegistro(ST_APP_WINDOW *pWindow, const char *message, COLOUR colour);

/**
 * Funcion que imprime el Estado completo del Tren con todas sus variables
 * @param * pWin Variable global para la interfaz grafica
 * @param TREN trenes es el vector de trenes
 * @return un entero
 */

int printEstadoTrenes(ST_APP_WINDOW *pWin , TREN trenes[]);

/**
 * Funcion que inicia el usuario de la interfaz grafica
 * @param * variable global para la interfaz grafica
 */

void unInitUserInterface(ST_APP_WINDOW *);

/**
 * Funcion que imprime el Estado completo de la Estacion
 * @param * pWin Variable global para la interfaz grafica
 * @param ESTACION est es el vector de estaciones
 */

void printEstadoEstaciones(ST_APP_WINDOW *pWin, ESTACION est[]);

/**
 * Funcion que imprime la Ayuda del programa
 * @param * pAppWin representa la Variable global para la interfaz grafica
 */

void printHelp(ST_APP_WINDOW *pAppWin);

/**
 * Funcion que limpia la ventana de comandos y hace un refresco de pantalla
 * @param * pWin variable global, ventana de comandos
 * @return ERR_OK
 */

void clearWindow(WINDOW *pWin);

/**
 * Funcion que limpia la ventana de comandos
 * @param * pWin variable global, ventana de comandos
 * @return ERR_OK
 */

void clearCmdWindow(WINDOW *pWin);

/** 
 * Funcion para el hilo que se encarga de la interfaz grafica
 * @param sin parametros
 */

void InterfazGrafica();

#endif
