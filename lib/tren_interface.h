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

#include <curses.h>
#include <unistd.h>
#include "funcTrenes.h"
#include "Conexion.h"

#define LINE_LENGTH 80
#define VIEW_WIN_WIDTH 0
#define VIEW_WIN_HEIGTH 30
#define MSG_WIN_WIDTH 0
#define MSG_WIN_HEIGTH 50
#define SPACE 1

/**
 * Estructura de datos para la variable global
 */
typedef struct {
    WINDOW *pAppFrame;
    WINDOW *pLogFrame;
    WINDOW *pLogWindow;
    WINDOW *pCmdFrame;
    WINDOW *pCmdWindow;
} ST_APP_WINDOW;

/**
 *Estructura del tipo enum para los colores a utilizar
 */
typedef enum {RED = 1, GREEN, BLUE, WHITE, YELLOW, CYAN,} COLOUR;

/**
 * Funcion que Crea las ventanas de la app. Asocia colores con las ventanas
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
 * Funcion que imprime en la pantalla tipo Log en la parte superior
 * @param * pWindow puntero a la estructura que contiene las ventanas
 * @param * message contiene el mensaje a imprimir
 * @param colour de la estructura tipo enum con los colores a utilizar
 */

void printWindowTitle(WINDOW *pWin, const char * message);

/**
 * Funcion Imprime mensaje
 * @param * pWindow puntero a la estructura que contiene las ventanas
 * @param * message contiene el mensaje a imprimir
 * @param colour de la estructura tipo enum con los colores a utilizar
 */

void printMessage(ST_APP_WINDOW *pWindow, const char * message, COLOUR colour);

/**
 * Funcion que inicia el usuario de la interfaz grafica
 * @param * variable global para la interfaz grafica
 */

void unInitUserInterface(ST_APP_WINDOW *);

/**
 * Funcion que limpia la ventana de comandos
 * @param * pWin variable global, ventana de comandos
 * @return ERR_OK
 */

void clearCmdWindow(WINDOW *pWin);

/**
 * Funcion que limpia la ventana de comandos y hace un refresco de pantalla
 * Limpia la pantalla del Log
 * @param * pWin variable global, ventana de comandos
 * @return ERR_OK
 */

void clearLogWindow(WINDOW *pWin);

/**
 * Funcion que imprime la Ayuda del programa
 * @param * pAppWin representa la Variable global para la interfaz grafica
 */

void printHelp(ST_APP_WINDOW *pAppWin);

/**
 * Funcion que imprime un mensaje que se le a asignado el anden al tren
 * @param * pAppWin representa la Variable global para la interfaz grafica
 */
void imprimirAndenAsignado(ST_APP_WINDOW *pWin);

/**
 * Funcion Muestra el Tren viajando
 * @param * pLogWindow representa la Variable global
 * @param * puntero al tiempo restante del tren para actualizarlo.
 */
void DibujarTrenViajando(WINDOW *pLogWindow, int * tiempoRestante);

/**
 * Funcion Salir del programa
 * @param TREN tren estructura de datos tren
 * @param int client numero de cliente servidor
 * @param * pAppWin representa la Variable global para la interfaz grafica
 */

void salirDelPrograma(TREN tren, int client, ST_APP_WINDOW * pWin);

/**
 * Funcion para el hilo que se encarga de la interfaz grafica
 * @param * argumentos
 */

void InterfazGrafica(void * argumentos);

#endif
