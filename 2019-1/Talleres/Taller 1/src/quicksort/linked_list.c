#include <stdlib.h>
#include "linked_list.h"

////// Funciones privadas ///////

/** Crea un nodo a partir de un valor */
static Node* node_init(int value)
{
  // Pido memoria para el nodo
  Node* node = malloc(sizeof(Node));

  // Le asigno el valor correspondiente
  node -> value = value;

  // Hago que apunte al null
  node -> next = NULL;

  // Retorno el nodo
  return node;
}

/** Libera iterativamente la memoria del nodo y los nodos siguientes */
static void node_destroy(Node* node)
{
  // Esta funcion es iterativa porque puede generar stack overflow
  while (node)
  {
    Node* next = node -> next;
    free(node);
    node = next;
  }
}

// Agrega un nodo a la lista ligada
static void ll_add_node(LinkedList* ll, Node* node)
{
  if (!ll -> head)
  {
    // Si la lista no tiene elementos, el nodo es inicial y final
    ll -> head = node;
    ll -> tail = node;
  }
  else
  {
    // Sino, lo agrego al final
    ll -> tail -> next = node;
    ll -> tail = node;
  }
}

/** Hace partition de ll y separa los datos en las otras listas */
static void partition(LinkedList* ll, LinkedList* smallers, LinkedList* equals, LinkedList* biggers)
{
  // Selecciono el pivote como el primer elemento de la lista
  int piv = ll -> head -> value;

  // Itero sobre la lista ll moviendo los nodos a la lista que corresponde
  Node* actual = ll -> head;
  while (actual)
  {
    // Guardo una referencia al nodo siguiente
    Node* next = actual -> next;

    // Desconecto el nodo actual de la lista
    actual -> next = NULL;

    // Veo en que lista va el nodo y lo inserto
    if (actual -> value < piv) ll_add_node(smallers, actual);
    else if (actual -> value == piv) ll_add_node(equals, actual);
    else ll_add_node(biggers, actual);

    // Actualizo el nodo actual
    actual = next;
  }
}

////// Funciones publicas ///////

/** Crea una lista ligada vacia */
LinkedList* ll_init()
{
  // Creo la lista ligada
  LinkedList* ll = malloc(sizeof(LinkedList));

  // Pongo sus punteros en nulo y su contador en 0
  ll -> head = NULL;
  ll -> tail = NULL;

  // Retorno la lista ligada
  return ll;
}

/** Destruye la lista ligada */
void ll_destroy(LinkedList* ll)
{
  // Primero destruyo los nodos de la lista
  node_destroy(ll -> head);

  // Luego libero la memoria de la lista
  free(ll);
}

/** Agrega un valor a la lista */
void ll_append(LinkedList* ll, int value)
{
  // Creo el nodo a insertar
  Node* node = node_init(value);

  // Funcion que agrega un nodo al final de una lista ligada
  ll_add_node(ll, node);
}

/** Ordena la lista ligada usando quicksort */
void ll_quicksort(LinkedList* ll)
{
  // La idea es que al hacer partition se dividan los datos en 3 listas:
  // Menores al pivote, iguales al pivote y mayores al pivote.
  // Luego se ordenan las listas recursivamente, y se unen los nodos en ll

  // Si no tengo elementos solo retorno
  if (!ll -> head) return;

  // Creo las listas a correspondientes
  LinkedList smallers;
  smallers.head = NULL;
  smallers.tail = NULL;
  LinkedList equals;
  equals.head = NULL;
  equals.tail = NULL;
  LinkedList biggers;
  biggers.head = NULL;
  biggers.tail = NULL;

  // Llamo a partition con la lista ligada original y las nuevas listas
  partition(ll, &smallers, &equals, &biggers);

  // Llamo a quicksort recursivamente
  ll_quicksort(&smallers);
  ll_quicksort(&biggers);

  // Concateno los nodos de las 3 listas
  if (smallers.tail)
  {
    // Si existe al menos un nodo en smallers, concateno smallers - equals
    smallers.tail -> next = equals.head;
  }
  if (biggers.head)
  {
    // Si existe al menos un nodo en biggers, concateno equals - biggers
    equals.tail -> next = biggers.head;
  }

  // Agrego los nodos a la lista ll
  ll -> head = smallers.head ? smallers.head : equals.head;
  ll -> tail = biggers.tail ? biggers.tail : equals.tail;
}
