#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

////////////////////////////////////////////////////////////////////////////////
//                  Funciones utiles para no escribir tanto                   //
////////////////////////////////////////////////////////////////////////////////

/** Retorna la key del objeto de la posicion dada */
static int key(Heap* heap, int i)
{
  return heap -> array[i] -> key;
}

/** Hace swap de los elementos indicados */
static void swap(Heap* heap, int i1, int i2)
{
  Object* aux = heap -> array[i1];
  heap -> array[i1] = heap -> array[i2];
  heap -> array[i2] = aux;
}

////////////////////////////////////////////////////////////////////////////////
//             Funciones para conservar las propiedades del heap              //
////////////////////////////////////////////////////////////////////////////////

/** Mueve el elemento hacia arriba hasta que este en su posicion correcta */
static void sift_up(Heap* heap, int pos)
{
  // Si ya estoy en la raiz, termino
  if (!pos) return;

  // Posicion del padre
  int father = (pos - 1) / 2;

  // Si el padre es mayor hago swap y sigo iterando
  if (key(heap, father) > key(heap, pos))
  {
    swap(heap, pos, father);
    sift_up(heap, father);
  }
}

/** Mueve el elemento hacia abajo hasta que este en su posicion correcta */
static void sift_down(Heap* heap, int pos)
{
  // Posicion de los hijos
  int left = pos * 2 + 1;
  int right = left + 1;

  // Si estoy en una hoja termino
  if (heap -> count <= left) return;

  // Veo cual de los hijos es menor
  int smaller;
  if (heap -> count == right || key(heap, left) < key(heap, right))
  {
    smaller = left;
  }
  else
  {
    smaller = right;
  }

  // Si la key actual es mayor a la del hijo menor, hago swap y sigo bajando
  if (key(heap, pos) > key(heap, smaller))
  {
    swap(heap, pos, smaller);
    sift_down(heap, smaller);
  }
}

////////////////////////////////////////////////////////////////////////////////
//                 Funciones de uso publico del heap                          //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa un heap vacio */
Heap* heap_init(int size)
{
  Heap* heap = malloc(sizeof(Heap));

  heap -> count = 0;
  heap -> size = size;
  heap -> array = calloc(heap -> size, sizeof(Object*));

  return heap;
}

/** Inserta un elemento en el heap */
void heap_insert(Heap* heap, Object* obj)
{
  // Inserto el elemento al final del heap
  heap -> array[heap -> count] = obj;
  heap -> count += 1;
  // Lo desplazo hasta su posicion correcta del heap
  sift_up(heap, heap -> count - 1);
}

/** Elimina la cabeza del heap y la retorna */
Object* heap_pop(Heap* heap)
{
  // Si el heap esta vacio retorno NULL
  if (!heap -> count) return NULL;

  // Obtengo la cabeza del heap
  Object* obj = heap -> array[0];
  // Disminuyo el contador de elementos en 1
  heap -> count -= 1;

  // Si quedan elementos, remplazo la cabeza por el ultimo elemento
  if (heap -> count > 0)
  {
    heap -> array[0] = heap -> array[heap -> count];
    heap -> array[heap -> count] = NULL;
    // Restauro la propiedad de heap
    sift_down(heap, 0);
  }
  else
  {
    // Si no quedan elementos, seteo la cabeza en NULL
    heap -> array[0] = NULL;
  }

  // Retorno el elemento que elimine
  return obj;
}

/** Destruye el heap */
void heap_destroy(Heap* heap)
{
  free(heap -> array);
  free(heap);
}

/** Ordena un arreglo de objetos usando un heap */
void heapsort(Object** array, int size)
{
}
