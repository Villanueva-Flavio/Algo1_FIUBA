
![enter image description here](https://i.pinimg.com/originals/df/ef/52/dfef52bc718c0e35ded5ad5eb80da4bb.jpg)

**Aclaracion:**
Los trabajos prácticos fuerzan el uso de las buenas practicas en su totalidad, por lo que todo lo que se compile con GCC debe poder cumplir la condición de poder usar los flags:

    -Wall -Werror -Wconversion -lm -std=c99

**Ejecución:**

Desde la carpeta TP1 o TP2 o TP3


    gcc *.c *.o FLAGS(opcional) -o juego
    ./juego



**Resumen de Trabajos prácticos:**

 - TP1: Un dia en la uni
	 - 
	 **Descripción:**
Cuestionario que trata sobre responder unas preguntas, y acorde a lo respondido, se obtiene un personaje que lo identifica.

	**Jugabilidad:**
	El usuario debe responder con la letra correspondiente en mayúsculas por STDIN, y recibirá que personaje le corresponde

	**Desarrollado:**
Punteros, cambios explícitos de variable, funciones

	**Nota de TP:**
	7/10

 - TP2: No ordenaste tu papeleo anoche
	 - 
	 **Descripción:**
	 Habiendo recibido el personaje del Trabajo Practico 1, hay que rellenar las estructuras de datos que complementan a la inicialización del juego con sus respectivos niveles y el jugador, para dejarlo listo para ser jugable.
	
	**Jugabilidad:**
	Nada agregado

	**Desarrollado:**
	Estructuras, archivos binarios, crear librerías, matrices y vectores

	**Nota del TP:**
7/10

 - TP3: Mike en el multiverso de los papeleos
	 - 
	 **Descripción:**
	 Con el juego ya inicializado, solo resta que sea jugable, poder perder, ganar, colisionar objetos, tener gravedad, rotar matrices, e incluso recibir efectos acorde al objeto con el que se colisiona y eliminarlo de ser necesario

	**Jugabilidad:**

 	(Los controles están en una interfaz dentro del juego) 
 	Completado el cuestionario, el usuario debe recolectar todos los papeleos: 
 📜 en el orden solicitado, el siguiente papel que debe recolectar esta indicado
 en verde.

	Puede moverse en: 
	> Izquierda ( A )
	> Derecha ( D )
	> Rotar mapa antihorario ( Q )
	> Rotar mapa horario ( E )
	> Usar Martillo ( Z ) -> (Dirección, menos pared borde)
	> Usar Extintor ( C ) -> (Dirección del fuego)
	
	

Una vez recogido todos los papeleos, se avanzará al nivel siguiente.

* El juego consta de 3 niveles. 	
* Se gana con finalizar el tercer nivel.
* Se pierde si se queda sin movimientos. 
* En el mapa existen "Medias" que restaran movimientos al jugador
* El "Fuego" te hace perder directamente. 
* De lo contrario, las botellas aumentan movimientos.
* El interruptor evita que Randall mueva los papeleos constantemente.
>



	

  **Desarrollado:** 	
  Operaciones aritméticas, ordenamiento, lectura de STDIN directa,
  modificación de estructuras, matrices y vectores,   lógica del juego.
   
  

 **Nota del TP:** 	
 10/10
