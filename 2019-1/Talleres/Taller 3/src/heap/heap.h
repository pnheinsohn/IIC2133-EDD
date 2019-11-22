#pragma once
#include "object.h"

/** Estructura de un min heap */
typedef struct heap
{
  /** Numero de elementos */
  int count;
  /** Tamanio del array */
  int size;
  /** Arreglo de objetos */
  Object** array;
} Heap;

/** Inicializa un heap vacio */
Heap* heap_init(int size);

/** Inserta un elemento en el heap */
void heap_insert(Heap* heap, Object* obj);

/** Elimina la cabeza del heap y la retorna */
Object* heap_pop(Heap* heap);

/** Ordena un arreglo de objetos usando un heap */
void heapsort(Object** array, int size);

/** Destruye el heap */
void heap_destroy(Heap* heap);
