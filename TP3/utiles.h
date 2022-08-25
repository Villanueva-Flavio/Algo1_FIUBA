#ifndef __UTILES_H__
#define __UTILES_H__

#define MAX_PAREDES 500

typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;


/* Pre condiciones: El parametro nivel debe contener el valor de un nivel (1 a 3 inclusives).
 * Post condiciones: Devuelve el vector de coordenadas de las paredes cargado, junto a su respectivo tope, las pos inicial del jugador.
*/
void obtener_paredes(int nivel, coordenada_t paredes[MAX_PAREDES], int* tope_paredes, coordenada_t* pos_inicial_jugador);


/* Pre condiciones: La variable segundos debe contener un valor positivo.
 * Post condiciones: Detendrá el tiempo los segundos indicados como parámetro.
*/
void detener_el_tiempo(float segundos);


#endif /* __UTILES_H__ */