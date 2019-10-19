#include <stdio.h>
#include <stdlib.h>
typedef struct{
	int age;
	char name[30];
	int type;
	char pregnant;
}DATOS_CLIENT;
typedef struct nodo{
	DATOS_CLIENT data;
	nodo *next;
}STR_NODO;
typedef struct{
	STR_NODO *frente;
	STR_NODO *fin;
}STR_COLA;
