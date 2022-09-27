#define NIVEL_UNO 1
#define NIVEL_DOS 2
#define NIVEL_TRES 3
#define MAX_OBSTACULOS 100
#define MAX_HERRAMIENTAS 100
#define MAX_PAPELEOS 10
#define MAX_NIVELES 3
#define RANDALL 'R'
#define MOVIMIENTO 'P'
#define MEDIAS 'M'
#define FUEGO 'F'
#define EXTINTORES 'E'
#define MARTILLOS 'm'
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
#define CONTROLES_A 'U'
#define CONTROLES_B '-'
#define ELEMENTO_NIVEL 'K'
#define SUBIDA_DE_NIVEL 'N'
#define PAPELEO_INCORRECTO 'X'
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
#define SUPERIOR 1
#define INFERIOR 2
#define CODE_PARED "\U0001f3fb\uFE0F"
#define CODE_INTERRUPTOR "\U0001f579"
#define CODE_BOTELLA "\U0001f9f4"
#define CODE_PAPELEO "\U0001f4dc"
#define CODE_MEDIA "\U0001f9e6"
#define CODE_WAZOWSKI "\U0001f9d1"
#define CODE_FUEGO "\U0001f525"
#define CODE_EXTINTOR "\U0001f9ef"
#define CODE_MARTILLO "\U0001f528"
#define MOV_ARRIBA 'W'
#define MOV_IZQUIERDA 'A'
#define MOV_ABAJO 'S'
#define MOV_DERECHA 'D'
#define ROT_HORARIA 'E'
#define ROT_ANTIHORARIA 'Q'
#define USAR_MARTILLO 'Z'
#define USAR_EXTINTOR 'C'
#define PENAlIZACION_MEDIAS 10
#define GANANCIA_BOTELLA 7
#define ACCION_ARRIBA 'W'
#define ACCION_ABAJO 'S'
#define SALIR 'O'

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utiles.h"
#include "termcolor-c.h"
#include "controlador.h"

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
    int movimientos_realizados;
}jugador_t;

typedef struct juego{
    int nivel_actual;
    nivel_t niveles[MAX_NIVELES];
    jugador_t jugador;
    char personaje_tp1;
}juego_t;

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
* Funcion que recibe el numero de nivel
* Y devuelve cuantos movimientos se requieren para la ejecucion de randall
*/
int movimientos_requeridos_por_randall(int nivel){
    int resultado;
    switch (nivel){
        case NIVEL_UNO:
            resultado = 7;
            break;
        case NIVEL_DOS:
            resultado = 5;
            break;
        case NIVEL_TRES:
            resultado = 3;
    }
    return resultado;
}

/*
* Funcion que recibe el nivel y si el interruptor esta apretado
* devuelve True si en este turno se pueden mover los papeleos
*/
bool papeleos_movibles(int movimientos, int nivel, bool interruptor_apretado){
    return (movimientos % movimientos_requeridos_por_randall(nivel) == 0 && movimientos >= movimientos_requeridos_por_randall(nivel))? true:false;
}


/*
* Funcion que recibe 2 coordenadas
* Devuelve true: Si ambas coordenadas son iguales
* De lo contrario false
*/
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
    return a.fil == b.fil && a.col == b.col;
}

