#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "../random/pcg_basic.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Debes dar como input el numero de elementos a ordenar\n");
    return 0;
  }

  // Seteamos una semilla para los numeros aleatorios
	pcg32_srandom(0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL);

  // Leemos el numero de elementos a ordenar
  int size = atoi(argv[1]);

  // Creo un arreglo de tamanio size de objetos
  Object** array = malloc(sizeof(Object*) * size);
  for (int i = 0; i < size; i++)
  {
    array[i] = obj_init(pcg32_boundedrand(size), pcg32_boundedrand(size));
  }

  // Ordeno usando heapsort
  heapsort(array, size);

  // Imprimo el arreglo y libero los objetos
  for (int i = 0; i < size; i++)
  {
    printf("%d %d\n", array[i] -> key, array[i] -> value);
    obj_destroy(array[i]);
  }
  free(array);

  return 0;
}
