#define NIVEL_UNO 1
#define NIVEL_DOS 2
#define NIVEL_TRES 3
#define MAX_OBSTACULOS 100
#define MAX_HERRAMIENTAS 100
#define MAX_PAPELEOS 10
#define MAX_NIVELES 3
#define MEDIAS 'M'
#define FUEGO 'F'
#define EXTINTORES 'E'
#define MARTILLOS 'M'
#define BOTELLAS 'B'
#define INTERRUPTORES 'I'
#define MIKE_WAZOWSKI 'W'
#define PARED '+'
#define PAPELEOS 'L'
#define OLAF 'O'
#define STITCH 'S'
#define JASMIN 'J'
#define RAYO 'R'
#define VACIO ' '
#define CANT_PAPELEO_NIVEL_UNO 2
#define CANT_PAPELEO_NIVEL_DOS 3
#define CANT_PAPELEO_NIVEL_TRES 4
#define CANT_FUEGO_N1 10
#define CANT_FUEGO_N2 5
#define CANT_FUEGO_N3 3
#define CANT_MEDIAS_N1 5
#define CANT_MEDIAS_N2 4
#define CANT_MEDIAS_N3 3
#define SIZE_NIVEL_UNO 22
#define SIZE_NIVEL_DOS 17
#define SIZE_NIVEL_TRES 12
#define CANT_EXTINTORES_NIVEL_UNO 4
#define CANT_EXTINTORES_NIVEL_DOS 2
#define CANT_EXTINTORES_NIVEL_TRES 2
#define CANT_MARTILLOS_NIVEL_UNO 4
#define CANT_MARTILLOS_NIVEL_DOS 5
#define CANT_MARTILLOS_NIVEL_TRES 6
#define CANT_BOTELLAS_NIVEL_UNO 4
#define CANT_BOTELLAS_NIVEL_DOS 3
#define CANT_BOTELLAS_NIVEL_TRES 2
#define CANT_INTERRUPTORES_NIVEL_UNO 1
#define CANT_INTERRUPTORES_NIVEL_DOS 1
#define CANT_INTERRUPTORES_NIVEL_TRES 0
#define MOVIMIENTOS_INICIALES 40
#define CANT_MOVIMIENTOS_NIVEL_DOS 30
#define CANT_MOVIMIENTOS_NIVEL_TRES 20
#define MOVIMIENTOS_REGALADOS_POR_RAYO 10
#define MAX_SIZE 22


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utiles.h"

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

void debug_pos(coordenada_t a, char tipo){
    printf("(%i, %i) - %c\n", a.fil, a.col, tipo);
}

/*
* Procedimiento que recibe un struct de nivel
* Y quita el ultimo papeleo
*/

void quitar_papeleo(nivel_t* nivel){
    nivel->papeleos[nivel->tope_papeleos].posicion.fil = 0;
    nivel->papeleos[nivel->tope_papeleos].posicion.col = 0;
    nivel->papeleos[nivel->tope_papeleos].recolectado = true;
    nivel->tope_papeleos --;
}

/*
* Procedimiento que recibe un struct de nivel, y un identificador
* Y quita el obstaculo: fuego, correspondiente al indice del identificador
*/
void quitar_fuego(nivel_t* nivel, int id){
    int index, i;
    if(id != -1){
        for(i = 0; i<nivel->tope_obstaculos; i++){
            if(nivel->obstaculos[i].tipo != FUEGO){
                index = i - 1;
                i = nivel->tope_obstaculos;
            }
        }
    } else {
        index = id;
    }
    for(i = index; i<nivel->tope_obstaculos; i++){
        if(index != -1){
            nivel->obstaculos[i].posicion.fil = nivel->obstaculos[i+1].posicion.fil;
            nivel->obstaculos[i].posicion.col = nivel->obstaculos[i+1].posicion.col;
            nivel->obstaculos[i].tipo = nivel->obstaculos[i+1].tipo;
        }
    }
    nivel->tope_obstaculos --;
}

/*
* Funcion que recibe 2 coordenadas
* Devuelve true: Si ambas coordenadas son iguales
* De lo contrario false
*/
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
    return (a.fil == b.fil && a.col == b.col)? true:false;
}

