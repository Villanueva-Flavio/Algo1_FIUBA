#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define OLAF 'O'
#define STITCH 'S'
#define JASMIN 'J'
#define RAYO_MCQUEEN 'R'

#define TRINEO 'T'
#define NAVE_ESPACIAL 'N'
#define CAMION 'C'
#define ALFOMBRA 'A'

#define BOLAS_DE_NIEVE 'B'
#define PIUM_PIUM 'P'
#define KUCHAU 'K'
#define GENIO 'G'

#define MAX_MINUTOS 60
#define MAX_HORAS 24
#define MIN_HORAS 0
#define MIN_MINUTOS 0

#define MIN_NOTA 1
#define MAX_NOTA 10
#define HORA_EXAMEN 1530
#define LIMITE_MEDIO_PUNTOS 41
#define INT_VACIO 0
#define CHAR_VACIO 'V'

#define VALOR_DE_TRINEO_Y_NAVE 10
#define VALOR_DE_CAMION_Y_ALFOMBRA 20

#define VALOR_DE_BOLA_DE_NIEVE 3
#define VALOR_DE_PIUM_PIUM 6
#define VALOR_DE_KUCHAU 9
#define VALOR_DE_GENIO 12

/*
* Procedimiento que muestra al jugador el resultado de sus decisiones
*/
void imprimir_personaje(char personaje, int puntos){
	switch (personaje){
		case STITCH:
			printf("Con un puntaje de %i puntos se definio el personaje -Stitch-.\n", puntos);
			break;
		case OLAF:
			printf("Con un puntaje de %i puntos se definio el personaje -Olaf-.\n", puntos);
			break;
		case JASMIN:
			printf("Con un puntaje de %i puntos se definio el personaje -Jasmín-.\n", puntos);
			break;
		case RAYO_MCQUEEN:
			printf("Con un puntaje de %i puntos se definio el personaje -Rayo McQueen-.\n", puntos);
			break;
	}
}

/*
* Funcion que recibe el numero de pregunta y sus respuestas (VACIO si no tiene)
* y Devuelve Verdadero o Falso si corresponde a la pregunta
*/
bool respuesta_valida(int numero_pregunta, char respuesta_textual, int respuesta_numerica_a, int respuesta_numerica_b){
	bool resultado = true;
	switch(numero_pregunta){
		case 1:
			if(respuesta_textual != TRINEO && respuesta_textual != NAVE_ESPACIAL && respuesta_textual != CAMION && respuesta_textual != ALFOMBRA){
				resultado = false;
			}
			break;
		case 2:
			if(respuesta_textual != BOLAS_DE_NIEVE && respuesta_textual != PIUM_PIUM && respuesta_textual != KUCHAU && respuesta_textual != GENIO){
				resultado = false;
			}
			break;
		case 3:
			if(respuesta_numerica_a < MIN_HORAS || respuesta_numerica_a >= 24 || respuesta_numerica_b < MIN_MINUTOS || respuesta_numerica_b >= MAX_MINUTOS){
				resultado = false;
			}
			break;
		case 4:
			if(respuesta_numerica_a < MIN_NOTA || respuesta_numerica_a > MAX_NOTA){
				resultado = false;
			}
			break;
	}
	return resultado;
}

/*
* Funcion que recibe el resultado de todas las respuestas
* y devuelve una ecuacion realizada
*/
int puntaje_total(int primera_respuesta, int segunda_respuesta, int cuarta_respuesta){
	int primer_resultado, segundo_resultado;
	if(primera_respuesta == TRINEO || primera_respuesta == NAVE_ESPACIAL){
		primer_resultado = VALOR_DE_TRINEO_Y_NAVE;
	} else {
		primer_resultado = VALOR_DE_CAMION_Y_ALFOMBRA;
	}
	switch (segunda_respuesta){
		case 'B':
			segundo_resultado = VALOR_DE_BOLA_DE_NIEVE;			
			break;
		case 'P':
			segundo_resultado = VALOR_DE_PIUM_PIUM;			
			break;
		case 'K':
			segundo_resultado = VALOR_DE_KUCHAU;			
			break;
		case 'G':
			segundo_resultado = VALOR_DE_GENIO;			
			break;
	}
	return primer_resultado + (segundo_resultado * cuarta_respuesta);
}

