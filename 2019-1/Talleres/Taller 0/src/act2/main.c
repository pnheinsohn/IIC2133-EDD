/****************************************************************************/
/*                              Acto 2: Punteros                            */
/****************************************************************************/

/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>

/* Suma 1 a la variable ingresada (no funciona) */
void sumar_1_sin_puntero(uint8_t i)
{
  // Esto no funciona ya que al llamar sumar_1_sin_puntero(x) lo primero que se hace es
  // uint8_t i = x;
  // (Y así con cada parametro de la función)
  // i es entonces una variable local de esta función
  // Al modificar i, no estamos modificando x. Son variables distintas.
  i += 1;
}

/* Suma 1 a la variable en la dirección dada */
void sumar_1_con_puntero(uint8_t* i)
{
  // Aqui pasa lo mismo que en la otra función, al llamar sumar_1_con_puntero(y)
  // lo primero que se hace es
  // uint8_t* i = y;
  // (Y así con cada parametro de la función)
  // i es entonces una variable local de esta función
  // Pero tanto i como y apuntan al mismo lugar, por lo que si accedemos y modificamos
  // el cambio persiste al salir de la función
  *i += 1;
  // Al modificar i, no estamos modificando y. Son variables distintas.
  i += 1;
}

/** Intercambia los valores de dos variables */
void swap(uint32_t* a, uint32_t* b)
{
  uint32_t aux = *a;
  *a = *b;
  *b = aux;
}

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  /* Imprime el mensaje en la consola */
  printf("Bienvenido al Acto 2: Punteros\n");

  // Las variables de nuestro programa están guardadas en la memoria RAM
  uint8_t x = 89;

  // Un puntero a una variable de cierto tipo es simplemente 
  // la dirección de memoria donde está guardada
  // La dirección de memoria de una variable se obtiene anteponiendo & a su nombre
  // Los punteros se escriben como tipo*
  uint8_t* y = &x;

  // %p = puntero
  printf("En la dirección %p el valor guardado es %d\n", y, x);

  // Para acceder a la información guardada en un puntero, anteponemos * a su nombre
  printf("En la dirección %p el valor guardado es %d\n", y, *y);

  // Esto tambien nos permite modificar el valor de la variable x
  *y = 92;
  printf("x = %d\n", x);

  // Y tambien nos permite modificar la variable dentro de una función
  sumar_1_con_puntero(y);
  printf("x = %d\n", x);

  // Haciendolo sin puntero no pasa nada
  sumar_1_sin_puntero(x);


  // No olvidar que los punteros son a su vez variables con propio tipo.
  // Eso significa que tienen su lugar donde están guardados en memoria
  // Y por lo tanto podemos hacer punteros a punteros.
  // Como se dijo antes, un puntero a un tipo se escribe como tipo*
  // Esto se puede repetir sin limite
  // Por ejemplo:

  // Un puntero a un puntero a un uint8_t
  uint8_t** y2 = &y;
  // Un puntero a un puntero a un puntero a un uint8_t
  uint8_t*** y3 = &y2;

  // El operador & no se puede repetir para obtener el puntero del puntero

  // Pero si es posible repetir el operador * para acceder al puntero de un puntero
  // Hay que pensarlo como que hay que "neutralizar" los * por el otro lado 
  // para llegar al tipo original.

  // Desde y2, ya que es un uint8_t**, a x se accede con **
  // Si usaramos un solo * estaríamos accediendo a "y"
  printf("x = %d\n", **y2);
  // Desde y3, ya que es un uint8_t***, a x se accede con ***
  // Si usaramos un solo * estariamos accediendo a "y2"
  printf("x = %d\n", ***y3);
  
  
  uint32_t A = 12379;
  uint32_t B = 19832;

  swap(&A, &B);

  printf("A = %d\nB = %d\n", A, B);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////  PELIGRO  /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // OJO: los punteros son direcciones de memoria, asi que desde un punto de 
  // vista numérico son todos equivalentes: numeros desde el 0 hasta el tamaño de la RAM

  // Esto es legal, siendo que "y" es un uint8_t*
  uint8_t* w = &y;
  // Al acceder al puntero w, lo unico que hace el compilador es interpretar 
  // lo que hay ahi como un uint8_t. Esto hace importante el tipo del puntero:
  // Le indica al compilador como interpretar ese valor.
  // (Los distintos tipos de datos estan guardados de manera distinta en memoria)

  // En este caso, el valor con el que estaríamos leyendo es algo impredecible.
  printf("%d\n", *w);

  // Por esto es muy importante fijarse en el numero de asteriscos que tiene el puntero
  // que quieren leer, y cual es el tipo original donde está guardada la información.

  return 0;
}