/*
* Funcion que recibe 2 coordenadas
* y devuelve la distancia manhattan que hay entre ellas
*/
int distancia_manhattan(coordenada_t a, coordenada_t b){
    int res_fil, res_col;    
    (a.fil < b.fil)? res_fil = b.fil - a.fil : a.fil - b.fil;
    (a.col < b.col)? res_col = b.col - a.col : a.col - b.col;
    return res_fil + res_col;
}

/*
* Funcion que recibe: un obstaculo de Fuego, un Vector de paredes y su respectivo tope
* Y devuelve true: Si el fuego esta al lado de una pared
* De lo contrario devuelve false
*/
bool esta_cerca_de_pared(coordenada_t fuego, coordenada_t pared[MAX_PAREDES], int tope){
    bool resultado = false;
    for(int i = 0; i<tope; i++){
        if(distancia_manhattan(fuego, pared[i]) == 1){
            resultado = true;
        }
    }
    return resultado;
}

/*
* Funcion que recibe 1 coordenada, 1 vector de coordenadas y su tope
* Las compara y devuelve -1 si no se encuentra la coordenada repetida en el vector
* De lo contrario devuelve el indice en el que se encuentra
*/
int buscar_coordenada_en_coordenada(coordenada_t a, coordenada_t lista[MAX_OBSTACULOS], int tope){
    int id = -1;
    for(int i = 0; i < tope; i++){
        if(coordenadas_iguales(a, lista[i]) == true){
            id = i;
            i = tope;
        }
    }
    return id;
}

/*
* Funcion que recibe el tamaño del mapa
* y devuelve una coordenada aleatoria dentro del rango
*/
coordenada_t coordenada_aleatoria(int size){
    coordenada_t resultado;
    resultado.fil = rand()%size;
    resultado.col = rand()%size;
    return resultado;
}

/*
* Funcion que recibe 1 coordenada, 1 vector de obstaculos y su tope
* Las compara y devuelve -1 si no se encuentra la coordenada repetida en el vector
* De lo contrario devuelve el indice en el que se encuentra
*/
int buscar_coordenada_en_objeto(coordenada_t a, objeto_t b[MAX_OBSTACULOS], int tope){
    int id = -1;
    for(int i = 0; i < tope; i++){
        if(tope == -1 || coordenadas_iguales(a, b[i].posicion) == true){
            id = i;
            i = tope;
        }
    }
    return id;
}

/*
* Funcion que recibe 1 coordenada, 1 vector de papeleos y su tope
* Las compara y devuelve -1 si no se encuentra la coordenada repetida en el vector
* De lo contrario devuelve el indice en el que se encuentra
*/
int buscar_coordenada_en_papeleo(coordenada_t a, papeleo_t b[MAX_PAPELEOS], int tope){
    int id = -1;
    for (int i = 0; i < tope; i++){
        if(tope == -1 || coordenadas_iguales(a, b[i].posicion) == true){
            id = i;
            i = tope;
        }
    }
    return id;
}

/*
* Funcion que recibe el nivel en el que se encuentra
* Y devuelve el tamaño de mapa que le corresponde
*/
int obtener_size_mapa(int numero_nivel){
    int resultado;
    switch(numero_nivel){
        case NIVEL_UNO:
            resultado = SIZE_NIVEL_UNO;
            break;
        case NIVEL_DOS:
            resultado = SIZE_NIVEL_DOS;
            break;
        case NIVEL_TRES:
            resultado = SIZE_NIVEL_TRES;
            break;
    }
    return resultado;
}