/*
* Funcion que recibe 2 coordenadas
* y devuelve la distancia manhattan que hay entre ellas
*/
int distancia_manhattan(coordenada_t a, coordenada_t b){
    int res_fil = (a.fil < b.fil)? b.fil - a.fil : a.fil - b.fil, res_col = (a.col < b.col)? b.col - a.col : a.col - b.col;
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
* Funcion que recibe los papeleos y el indice a verificar
* Devuelve True si ese indice de papeleo es el siguiente a recolectar
*/
bool es_el_papeleo_siguiente(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos, int indice){
    bool resultado;
    if(indice > 0){
        resultado = (papeleos[indice-1].recolectado)? true:false;
    } else {
        resultado = true;
    }
    return resultado;
}

/*
* Funcion que recibe un elemento del nivel, y la indicación de que elemento e indice es
* O -> Obstaculos, H-> Herramienta, de lo contrario Papeleo
* Y lo compara con el resto de los elementos ya cargados
* Devuelve True si la coordenada esta ocupada, de lo contrario False
*/
bool la_coordenada_esta_usada(char tipo, int tope_cargado, coordenada_t coordenada, nivel_t nivel){
    int res1 = buscar_coordenada_en_coordenada(coordenada, nivel.paredes, nivel.tope_paredes), res3 = -1, res4 = -1, res5 = -1;
    bool res2 = coordenadas_iguales(coordenada, nivel.pos_inicial_jugador);
    if(tipo != 'O'){
        res3 = buscar_coordenada_en_objeto(coordenada, nivel.obstaculos, nivel.tope_obstaculos);
        if(tipo != 'H'){
            res4 = buscar_coordenada_en_objeto(coordenada, nivel.herramientas, nivel.tope_herramientas);
            if(tope_cargado != 0){
                res5 = buscar_coordenada_en_papeleo(coordenada, nivel.papeleos, tope_cargado);
            }
        } else if (tope_cargado != 0){
            res4 = buscar_coordenada_en_objeto(coordenada, nivel.herramientas, tope_cargado);
        }
    } else if (tope_cargado != 0){
        res3 = buscar_coordenada_en_objeto(coordenada, nivel.obstaculos, tope_cargado);
    }
    return (res1 != -1 || res2 == true || res3 != -1 || res4 != -1 || res5 != -1)? true:false;
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
        resultado = (nivel.papeleos[buscar_coordenada_en_papeleo(posicion, nivel.papeleos, nivel.tope_papeleos)].recolectado)? VACIO:PAPELEOS;
        if(resultado == PAPELEOS && !es_el_papeleo_siguiente(nivel.papeleos, nivel.tope_papeleos, buscar_coordenada_en_papeleo(posicion, nivel.papeleos, nivel.tope_papeleos))){
            resultado = PAPELEO_INCORRECTO;
        }
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

/*
* Funcion que recibe el nivel actual
* Y devuelve cuantas paredes se colocaran aleatoriamente en dicho nivel
*/
int cant_paredes_random(int nivel_actual){
    int resultado;
    switch(nivel_actual){
        case NIVEL_UNO:
            resultado = 40;
            break;
        case NIVEL_DOS:
            resultado = 30;
            break;
        case NIVEL_TRES:
            resultado = 20;
            break;
    }
    return resultado;
}

/*
* Procedimiento que agrega una pared aleatoria al nivel
*/
void agregar_pared_random(nivel_t* nivel, jugador_t jugador, int nivel_actual){
    coordenada_t pos_aux;
    do{
        pos_aux = coordenada_aleatoria(obtener_size_mapa(nivel_actual));
    } while (quien_ocupa(*nivel, jugador.posicion, pos_aux) != VACIO);
    nivel->paredes[nivel->tope_paredes] = pos_aux;
    nivel->tope_paredes ++;
}

/*
* Procedimiento que elimina un objeto del vector
*/
void quitar_objeto(objeto_t objetos[MAX_OBSTACULOS], int indice, int* tope){
    objetos[indice].posicion.fil = -1;
    objetos[indice].posicion.col = -1;
}

/*
* Funcion que verifica con quien colisiona el jugador
* Devuelve el caracter que corresponde al elemento con el que se colisiona
*/
char colision_actual(nivel_t nivel, coordenada_t posicion_jugador){
    char resultado = VACIO;
    if(buscar_coordenada_en_coordenada(posicion_jugador, nivel.paredes, nivel.tope_paredes) != -1){
        resultado = PARED;
    } else if(buscar_coordenada_en_objeto(posicion_jugador, nivel.obstaculos, nivel.tope_obstaculos) != -1){
        resultado = (nivel.obstaculos[buscar_coordenada_en_objeto(posicion_jugador, nivel.obstaculos, nivel.tope_obstaculos)].tipo == FUEGO)? FUEGO:MEDIAS;
    } else if(buscar_coordenada_en_objeto(posicion_jugador, nivel.herramientas, nivel.tope_herramientas) != -1){
        resultado = (nivel.herramientas[buscar_coordenada_en_objeto(posicion_jugador, nivel.herramientas, nivel.tope_herramientas)].tipo == BOTELLAS)? BOTELLAS:INTERRUPTORES;
    } else if(buscar_coordenada_en_papeleo(posicion_jugador, nivel.papeleos, nivel.tope_papeleos) != -1){
        resultado = PAPELEOS;
    } else {
        resultado = VACIO;
    }
    return resultado;
}

/*
* Procedimiento que comprueba si la coordenada del jugador colisiona con algun objeto
* Realiza los cambios en el nivel o el jugador dependiendo de la colision
*/
void comprobar_colision(nivel_t* nivel, jugador_t* jugador){
    char elemento_colisionado = colision_actual(*nivel, jugador->posicion);
    switch (elemento_colisionado){
        case FUEGO:
            jugador->movimientos = 0;
        break;
        case MEDIAS:
            jugador->movimientos -= 10;
        break;
        case BOTELLAS:
            jugador->movimientos += 7;
            quitar_objeto(nivel->herramientas, buscar_coordenada_en_objeto(jugador->posicion, nivel->herramientas, nivel->tope_herramientas), &nivel->tope_herramientas);
        break;
        case INTERRUPTORES:
            jugador->ahuyenta_randall = (jugador->ahuyenta_randall)? false:true;
        break;
        case PAPELEOS:
            if(es_el_papeleo_siguiente(nivel->papeleos, nivel->tope_papeleos, buscar_coordenada_en_papeleo(jugador->posicion, nivel->papeleos, nivel->tope_papeleos))){
                nivel->papeleos[buscar_coordenada_en_papeleo(jugador->posicion, nivel->papeleos, nivel->tope_papeleos)].recolectado = true;
            }
        break;
    }
}

/*
* Funcion que recibe una coordenada y cuanto mide la matriz
* Devuelve true si esta dentro del rango
*/
bool esta_dentro_de_rango(coordenada_t posicion, int size){
    return (posicion.fil < size && posicion.fil > 0 && posicion.col < size && posicion.col > 0)? true:false;
}

/*
* Procedimiento que verifica si el jugador puede caer
* cambia su posicion hasta que no pueda caer mas
*/
void aplicar_efecto_gravitacional(nivel_t* nivel, jugador_t* jugador, int size, bool es_subida_de_nivel){
    coordenada_t coordenada_aux = jugador->posicion;
    coordenada_aux.fil += 1;
    while(buscar_coordenada_en_coordenada(coordenada_aux, nivel->paredes, nivel->tope_paredes) == -1 && esta_dentro_de_rango(coordenada_aux, size)){
        coordenada_aux.fil += 1;
        jugador->posicion.fil += 1;
        if(!es_subida_de_nivel){
            comprobar_colision(nivel, jugador);
        }
    }
}

/*
* Procedimiento que verifica los movimientos realizados
* Y agrega las paredes o mueve los papeleos segun corresponda
*/
void acciones_por_movimiento(nivel_t* nivel, jugador_t jugador, int nivel_actual){
    int movimientos_pared = cant_paredes_random(nivel_actual), dado = rand()%nivel->tope_papeleos;
    coordenada_t pos_aux;
    if(papeleos_movibles(jugador.movimientos_realizados, nivel_actual, jugador.ahuyenta_randall) && jugador.movimientos_realizados > 1){
        do{
            pos_aux = coordenada_aleatoria(obtener_size_mapa(nivel_actual));
        }while(quien_ocupa(*nivel, jugador.posicion, pos_aux) != VACIO);
            nivel->papeleos[dado].posicion = pos_aux;
    }
    if(jugador.movimientos_realizados < movimientos_pared){
        agregar_pared_random(nivel, jugador, nivel_actual);
    }
}

/*
* Procedimiento que imprime el elemento dado con un estilo distinto
*/
void imprimir_elemento(char elemento){
    switch(elemento){
        case MIKE_WAZOWSKI:
            fputs("", background_gray(text_green(stdout)));
            printf("%s", CODE_WAZOWSKI);
        break;
        case FUEGO:
            fputs("", background_gray(text_yellow(stdout)));
            printf("%s", CODE_FUEGO);
        break;
        case MEDIAS:
            fputs("", background_gray(text_white(stdout)));
            printf("%s", CODE_MEDIA);
        break;
        case BOTELLAS:
            fputs("", background_gray(text_white(stdout)));
            printf("%s", CODE_BOTELLA);
        break;
        case INTERRUPTORES:
            fputs("", background_gray(text_white(stdout)));
            printf("%s ", CODE_INTERRUPTOR);
        break;
        case PAPELEOS:
            fputs("", background_green(text_white(stdout)));
            printf("%s", CODE_PAPELEO);
        break;
        case PAPELEO_INCORRECTO:
            fputs("", background_gray(text_white(stdout)));
            printf("%s", CODE_PAPELEO);
        break;
        case PARED:
            fputs("", background_gray(text_red(stdout)));
            printf("%s", CODE_PARED);
        break;
        default:
            fputs("",background_gray(stdout));
            printf("  ");
    }
}

/*
* Procedimiento que vacia los datos de la matriz para evitar operar con NULLs
*/
void vaciar_matriz(char matriz[MAX_SIZE][MAX_SIZE], int tope){
    for(int i = 0; i<tope; i++){
        for(int k = 0; k < tope; k++){
            matriz[i][k] = VACIO;
        }
    }
}

/*
* Procedimiento que llena la matriz con los datos inicializados del juego
*/
void llenar_matriz(char matriz[MAX_SIZE][MAX_SIZE], juego_t juego, int tope){
    coordenada_t posicion_aux;
    for(int i = 0; i < tope; i++){
        for(int k = 0; k < tope; k++){
            posicion_aux.fil = i;
            posicion_aux.col = k;
            matriz[i][k] = quien_ocupa(juego.niveles[juego.nivel_actual-1], juego.jugador.posicion, posicion_aux);
        }
    }
}

/*
* Procedimiento que imprime la interfaz de usuario al costado del mapa
*/
void imprimir_elemento_ux(char elemento, int nivel, jugador_t jugador, int papeleos){
    fputs("", background_gray(stdout));
    switch(elemento){
        case CONTROLES_A:
            printf("|   Movimiento: A-D  Rotacion: Q-E    |");
            break;
        case CONTROLES_B:
            printf("|   Extintor: C    Martillo: Z        |");
            break;
        case INTERRUPTORES:
            printf("|   Interruptor operacional           |");
            break;
        case ELEMENTO_NIVEL:
            printf("|   NIVEL %i                           |", nivel);
            break;
        case EXTINTORES:
            printf("|   Extintores restantes: %i %s        |", jugador.extintores, CODE_EXTINTOR);
            break;
        case MARTILLOS:
            printf("|   Martillos restantes: %i %s         |", jugador.martillos, CODE_MARTILLO);
            break;
        case PAPELEOS:
            printf("|   Papeleos por ordenar: %i %s        |", papeleos, CODE_PAPELEO);
            break;
        case RANDALL:
            printf("|   OH NO, RANDALL MOVIO LOS PAPELES! |");
            break;
        case MOVIMIENTO:
            printf("|   Movimientos Restantes: %i        ", jugador.movimientos);
            if(jugador.movimientos < 10){
                printf("  |");
            }else if(jugador.movimientos < 100){
                printf(" |");
            }else{
                printf("|");
            }
            break;
        default:
            printf("|                                     |"); 
    }
    limpiar_stdout();
}

/*
* Procedimiento que solicita la impresion de la interfaz de usuario lateral
* realizando ajustes acorde al nivel para que quepe en el mapa
*/
void imprimir_ux(juego_t juego, int line){
    int line_fix = (line%2 == 0)? (line/2)-1 : (line+1)/2;
    line_fix = (juego.nivel_actual == NIVEL_UNO)? line_fix:line;
    char colision = colision_actual(juego.niveles[juego.nivel_actual -1], juego.jugador.posicion);
    switch(line_fix){
        case 1:
            imprimir_elemento_ux(ELEMENTO_NIVEL, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 2:
            imprimir_elemento_ux(EXTINTORES, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 3:
            imprimir_elemento_ux(MARTILLOS, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 4:
            imprimir_elemento_ux(PAPELEOS, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 5:
            imprimir_elemento_ux(MOVIMIENTO, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 6:
            (juego.jugador.ahuyenta_randall)?
                imprimir_elemento_ux(INTERRUPTORES, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos):
                imprimir_elemento_ux(VACIO, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 7:
            (papeleos_movibles(juego.jugador.movimientos_realizados, juego.nivel_actual, juego.jugador.ahuyenta_randall))?
                imprimir_elemento_ux(RANDALL, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos):   
                imprimir_elemento_ux(VACIO, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 8:
            if(colision == BOTELLAS){
                imprimir_elemento_ux(BOTELLAS, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            } else if(colision == MEDIAS){
                imprimir_elemento_ux(MEDIAS, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            } else {
                imprimir_elemento_ux(VACIO, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            }
            break;
        case 9:
            imprimir_elemento_ux(CONTROLES_A, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        case 10:
            imprimir_elemento_ux(CONTROLES_B, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);
            break;
        default:
            imprimir_elemento_ux(VACIO, juego.nivel_actual, juego.jugador, juego.niveles[juego.nivel_actual -1].tope_papeleos);    
    }
}

/*
* Procedimiento que imprime el mapa del nivel
*/
void imprimir_terreno(juego_t juego){
    int i, k, size = obtener_size_mapa(juego.nivel_actual);
    char matriz[MAX_SIZE][MAX_SIZE];
    vaciar_matriz(matriz, size);
    llenar_matriz(matriz, juego, size);
    for(i = 0; i<size; i++){
        fputs(" |", text_red(stdout));
        limpiar_stdout();
        for(k = 0; k<size; k++){
            imprimir_elemento(matriz[i][k]);
            limpiar_stdout();
        }
        imprimir_ux(juego, i);
        printf("\n");
    }
    printf(" ");
    vaciar_matriz(matriz, size);
    printf("\n");
}

/*
* Procedimiento que recibe el juego, el amigo que solicita el favor, y el nivel correspondiente
* Modifica el struct del juego acorde a la convencion de obsequios correspondiente
*/
void obtener_obsequio(juego_t* juego, int nivel){
    switch (juego->personaje_tp1){
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
* Procedimiento que al subir de nivel, reposiciona la ubicacion del jugador aleatoriamente
*/
void reposicionar_jugador(nivel_t nivel, coordenada_t* jugador_pos, int size, int nivel_actual){
    do{
        *jugador_pos = coordenada_aleatoria(size);
    } while(colision_actual(nivel, *jugador_pos) != VACIO);
}

/*
* Procedimiento que recibe el juego, el amigo que solicita el favor y el nivel correspondiente
* Realiza los cambios de elementos del jugador acorde suba de nivel y aplica su obsequio
*/
void subir_de_nivel(juego_t* juego){
    juego->nivel_actual++;
    int nivel = juego->nivel_actual;
    int size = obtener_size_mapa(nivel);
    reposicionar_jugador(juego->niveles[nivel-1], &juego->jugador.posicion, size, nivel);
    obtener_obsequio(juego, nivel);
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
    aplicar_efecto_gravitacional(&juego->niveles[juego->nivel_actual], &juego->jugador, size, true);
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
    coordenada_t posicion_aux;
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
        }  while(la_coordenada_esta_usada('O', i, posicion_aux, *nivel) || (!esta_cerca_de_pared(posicion_aux, nivel->paredes, nivel->tope_paredes) && i<cant_fuego));
    }
}

/*
* Procedimiento que recibe el nivel y su numero
* y rellena las posiciones, tope y tipo de herramienta
*/
void obtener_herramientas(int numero_nivel, nivel_t* nivel){
    int cant_botellas, cant_interruptores;
    coordenada_t posicion_aux;
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
        }while(la_coordenada_esta_usada('O', i, posicion_aux, *nivel));
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
    coordenada_t posicion_aux;
    int size = obtener_size_mapa(numero_nivel);
    for(int i = 0; i<nivel->tope_papeleos; i++){
        do{
            nivel->papeleos[i].posicion = coordenada_aleatoria(size);
            posicion_aux = nivel->papeleos[i].posicion;
        }while(la_coordenada_esta_usada('P', i, posicion_aux, *nivel));
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
    juego->jugador.movimientos_realizados = 0;
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
        inicializar_nivel(&juego->niveles[i-1], i, juego->personaje_tp1);
    }
    inicializar_jugador(juego);
    obtener_obsequio(juego, juego->nivel_actual -1);
}

/*
* Procedimiento que imprime un mensaje en caso de salir del juego
*/
void salida(){
    system("clear");
    printf("SALISTE DEL JUEGO\n");
    exit(0);
}

/*
* Procedimiento que recibe una coordenada
* Y se cambia sus coordenadas considerando que es parte de la matriz (Rotandola horaria o antihorariamente)
*/
void rotar_coordenada(coordenada_t* coordenada, char sentido, int size){
    coordenada_t coordenada_aux = *coordenada;
    coordenada->fil = (sentido == ROT_HORARIA)? coordenada_aux.col : abs(coordenada_aux.col - size+1);
    coordenada->col = (sentido == ROT_HORARIA)? abs(coordenada_aux.fil - size+1) : coordenada_aux.fil;
}

/*
* Procedimiento que recibe las paredes y las rota en la matriz
*/
void rotar_paredes(coordenada_t paredes[MAX_PAREDES], int tope, char sentido, int size){
    for(int i = 0; i < tope; i++){
        rotar_coordenada(&paredes[i], sentido, size);
    }
}

/*
* Procedimiento que recibe los objetos (Obstaculos o Herramientas) y las rota en la matriz
*/
void rotar_objetos(objeto_t objeto[MAX_OBSTACULOS], int tope, char sentido, int size){
    for(int i = 0; i < tope; i++){
        rotar_coordenada(&objeto[i].posicion, sentido, size);
    }
}

/*
* Procedimiento que recibe los papeleos y los rota en la matriz
*/
void rotar_papeleos(papeleo_t papeleos[MAX_PAPELEOS], int tope, char sentido, int size){
    for(int i = 0; i < tope; i++){
        rotar_coordenada(&papeleos[i].posicion, sentido, size);
    }
}

/*
* Procedimiento que rota toda la matriz
*/
void rotar_matriz(nivel_t* nivel, jugador_t* jugador, char sentido, int size){
    rotar_paredes(nivel->paredes, nivel->tope_paredes, sentido, size);
    rotar_coordenada(&jugador->posicion, sentido, size);
    rotar_objetos(nivel->obstaculos, nivel->tope_obstaculos, sentido, size);
    rotar_objetos(nivel->herramientas, nivel->tope_herramientas, sentido, size);
    rotar_papeleos(nivel->papeleos, nivel->tope_papeleos, sentido, size);
    comprobar_colision(nivel, jugador);
    aplicar_efecto_gravitacional(nivel, jugador, size, false);
}

bool es_solicitud_valida(char movimiento, char herramienta){
    bool resultado;
    switch(herramienta){
        case MARTILLOS:
            resultado = (movimiento != MOV_IZQUIERDA && movimiento != MOV_ARRIBA && movimiento != MOV_DERECHA && movimiento != MOV_ABAJO)? false:true;
            break;
        case EXTINTORES:
            resultado = (movimiento != MOV_IZQUIERDA && movimiento != MOV_ARRIBA && movimiento != MOV_DERECHA)? false:true;
            break;
        case VACIO:
            resultado = (movimiento != USAR_EXTINTOR && movimiento != USAR_MARTILLO && movimiento != MOV_DERECHA && movimiento != MOV_IZQUIERDA && movimiento != ROT_HORARIA && movimiento != ROT_ANTIHORARIA)? false:true;
            break;
    }
    return resultado;
}

/*
* Procedimiento que solicita el ingreso de un movimiento de parte del jugador
*/
void pedir_movimiento(char* movimiento, char herramienta){
    printf("> ");
    do{
        scanf(" %c", movimiento);
    } while (!es_solicitud_valida(*movimiento, herramienta));
        
    printf("\n");
}

/*
* Funcion que recibe el movimiento y los elementos del juego
* devuelve true si al ser un desplazamiento, puede realizarlo
*/
bool es_mov_valido(char movimiento, nivel_t nivel, coordenada_t jugador_pos, int size){
    coordenada_t pos_aux = jugador_pos;
    switch(movimiento){
        case MOV_IZQUIERDA:
            pos_aux.col--;
            break;
        case MOV_DERECHA:
            pos_aux.col++;
            break;
    }
    return (colision_actual(nivel, pos_aux) != PARED && esta_dentro_de_rango(pos_aux, size))? true:false;
}

/*
* Procedimiento que dado el movimiento que realizo el jugador
* aplica los cambios
*/
void aplicar_movimiento(char movimiento, jugador_t* jugador, nivel_t* nivel, int size){
    switch(movimiento){
        case MOV_IZQUIERDA:
            jugador->posicion.col--;
            break;
        case MOV_DERECHA:
            jugador->posicion.col++;
            break;
        case ROT_HORARIA:
            rotar_matriz(nivel, jugador, movimiento, size);
            break;
        case ROT_ANTIHORARIA:
            rotar_matriz(nivel, jugador, movimiento, size);
            break;
        case SALIR:
            salida();
            break;
    }
}

/*
* Procedimiento que dada la direccion, intenta utilizar un extintor en la misma
*/
void usar_extintor(char movimiento, nivel_t* nivel, jugador_t* jugador){
    coordenada_t pos_aux = jugador->posicion;
    pos_aux.col = (movimiento == MOV_IZQUIERDA)? pos_aux.col - 1: pos_aux.col + 1;
    if(movimiento == MOV_ARRIBA){
        pos_aux = jugador->posicion;
        pos_aux.fil -= 1;
    }
    if(colision_actual(*nivel, pos_aux) == FUEGO){
        nivel->obstaculos[buscar_coordenada_en_objeto(pos_aux, nivel->obstaculos, nivel->tope_obstaculos)] = nivel->obstaculos[nivel->tope_obstaculos -1];
        nivel->tope_obstaculos -= 1;
        jugador->extintores -= 1;
    } else {
        printf("No hay un incendio ahi\n");
        sleep(1);
    }
}

/*
* Funcion que recibe una coordenada
* Y Devuelve si esta en el borde de la matriz
*/
bool esta_en_el_borde(coordenada_t posicion, int size){
    return (posicion.fil == 0 || posicion.fil == size-1 || posicion.col == 0 || posicion.col == size-1)? true:false;
}

/*
* Procedimiento que dada la direccion, intenta usar un martillo en la misma
*/
void usar_martillo(char movimiento, nivel_t* nivel, jugador_t* jugador, int size){
    coordenada_t pos_aux = jugador->posicion;
    if(movimiento == MOV_IZQUIERDA){
        pos_aux.col -=1;
    } else if (movimiento == MOV_ARRIBA){
        pos_aux.fil-=1;
    } else if(movimiento == MOV_DERECHA){
        pos_aux.col+=1;
    } else {
        pos_aux.fil+=1;
    }

    if(colision_actual(*nivel, pos_aux) == PARED && !esta_en_el_borde(pos_aux, size)){
        nivel->paredes[buscar_coordenada_en_coordenada(pos_aux, nivel->paredes, nivel->tope_paredes)] = nivel->paredes[nivel->tope_paredes -1];
        nivel->tope_paredes --;
        jugador->martillos -= 1;
    } else {
        printf("No se puede usar ahi\n");
        sleep(1);
    }
}

/*
* Procedimiento que si el movimiento fue el uso de un extintor o martillo
* consulta al jugador su direccion y si es que se puede utilizar
*/
void usar_herramientas(char movimiento, nivel_t* nivel, jugador_t* jugador, int numero_nivel){
    char mov_herramienta;
    if(movimiento == USAR_EXTINTOR && jugador->extintores > 0){
        printf("Donde queres lanzar el extintor?: \n");
        pedir_movimiento(&mov_herramienta, EXTINTORES);
        if(mov_herramienta == MOV_IZQUIERDA || mov_herramienta == MOV_DERECHA || mov_herramienta == MOV_ARRIBA){
            usar_extintor(mov_herramienta, nivel, jugador);
        } else {
            printf("Movimiento Invalido\n");
            sleep(1);
        }
    }else if(movimiento == USAR_MARTILLO && jugador->martillos > 0){
        printf("Donde queres lanzar el martillazo?: \n");
        pedir_movimiento(&mov_herramienta, MARTILLOS);
        if(mov_herramienta == MOV_IZQUIERDA || mov_herramienta == MOV_DERECHA || mov_herramienta == MOV_ARRIBA || mov_herramienta == MOV_ABAJO){
            usar_martillo(mov_herramienta, nivel, jugador, obtener_size_mapa(numero_nivel));
        } else {
            printf("Movimiento Invalido\n");
            sleep(1);
        }
    } else {
        printf("No tenes suficientes herramientas\n");
        sleep(1);
    }
}

/*
* Funcion que recibe los papeleos
* Devuelve si el nivel actual esta ganado
*/
int estado_nivel(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos){
    int resultado = 1;
    for(int i = 0; i < tope_papeleos; i++){
        if(papeleos[i].recolectado == false){
            resultado = 0;
        }
    }
    return resultado;
}

/*
* Procedimiento que verifica si el nivel se gano o el jugador se quedo sin movimientos
* Deriva a Derrota si se perdio, Victoria si se gano el ultimo nivel, o subir de nivel en el caso contrario
*/
int estado_juego(juego_t juego){
    int resultado = 0;
    if(estado_nivel(juego.niveles[juego.nivel_actual -1].papeleos, juego.niveles[juego.nivel_actual -1].tope_papeleos) == 1 && juego.jugador.movimientos > 0){
        if(juego.nivel_actual == MAX_NIVELES){
            resultado = 1;
        }
    } else if (juego.jugador.movimientos <= 0){
        resultado = -1;
    }
    return resultado;
}

/*
* Procedimiento que toma todas las posibilidades de input del jugador
* Verifica y aplica los movimientos en caso de que se pueda
*/
void realizar_jugada(juego_t* juego){
    char movimiento;
    int size = obtener_size_mapa(juego->nivel_actual);
    inicializar_controlador();
    pedir_movimiento(&movimiento, VACIO);
    terminar_controlador();
    if(es_mov_valido(movimiento, juego->niveles[juego->nivel_actual-1], juego->jugador.posicion, size) == true){
        juego->jugador.movimientos = (movimiento == MOV_IZQUIERDA || movimiento == MOV_DERECHA || movimiento == ROT_ANTIHORARIA || movimiento == ROT_HORARIA)? juego->jugador.movimientos -1 : juego->jugador.movimientos;
        juego->jugador.movimientos_realizados = (movimiento == MOV_IZQUIERDA || movimiento == MOV_DERECHA || movimiento == ROT_ANTIHORARIA || movimiento == ROT_HORARIA)? juego->jugador.movimientos_realizados  +1: juego->jugador.movimientos_realizados;
        if(movimiento == USAR_EXTINTOR || movimiento == USAR_MARTILLO){
            usar_herramientas(movimiento, &juego->niveles[juego->nivel_actual-1], &juego->jugador, juego->nivel_actual);
        } else {
            acciones_por_movimiento(&juego->niveles[juego->nivel_actual-1], juego->jugador, juego->nivel_actual);
        }
        aplicar_movimiento(movimiento, &juego->jugador, &juego->niveles[juego->nivel_actual-1], size);
        comprobar_colision(&juego->niveles[juego->nivel_actual-1], &juego->jugador);
        aplicar_efecto_gravitacional(&juego->niveles[juego->nivel_actual-1], &juego->jugador, size, false);
        if(estado_nivel(juego->niveles[juego->nivel_actual-1].papeleos, juego->niveles[juego->nivel_actual-1].tope_papeleos) == 1 && juego->nivel_actual < NIVEL_TRES){
            subir_de_nivel(juego);
        }
    }
}
