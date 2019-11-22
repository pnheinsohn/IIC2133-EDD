#include "snek.h"
#include <stdio.h>
#include <stdlib.h>

/** Crea una copia de un nodo */
Node* node_copy(Node* node) {
  // Crea el nodo
  Node* copy = malloc(sizeof(Node));

  // Copio los valores
  copy->direction = node->direction;
  copy->row = node->row;
  copy->col = node->col;
  copy->next = NULL;

  // Retorno la copia
  return copy;
}

/** Inicializa un snek vacio */
Snek* snek_init() {
  // Creo el snek
  Snek* snek = malloc(sizeof(Snek));

  // Inicializo sus variables como un snek de largo 0
  snek->size = 0;
  snek->head = NULL;
  snek->tail = NULL;
  snek->grounds = 0;
  snek->spikes = 0;
  snek->hash = 0;

  // Retorno el snek
  return snek;
}

/** Inicializa un nodo con su posicion */
Node* node_init(uint8_t row, uint8_t col) {
  // Creo el nodo
  Node* node = malloc(sizeof(Node));

  // Inicializo su posicion y dejo el resto vacio
  node->row = row;
  node->col = col;
  node->direction = HEAD;
  node->last = NULL;
  node->next = NULL;

  // Retorno el nodo
  return node;
}

/** Agrega el nodo dado a la cola del snek (se llama luego de crear el snek)*/
void snek_expand(Snek* snek, Node* node) {
  if (snek->size == 0) {
    // Si el snek esta vacio, el nodo es head y tail
    snek->head = node;
    snek->tail = node;
    node->direction = HEAD;
  } else {
    // Si ya tengo elementos en el snek, concateno al final
    snek->tail->next = node;
    node->last = snek->tail;
    // Agrego la orientacion correspondiente al nodo
    if (snek->tail->col > node->col)
      node->direction = LEFT;
    else if (snek->tail->col < node->col)
      node->direction = RIGHT;
    else if (snek->tail->row < node->row)
      node->direction = DOWN;
    else
      node->direction = UP;
    // Actualizo el tail del snek
    snek->tail = node;
  }

  // Sumo 1 al largo del snek
  snek->size += 1;
}

/** Mueve la cola del snek a la cabeza con una direccion dada */
void snek_move(Snek* snek, Direction direction) {
  if (snek->size == 1) {
    // Si el snek tiene 1 solo nodo, cambio su posicion segun la direccion
    switch (direction) {
      case UP:
        snek->head->row -= 1;
        break;
      case DOWN:
        snek->head->row += 1;
        break;
      case RIGHT:
        snek->head->col += 1;
        break;
      case LEFT:
        snek->head->col -= 1;
        break;
      case HEAD:  // Este caso no deberia pasar nunca
        break;
    }
  } else {
    // Si tiene mas de un nodo, muevo el ultimo nodo al final de la serpiente

    // Obtengo el nodo que estoy moviendo
    Node* node = snek->tail;

    // Lo desconecto de la serpiente
    snek->tail = node->last;
    snek->tail->next = NULL;
    node->last = NULL;

    // Conecto el nodo al principio
    node->next = snek->head;
    snek->head = node;
    node->next->last = node;

    // Actualizo la direccion del nodo
    node->direction = HEAD;

    // Actualizo la posicion de la cabeza y la direccion del siguiente elemento
    switch (direction) {
      case UP:
        snek->head->row = snek->head->next->row - 1;
        snek->head->col = snek->head->next->col;
        snek->head->next->direction = DOWN;
        break;
      case DOWN:
        snek->head->row = snek->head->next->row + 1;
        snek->head->col = snek->head->next->col;
        snek->head->next->direction = UP;
        break;
      case RIGHT:
        snek->head->row = snek->head->next->row;
        snek->head->col = snek->head->next->col + 1;
        snek->head->next->direction = LEFT;
        break;
      case LEFT:
        snek->head->row = snek->head->next->row;
        snek->head->col = snek->head->next->col - 1;
        snek->head->next->direction = RIGHT;
        break;
      case HEAD:  // Este caso no deberia pasar nunca
        break;
    }
  }
}

/** Desplaza a la serpiente completa hacia abajo la cantidad dada */
void snek_fall(Snek* snek, uint8_t fall) {
  for (Node* node = snek->head; node; node = node->next) {
    // Itero por los nodos de la serpiente desplazandolos
    node->row += fall;
  }
}

/** Hace una copia de la serpiente */
Snek* snek_copy(Snek* snek) {
  // Creo el nuevo snek
  Snek* copy = malloc(sizeof(Snek));

  // Copio los valores de la snek
  copy->grounds = snek->grounds;
  copy->spikes = snek->spikes;
  copy->size = snek->size;
  copy->head = NULL;
  copy->tail = NULL;

  // Copio la cabeza
  copy->head = node_copy(snek->head);
  copy->head->last = NULL;
  copy->tail = copy->head;

  // Copio los nodos
  for (Node* node = snek->head->next; node; node = node->next) {
    // Copio el nodo
    Node* n_copy = node_copy(node);

    // Conecto el nodo al nodo anterior
    n_copy->last = copy->tail;
    copy->tail->next = n_copy;

    // Dejo el nodo creado como cola
    copy->tail = n_copy;
  }

  // Retorno la copia
  return copy;
}

/** Libera los nodos del snek recursivamente de la cabeza a la cola */
void nodes_destroy(Node* node) {
  if (node) {
    // Si el nodo no es nulo, libero el nodo siguiente y luego el actual
    nodes_destroy(node->next);
    free(node);
  }
}

/** Libera la memoria de un snek */
void snek_destroy(Snek* snek) {
  // Libero los nodos
  nodes_destroy(snek->head);

  // Libero el snek
  free(snek);
}