/*
* Funcion que recibe el nivel, las coordenadas del jugador y el elemento a comparar
* Compara el elemento con todos los del juego
* Devuelve un char que representa el VACIO si no es igual a ninguna coordenada de otro elemento
* De lo contrario devuelve el char que represente al elemento con el que comparta dicha coordenada
*/
char quien_ocupa(nivel_t nivel, coordenada_t jugador, coordenada_t posicion){
    char resultado = VACIO;
    int id_obstaculos = buscar_coordenada_en_objeto(posicion, nivel.obstaculos, nivel.tope_obstaculos);
    int id_herramientas = buscar_coordenada_en_objeto(posicion, nivel.herramientas, nivel.tope_herramientas);
    
    if(coordenadas_iguales(posicion, jugador) == true){
        resultado = MIKE_WAZOWSKI;
    } else if (buscar_coordenada_en_papeleo(posicion, nivel.papeleos, nivel.tope_papeleos) != -1){
        resultado = PAPELEOS;
    } else if (id_obstaculos != -1){
        if(nivel.obstaculos[id_obstaculos].tipo == FUEGO){
            resultado = FUEGO;
        } else {
            resultado = MEDIAS;
        }
    } else if (id_herramientas != -1){
        if(nivel.herramientas[id_herramientas].tipo == BOTELLAS){
            resultado = BOTELLAS;
        } else {
            resultado = INTERRUPTORES;
        }
    } else if (buscar_coordenada_en_coordenada(posicion, nivel.paredes, nivel.tope_paredes) != -1){
        resultado = PARED;
    }
    
    return resultado;
}

void imprimir_terreno(juego_t juego){
    int i, k, size = obtener_size_mapa(juego.nivel_actual);
    char matriz[size][size];
    coordenada_t posicion_aux;
    for(i = 0; i<size; i++){
        for(k = 0; k<size; k++){
            matriz[i][k] = VACIO;
        }
    }

    for(i = 0; i<size; i++){
        for(k = 0; k<size; k++){
            posicion_aux.fil = i;
            posicion_aux.col = k;
            matriz[i][k] = quien_ocupa(juego.niveles[juego.nivel_actual-1], juego.jugador.posicion, posicion_aux);
        }
    }

    for(i = 0; i < size; i++){
        for(k = 0; k<size; k++){
            printf(" %c", matriz[i][k]);
        }
        printf("\n");
    }

    for(i = 0; i<size; i++){
        for(k = 0; k<size; k++){
            matriz[i][k] = VACIO;
        }
    }
}

/*
* Procedimiento que recibe el juego, el amigo que solicita el favor, y el nivel correspondiente
* Modifica el struct del juego acorde a la convencion de obsequios correspondiente
*/
void obtener_obsequio(juego_t* juego, char amigo_pide_favor, int nivel){
    switch (amigo_pide_favor){
        case JASMIN:
            juego->jugador.martillos ++;
            break;
        case RAYO:
            if(nivel == NIVEL_UNO){
                juego->jugador.movimientos += MOVIMIENTOS_REGALADOS_POR_RAYO;
            }
            break;
        case STITCH:
            if(nivel == NIVEL_TRES){
                quitar_papeleo(&juego->niveles[nivel-1]);
            }
            break;
        case OLAF:
            if(nivel == NIVEL_DOS){
                for(int i = 0; i < 2; i++){
                    quitar_fuego(&juego->niveles[nivel-1], -1);
                }
            } else if (nivel == NIVEL_TRES){
                quitar_fuego(&juego->niveles[nivel-1], -1);
            }
    }
}

/*
* Procedimiento que recibe el juego, el amigo que solicita el favor y el nivel correspondiente
* Realiza los cambios de elementos del jugador acorde suba de nivel y aplica su obsequio
*/
void subir_de_nivel(juego_t* juego, char amigo_pide_favor, int nivel){
    juego->nivel_actual++;
    obtener_obsequio(juego, amigo_pide_favor, nivel);
    switch(nivel){
        case NIVEL_DOS:
            juego->jugador.martillos += CANT_MARTILLOS_NIVEL_DOS;
            juego->jugador.extintores += CANT_EXTINTORES_NIVEL_DOS;
            juego->jugador.movimientos += CANT_MOVIMIENTOS_NIVEL_DOS;
            juego->jugador.ahuyenta_randall = false;
            break;
        case NIVEL_TRES:
            juego->jugador.martillos += CANT_MARTILLOS_NIVEL_TRES;
            juego->jugador.extintores += CANT_EXTINTORES_NIVEL_TRES;
            juego->jugador.movimientos += CANT_MOVIMIENTOS_NIVEL_TRES;
            juego->jugador.ahuyenta_randall = false;
            break;
    } 
}

