#include <time.h>
#include <stdlib.h>
#include "papeleo.h"
#include "dia_en_la_uni.h"
#include <stdbool.h>
#include "controlador.h"

/*
* Procedimiento que imprime un mensaje en caso de perder
*/
void derrota(){
    system("clear");
    printf("    :( （ • •）             \n");
    printf("    ╔uu══════════════════╗☆ \n");
    printf("    ❝ <   Perdiste    >  ❞  \n");
    printf("    ╚══════nn════════════╝  \n\n\n\n");
    exit(0);
}

/*
* Procedimiento que imprime un mensaje en caso de ganar
*/
void victoria(){
    system("clear");
    printf("    ☆ （ • •）☆     :3      \n");
    printf("    ╔uu══════════════════╗☆ \n");
    printf("    ❝ <   Ganaste!    >  ❞  \n");
    printf("    ╚══════nn════════════╝  \n\n\n\n");
    exit(0);
}

int main(){
    srand((unsigned)time(NULL));
    juego_t juego;
    char amigo_pide_favor;
    revisar_amigo(&amigo_pide_favor);
    inicializar_juego(&juego, amigo_pide_favor);
    while(true){
        system("clear");
        imprimir_terreno(juego);
        realizar_jugada(&juego);
        switch(estado_juego(juego)){
            case -1:
                derrota();
                break;
            case 1:
                victoria();
                break;
        }
    }
    return 0;
}