#pragma once

/** Estructura de un nodo de la lista simplemente ligada */
typedef struct node
{
  /** Nodo sigiente en la lista (NULL si no hay otro nodo) */
  struct node* next;
  /** valor que guarda el nodo */
  int value;
} Node;

/** Lista ligada. Mantiene el nodo inicial y el final */
typedef struct linked_list
{
  /** Nodo inicial de la lista (posicion 0) */
  Node* head;
  /** Nodo final de la lista */
  Node* tail;
} LinkedList;

/////// Funciones publicas de la lista ////////

/** Crea una lista ligada vacia */
LinkedList* ll_init();

/** Destruye la lista ligada */
void ll_destroy(LinkedList* ll);

/** Agrega un valor a la lista */
void ll_append(LinkedList* ll, int value);

/** Ordena la lista ligada usando quicksort */
void ll_quicksort(LinkedList* ll);