/*
* Procedimiento que le pide al jugador responder una pregunta
*/
void primera_pregunta(char* respuesta){
	
	printf("Estas saliendo de tu casa, y te enteras que hay paro de trenes, Que otro transporte elegis?\n");
    printf("(T) Trineo, (N) Nave Espacial, (C) Camion, (A) Alfombra\n");
	scanf(" %c", respuesta);
	while(respuesta_valida(1, *respuesta, INT_VACIO, INT_VACIO) == false){
		printf("Ingresa una opcion valida\n");
		scanf(" %c", respuesta);
	}
}

/*
* Procedimiento que le pide al jugador responder una pregunta
*/
void segunda_pregunta(char* respuesta){
	
	printf("En medio del camino te encontras con una manifestacion, Que medio utilizas para deshacerte de la manifestacion?\n");
    printf("(B) Bolas de nieve, (P) Pium Pium, (K) Kuchau, (G) Genio\n");
	scanf(" %c", respuesta);
	while(respuesta_valida(2, *respuesta, INT_VACIO, INT_VACIO) == false){
		printf("Ingresa una opcion valida\n");
		scanf(" %c", respuesta);
	}
}

/*
* Procedimiento que le pide al jugador responder una pregunta
*/
void tercera_pregunta(double* respuesta){
	
	printf("Lograste superar la manifestacion, A que hora conseguiste llegar a la universidad?\n");
	scanf("%lf", respuesta);
	int respuesta_entera = (int)*respuesta;
	int respuesta_decimal = (int)(*respuesta * 100) - (respuesta_entera * 100);
	while(respuesta_valida(3, CHAR_VACIO, respuesta_entera, respuesta_decimal)==false){
		printf("Ingresa un numero valido\n");
		scanf("%lf", respuesta);
		respuesta_entera = (int)*respuesta;
		respuesta_decimal = (int)(*respuesta * 100) - (respuesta_entera * 100);
	}
}

/*
* Procedimiento que le pide al jugador responder una pregunta
*/
void cuarta_pregunta(int* resultado){
	
	int respuesta;
	printf("Por fin llego la nota del parcial! Que nota lograste sacarte?\n");
	scanf("%i", &respuesta);
	while(respuesta_valida (4, CHAR_VACIO, respuesta, INT_VACIO) == false){
		printf("Ingresa un numero valido\n");
		scanf("%i", &respuesta);
	}
	switch (respuesta){
		case 1 ... 3 :
			*resultado = 1;
			break;
		case 4 ... 6 :
			*resultado = 2;
			break;
		case 7 ... 8 :
			*resultado = 3;
			break;
		case 9 ... 10 :
			*resultado = 4;
			break;			
	}
}

/*
* Procedimiento que gestiona todas las preguntas en orden, recibe las variables que seran modificadas por cada respuesta
*/
void realizar_cuestionario(int* nota, double* hora_de_llegada, char* vehiculo, char* arma){
	primera_pregunta(vehiculo);
	segunda_pregunta(arma);
	tercera_pregunta(hora_de_llegada);
	cuarta_pregunta(nota);
}

/*
* Procedimiento que recibe el resultado de las cuentas y la hora
* Y procesa el resultado final
*/
void mostrar_personaje(int resultado_cuentitas, double hora_de_llegada, char* amigo){
	
	int fraccion_horas = (int)hora_de_llegada;
	int fraccion_minutos = (int)(hora_de_llegada * 100) - (fraccion_horas * 100);
	int hora = (fraccion_horas * 100) + fraccion_minutos;
	if(resultado_cuentitas <= LIMITE_MEDIO_PUNTOS){
		if(hora <= HORA_EXAMEN){							//arreglar caso borde minutos
			imprimir_personaje(STITCH, resultado_cuentitas);
			*amigo = STITCH;
		} else {
			imprimir_personaje(OLAF, resultado_cuentitas);
			*amigo = OLAF;
		}
	} else {
		if(hora <= HORA_EXAMEN){
			imprimir_personaje(JASMIN, resultado_cuentitas);
			*amigo = JASMIN;
		} else {
			imprimir_personaje(RAYO_MCQUEEN, resultado_cuentitas);
			*amigo = RAYO_MCQUEEN;
		}
	}
}

int main(){
	int nota, resultado_cuentitas;
	double hora_de_llegada;
	char vehiculo, arma, amigo;
	realizar_cuestionario(&nota, &hora_de_llegada, &vehiculo, &arma);
	resultado_cuentitas = puntaje_total(vehiculo, arma, nota);
	mostrar_personaje(resultado_cuentitas, hora_de_llegada, &amigo);
	return 0;
}