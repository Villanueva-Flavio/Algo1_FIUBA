#ifndef __CONTROLADOR__
#define __CONTROLADOR__

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/*
 * Pre:
 * Post: Cambia la configuración de la terminal para automatizar la
 * 	  entrada del teclado.
 */
void inicializar_controlador();

/*
 * Pre:
 * Post: Devuelve la terminal a la configuración original.
 */
void terminar_controlador();

#endif /* __CONTROLADOR__ */