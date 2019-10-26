#ifndef FUNCTRENES_H
#define FUNCTRENES_H

#define max_nombre_est 30

#define esTren(x) x == '1'

typedef struct{
    int ID;
    int combustible;
    char modelo[10];
    char estOrigen[max_nombre_est];
    char estDestino[max_nombre_est];
    int tiempoRestante;
    int nCliente;
    int registrado;
}TREN;

void cargarCombustible(int *combustible);

void solicitarPasoAnden();

void partir();

TREN inicializarTren(char *arch);

void armarMensajeEstadoDelTren(TREN t, char * mensaje);

void armarMensajeRegistrarse(TREN tren,char *mensaje);

void armarMensajeExit(TREN tren, char * mensaje);

#endif	// FUNCTRENES_H

