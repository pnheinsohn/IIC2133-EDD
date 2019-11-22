/****************************************************************************/
/*                              Acto 5: Listas                              */
/****************************************************************************/

/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>

// Archivo de listas ligadas
#include "linked_list.h"

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  /* Imprime el mensaje en la consola */
  printf("Bienvenido al Acto 5: Listas\n");

  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct LinkedList esta definida en el
  // archivo linked_list.h junto con todas sus funciones publicas. En le archivo
  // linked_list.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la lista ligada
  LinkedList* ll = ll_init();

  // Agrego 10 elementos a la lista ligada
  for (int i = 0; i < 10; i++)
  {
    ll_append(ll, i);
  }

  // Imprimo el elemento de la posicion 5
  printf("El elemento en la posicion %d es %d\n", 5, ll_get(ll, 5));

  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(ll);

  // Como ejercicio puedes probar el programa usando valgrind para ver que no
  // hay leaks de memoria y luego eliminar la linea que llama a ll_destroy para
  // ver que detecta los leaks de memoria de la lista 

  return 0;
}