/*
* Procedimiento que recibe los topes respectivos de cada obstaculo y su nivel
* Y le asigna la cantidad que le corresponde segun lo solicitado
*/
void cargar_cant_obstaculos(int* cant_fuego, int* cant_medias, int numero_nivel){
    switch (numero_nivel){
        case NIVEL_UNO:
            *cant_fuego = CANT_FUEGO_N1;
            *cant_medias = CANT_MEDIAS_N1;
            break;
        case NIVEL_DOS:
            *cant_fuego = CANT_FUEGO_N2;
            *cant_medias = CANT_MEDIAS_N2;
            break;
        case NIVEL_TRES:
            *cant_fuego = CANT_FUEGO_N3;
            *cant_medias = CANT_MEDIAS_N3;
            break;
    }
}

/*
* Procedimiento que recibe los topes respectivos de cada herramienta y su nivel
* Y les asigna la cantidad que le corresponde segun lo solicitado
*/
void cargar_cant_herramientas(int* cant_botellas, int* cant_interruptores, int numero_nivel){
    switch(numero_nivel){
        case NIVEL_UNO:
            *cant_botellas = CANT_BOTELLAS_NIVEL_UNO;
            *cant_interruptores = CANT_INTERRUPTORES_NIVEL_UNO;  
            break;
        case NIVEL_DOS:
            *cant_botellas = CANT_BOTELLAS_NIVEL_DOS;
            *cant_interruptores = CANT_INTERRUPTORES_NIVEL_DOS;
            break;
        case NIVEL_TRES:
            *cant_botellas = CANT_BOTELLAS_NIVEL_TRES;
            *cant_interruptores = CANT_INTERRUPTORES_NIVEL_TRES;
            break;
    }
}

/*
* Procedimiento que recibe el nivel y su numero
* y rellena las posiciones, tope y tipo de obstaculo
*/
void obtener_obstaculos(int numero_nivel, nivel_t* nivel){
    int cant_fuego, cant_medias;
    coordenada_t posicion_aux, jugador = nivel->pos_inicial_jugador;
    cargar_cant_obstaculos(&cant_fuego, &cant_medias, numero_nivel);
    nivel->tope_obstaculos = cant_fuego + cant_medias;
    for(int i = 0; i<nivel->tope_obstaculos; i++){
        do{
            nivel->obstaculos[i].posicion = coordenada_aleatoria(obtener_size_mapa(numero_nivel));
            posicion_aux.fil = nivel->obstaculos[i].posicion.fil;
            posicion_aux.col = nivel->obstaculos[i].posicion.col;
            if(i<cant_fuego){
                nivel->obstaculos[i].tipo = FUEGO;
            } else {
                nivel->obstaculos[i].tipo = MEDIAS;
            }
            if(nivel->obstaculos[i].tipo == FUEGO && esta_cerca_de_pared(posicion_aux, nivel->paredes, nivel->tope_paredes) == false){
                continue;
            }
        }  while(coordenadas_iguales(posicion_aux, jugador) == true || buscar_coordenada_en_coordenada(posicion_aux, nivel->paredes, nivel->tope_paredes) != -1 || buscar_coordenada_en_objeto(posicion_aux, nivel->obstaculos, i-1) != -1);
    }
}

/*
* Procedimiento que recibe el nivel y su numero
* y rellena las posiciones, tope y tipo de herramienta
*/
void obtener_herramientas(int numero_nivel, nivel_t* nivel){
    int cant_botellas, cant_interruptores;
    coordenada_t posicion_aux, jugador = nivel->pos_inicial_jugador;
    cargar_cant_herramientas(&cant_botellas, &cant_interruptores, numero_nivel);
    nivel->tope_herramientas = cant_botellas + cant_interruptores;
    for(int i = 0; i<nivel->tope_herramientas; i++){
        do{
            nivel->herramientas[i].posicion = coordenada_aleatoria(obtener_size_mapa(numero_nivel));
            posicion_aux = nivel->herramientas[i].posicion;
            if(i<cant_botellas){
                nivel->herramientas[i].tipo = BOTELLAS;
            } else {
                nivel->herramientas[i].tipo = INTERRUPTORES;
            }
        }while(coordenadas_iguales(posicion_aux, jugador) == true && buscar_coordenada_en_coordenada(posicion_aux, nivel->paredes, nivel->tope_paredes) != -1 && buscar_coordenada_en_objeto(posicion_aux, nivel->obstaculos, nivel->tope_obstaculos) != -1 && buscar_coordenada_en_objeto(posicion_aux, nivel->herramientas, i-1) != -1);
    }
}

