#include <time.h>
#include <stdlib.h>
#include "papeleo.h"
#include "dia_en_la_uni.h"
#include <stdbool.h>

int main(){
    srand((unsigned)time(NULL));
    juego_t juego;
    char amigo_pide_favor;
    revisar_amigo(&amigo_pide_favor);
    inicializar_juego(&juego, amigo_pide_favor);
    imprimir_terreno(juego);
    return 0;
}