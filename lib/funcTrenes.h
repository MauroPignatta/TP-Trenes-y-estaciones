#ifndef FUNCTRENES_H
#define FUNCTRENES_H

#define max_est 30

typedef struct{
    int ID;
    int combustible;
    char modelo[10];
    char estOrigen[max_est];
    char estDestino[max_est];
    int tiempoRestante;
}TREN;

void cargarCombustible(int *combustible);

void solicitarPasoAnden();

void registrarse(char *mensaje,TREN tren);

void partir();

TREN inicializarTren(char *arch);

void armarMensajeEstadoDelTren(TREN t, char * mensaje);


#endif	// FUNCTRENES_H

