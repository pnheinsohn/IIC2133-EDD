/****************************************************************************/
/*                              Acto 4: Arreglos                            */
/****************************************************************************/

/** Módulo donde definimos un par de tipos para el Acto 1 */
#include "tipos.h"
/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>
/** Módulo estandar de C, para solicitar memoria adicional */
#include <stdlib.h>

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  // Esto es un arreglo. Es similar a una lista, con las siguientes restricciones:
  // - Todos los elementos de un arreglo deben ser del mismo tipo
  // - El largo de los arreglos es fijo
  // - Los elementos están contiguos en memoria
  uint8_t A[6] = {2, 7, 3, 5, 1, 4};

  // Podemos acceder al i-ésimo elemento con A[i]
  for(int i = 0; i < 6; i++)
  {
    // Esto sirve para leerlo y modificarlo
    A[i] = 9 - A[i];
    printf("A[%d] = %d\n", i, A[i]);
  }

  // En realidad un arreglo no es más que un trozo de memoria contigua
  // La variable A en este caso, más que ser un objeto, es simplemente 
  // el puntero a donde empieza este trozo de memoria
  uint8_t* pA = A;

  // Esto significa que, sí, podemos utilizar los corchetes con un puntero
  for(int i = 0; i < 6; i++)
  {
    pA[i]++;
    printf("pA[%d] = %d\n", i, pA[i]);
  }

  // Esto es porque el operador A[i] es syntactic sugar para
  // *(A + i)
  // El compilador ve el tipo de los elementos del arreglo
  // y sabe por cuanto multiplicar i para avanzar en la memoria
  // Por esto es importante que todos sean del mismo tipo,
  // sino cada celda requeriría un salto de tamaño distinto!

  // Por cierto esto significa que i[A] es válido (pero no recomendado!)
  printf("A[3] = %d\n", A[3]);
  printf("3[A] = %d\n", 3[A]);
  printf("*(A + 3) = %d\n", *(A + 3));

  // En general, la memoria del stack es limitada
  // Queremos poder llamar funciones sin miedo a que el stack se llene
  // por lo que los arreglos grandes por lo general se recomienda crearlos 
  // en el heap

  // Si queremos un arreglo de tamaño N para valores de tamaño T
  // Esto es tan simple como pedir un bloque de memoria de tamaño T*N

  // Arreglo de 6 elementos de tipo uint16_t
  uint16_t* B = malloc(6 * sizeof(uint16_t));

  B[0] = 2;
  B[1] = 7;
  B[2] = 3;
  B[3] = 5;
  B[4] = 1;
  B[5] = 4;

  for(int i = 0; i < 6; i++)
  {
    printf("B[%d] = %d\n", i, B[i]);
  }

  // OJO: Al trabajar con arreglos ten siempre una variable a su lado
  // que indique el largo, ya que no es posible obtenerlo de ninguna forma

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////  PELIGRO  /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // Nada te impide salirte de un arreglo!
  // Dado como funciona A[i], si "i" no está dentro del arreglo
  // vas a terminar accediendo memoria de otras cosas!

  // Leer memoria invalida es malo
  // Imagina que tus calculos dependieran del valor que estas leyendo,
  // terminarías con cualquier cosa!
  printf("A[-1] = %d\n", A[-1]);
  printf("A[6] = %d\n", A[6]);

  // Modificar memoria invalida es aun peor!
  // Ya que tu programa va a fallar debido a un valor extraño en otra variable
  // siendo que tu nunca le asignaste ese valor directamente!
  A[-1] = 151;
  A[6] = A[6] * 2;

  // Otra razon para usar todos tus arreglos en el HEAP:
  // Si accedes a una dirección invalida, VALGRIND siempre te va a avisar
  // (Es posible que avise en caso acceder memoria sin inicializar desde
  // un arreglo del stack pero eso ya es suerte)
  printf("B[-1] = %d\n", B[-1]);
  printf("B[6] = %d\n", B[6]);
  B[-1] = 151;
  B[6] = B[6] * 2;

  free(B);
  return 0;
}