/*
* Funcion que recibe el nivel en el que se encuentra
* y devuelve la cantidad de papeleos que deben haber en dicho nivel
*/
int papeleo_por_nivel(int numero_nivel){
    int resultado;
    switch (numero_nivel){
        case NIVEL_UNO:
            resultado = 2;
            break;
        case NIVEL_DOS:
            resultado = 3;
            break;
        case NIVEL_TRES:
            resultado = 4;
            break;
    }
    return resultado;
}

/*
* Funcion que recibe el nivel en el que se encuentra y el indice de papeleo
* Y devuelve el indice que le corresponde al papeleo que lo llama
*/
int obtener_id_papeleo(int nivel, int indice_actual){
    int resultado;
    switch (nivel){
        case NIVEL_UNO:
            resultado = indice_actual;
            break;
        case NIVEL_DOS:
            resultado = indice_actual + CANT_PAPELEO_NIVEL_UNO;
            break;
        case NIVEL_TRES:
            resultado = indice_actual + CANT_PAPELEO_NIVEL_UNO + CANT_PAPELEO_NIVEL_DOS;
            break;
    }
    return resultado;
}

/*
* Procedimiento que recibe el nivel y su numero
* y rellena las posiciones, tope y tipo de papeleos
*/
void obtener_papeleos(int numero_nivel, nivel_t* nivel){
    nivel->tope_papeleos = papeleo_por_nivel(numero_nivel);
    coordenada_t posicion_aux, jugador = nivel->pos_inicial_jugador;
    int size = obtener_size_mapa(numero_nivel);
    for(int i = 0; i<nivel->tope_papeleos; i++){
        do{
            nivel->papeleos[i].posicion = coordenada_aleatoria(size);
            posicion_aux = nivel->papeleos[i].posicion;
        }while(coordenadas_iguales(posicion_aux, jugador) == false && buscar_coordenada_en_coordenada(posicion_aux, nivel->paredes, nivel->tope_paredes) != -1 && buscar_coordenada_en_objeto(nivel->papeleos[i].posicion, nivel->obstaculos, nivel->tope_obstaculos) != 1 && buscar_coordenada_en_objeto(nivel->papeleos[i].posicion, nivel->herramientas, nivel->tope_herramientas) != -1 && buscar_coordenada_en_papeleo(nivel->papeleos[i].posicion, nivel->papeleos, nivel->tope_papeleos -1) != -1);
        nivel->papeleos[i].id_papeleo = obtener_id_papeleo(numero_nivel, i);
        nivel->papeleos[i].recolectado = false;
    }
}

/*
* Procedimiento que transfiere los datos de la posicion inicial del jugador a su struct correspondiente, y rellena los datos restantes
*/
void inicializar_jugador(juego_t* juego){
    juego->jugador.posicion.fil = juego->niveles[NIVEL_UNO -1].pos_inicial_jugador.fil; 
    juego->jugador.posicion.col = juego->niveles[NIVEL_UNO -1].pos_inicial_jugador.col;
    juego->jugador.movimientos = MOVIMIENTOS_INICIALES;
    juego->jugador.martillos = CANT_MARTILLOS_NIVEL_UNO;
    juego->jugador.extintores = CANT_EXTINTORES_NIVEL_UNO;
    juego->jugador.ahuyenta_randall = false;
}

void inicializar_nivel(nivel_t* nivel, int numero_nivel, char amigo_pide_favor){
    obtener_paredes(numero_nivel, nivel->paredes, &nivel->tope_paredes, &nivel->pos_inicial_jugador);
    obtener_obstaculos(numero_nivel, nivel);
    obtener_herramientas(numero_nivel, nivel);
    obtener_papeleos(numero_nivel, nivel);
}

void inicializar_juego(juego_t* juego, char amigo_pide_favor){
    juego->nivel_actual = NIVEL_UNO;
    juego->personaje_tp1 = amigo_pide_favor;
    for(int i = 1; i <= MAX_NIVELES; i++){
        inicializar_nivel(&juego->niveles[i-1], i, amigo_pide_favor);
    }
    inicializar_jugador(juego);
    obtener_obsequio(juego, amigo_pide_favor, juego->nivel_actual -1);
}