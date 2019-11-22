#include <stdio.h>
#include <stdlib.h>
// Librería estándar de tiempo
#include <time.h> 

#include "linked_list.h"

int main(int argc, char *argv[])
{
  // Compruebo que se llame correctamente al programa
  if (argc != 5)
  {
    printf("Modo de uso incorrecto. Ingresar './quicksort min max delta repetition'\n");
    return 0;
  }

  // Iteraremos sobre la cantidad de elementos (N) a ordenar
  // en la lista ligada. Para esto comenzaremos desde un N min
  // hasta un N max, aumentando en delta por cada iteración. Por
  // cada N, ordenamos 'repetition' listas para obtener un promedio.

  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  int delta = atoi(argv[3]);
  int repetition = atoi(argv[4]);

  // Para cada N desde min hasta max
  int N = min;
  while(N < max){

    // Imprimo mi N
    printf("%d,", N);
    
    // Por cada repetición (repetition)
    for(int k = 0; k < repetition; k++)
    {

      // Creo una lista
      LinkedList* ll = ll_init();

      // Agrego N numeros aleatorios a la lista entre 0 y 10000
      for (int j = 0; j < N; j++)
      {
        ll_append(ll, rand() % 1000);
      }

      // Tomamos el tiempo que nos demoramos en ordenar la lista
      clock_t t; 
      t = clock();

      // Ordenamos la lista 
      ll_quicksort(ll);

      t = clock() - t; 
      double time = ((double)t)/CLOCKS_PER_SEC;

      // Imprimimos el tiempo
      printf("%f,", time); 

      // Eliminamos la lista (libero memoria)
      ll_destroy(ll);
    }

    //Imprimimos salto de línea
    printf("\n");



    // Aumentamos N en delta
    N += delta;
  }

  // Imprimo lista
  // for (Node* node = ll -> head; node; node = node -> next)
  // {
  //   printf("%d\n", node -> value);
  // }

  return 0;
}