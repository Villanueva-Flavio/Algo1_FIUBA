#ifndef __PAPELEO_H__
#define __PAPELEO_H__

#include "utiles.h"
#include <stdbool.h>

#define MAX_OBSTACULOS 100
#define MAX_HERRAMIENTAS 100
#define MAX_PAPELEOS 10
#define MAX_NIVELES 3

typedef struct papeleo{
    coordenada_t posicion;
    int id_papeleo;
    bool recolectado;
} papeleo_t;

typedef struct objeto{
    coordenada_t posicion;
    char tipo;
} objeto_t;

typedef struct nivel{
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    objeto_t obstaculos[MAX_OBSTACULOS];    
    int tope_obstaculos;
    objeto_t herramientas[MAX_HERRAMIENTAS];
    int tope_herramientas;
    papeleo_t papeleos[MAX_PAPELEOS];
    int tope_papeleos;
    coordenada_t pos_inicial_jugador;
}nivel_t;

typedef struct jugador{
    coordenada_t posicion;
    int movimientos;
    int martillos;
    int extintores;
    bool ahuyenta_randall;
}jugador_t;

typedef struct juego{
    int nivel_actual;
    nivel_t niveles[MAX_NIVELES];
    jugador_t jugador;
    char personaje_tp1;
}juego_t;

/* 
 * Procedimiento que recibe el juego e imprime toda su información por pantalla.
 */ 
void imprimir_terreno(juego_t juego);

/* 
 * Inicializará un nivel, cargando toda la información inicial, las paredes,
 * los objetos, los papeleos y la posición inicial del jugador en dicho nivel.
 */
void inicializar_nivel(nivel_t* nivel, int numero_nivel, char amigo_pide_favor);

/*
 * Inicializará el juego, cargando toda la información inicial, los datos del jugador, 
 * el personaje resultado del tp anterior, y los 3 niveles.
 */
void inicializar_juego(juego_t* juego, char amigo_pide_favor);

#endif /* __PAPELEO_H__ */