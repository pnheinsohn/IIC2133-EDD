// Esta linea de codigo hace que no hayan problemas al importar un mismo archivo
// desde otros archivos
#pragma once

/** Estructura de un nodo de la lista ligada. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct node
{
  // En este caso no podemos referirnos a la estructura como Node ya que aun
  // no esta completo el typedef

  /** Puntero al nodo anterior */
  struct node* last;
  /** Puntero al nodo siguiente */
  struct node* next;
  /** Numero almacenado en el nodo */
  int value;
} Node;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct linked_list
{
  /** Nodo inicial de la lista ligada */
  Node* start;
  /** Nodo final de la lista ligada */
  Node* end;
  /** Contador de elementos de la lista ligada */
  int count;
} LinkedList;

///////////////////////////// Funcione publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
LinkedList* ll_init();

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(LinkedList* ll, int value);

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
int ll_get(LinkedList* ll, int position);

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(LinkedList* ll);
