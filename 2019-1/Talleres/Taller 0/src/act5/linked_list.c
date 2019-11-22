// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>

// Importamos el archivo .h correspondiente
#include "linked_list.h"

//////////////////////////// Funciones privadas ///////////////////////////

// Las funciones que tienen el keyword "static" solo pueden ser llamadas desde
// el archivo en el que fueron definidas. Sirve por ejemplo para hacer funciones
// internas ajenas al uso que se le va a dar a la estructura.
// Por ejemplo la funcion node_init solo se va a usar al agregar elementos a la
// lista ligada pero no se va a llamar desde el programa principal

/** Funcion que crea un nodo de la lista ligada a partir de un valor */
static Node* node_init(int value)
{
  // Pido la memoria para el nodo
  Node* node = malloc(sizeof(Node));

  // Inicializo los referencias en NULL
  node -> last = NULL;
  node -> next = NULL;

  // Le doy el valor correspondiente
  node -> value = value;

  // Retorno el nodo
  return node;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void nodes_destroy(Node* node)
{
  // Si el nodo no es NULL, llama recursivamente al siguiente
  // y libero el nodo actual
  if (node)
  {
    nodes_destroy(node -> next);
    free(node);
  }
}

//////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una lista ligada. La crea vacia inicialmente */
LinkedList* ll_init()
{
  // Primero pido la memoria para la lista ligada
  LinkedList* ll = malloc(sizeof(LinkedList));

  // Luego inicializo en 0 el contador y en NULL las referencias
  ll -> count = 0;
  ll -> start = NULL;
  ll -> end = NULL;

  // Retorno la lista ligada
  return ll;
}

/** Funcion que agrega un elemento al final de la lista ligada */
void ll_append(LinkedList* ll, int value)
{
  // Primero creo un nodo nuevo
  Node* node = node_init(value);

  // Luego lo agrego a la lista ligada
  if (!ll -> count)
  {
    // En el caso de que este vacia la lista dejo el nodo como inicial y final
    ll -> start = node;
    ll -> end = node;
  }
  else
  {
    // En otro caso lo conecto con el ultimo nodo de la lista y lo dejo como
    // nodo final de la lista
    ll -> end -> next = node;
    node -> last = ll -> end;
    ll -> end = node;
  }

  // Sumo 1 al numero de nodos
  ll -> count++;
}

/** Funcion que obtiene el valor de la lista ligada en la posicion dada */
int ll_get(LinkedList* ll, int position)
{
  // Si no hay suficientes nodos, hago un error
  if (position >= ll -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la lista\n");
    // Este comando hace terminar el programa
    exit(1);
  }

  // Me muevo por los nodos hasta encontrar la posicion que busco
  Node* actual = ll -> start;
  for (int i = 0; i < position; i++)
  {
    actual = actual -> next;
  }

  // Retoro el valor del nodo correspondiente
  return actual -> value;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void ll_destroy(LinkedList* ll)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  nodes_destroy(ll -> start);

  // Luego libero la memoria de la lista
  free(ll);
}
