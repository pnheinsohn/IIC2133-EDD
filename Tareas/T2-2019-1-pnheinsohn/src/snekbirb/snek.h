#pragma once
#include <stdint.h>
#include <stdbool.h>

/** Representa una dirección */
enum direction
{
  UP = 0,
  LEFT = 1,
  RIGHT = 2,
  DOWN = 3,
  HEAD = 4
};
typedef enum direction Direction;

/** Entrega el opuesto de una dirección x */
// Direction OPPOSITE(Direction x);

#define OPPOSITE(x) (3 - (x))

/** Nodo de una serpiente */
typedef struct node
{
  /** Direccion del nodo con respecto al anterior */
  Direction direction;
  /** Fila del nodo */
  uint8_t row;
  /** Columna del nodo */
  uint8_t col;
  /** Nodo siguiente (hacia la cola) */
  struct node* next;
  /** Nodo anterior (hacia la cabeza) */
  struct node* last;
} Node;

/** Estructura de un snek */
typedef struct snek
{
  /** Largo del snek */
  uint8_t size;
  /** Nodo cabeza */
  Node* head;
  /** Nodo cola */
  Node* tail;
  /** Numero de apoyos en tierra actuales */
  uint8_t grounds;
  /** Numero de apoyos en espinas actuales */
  uint8_t spikes;
  /** Hash actual del snek */
  uint64_t hash;
} Snek;

////////////////////////////////////////////////////////////////////////////////
//                          Funciones de un snek                              //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa un snek vacio */
Snek* snek_init();

/** Inicializa un nodo con su posicion */
Node* node_init(uint8_t row, uint8_t col);

/** Agrega el nodo dado a la cola del snek */
void snek_expand(Snek* snek, Node* node);

/** Mueve la cola del snek a la cabeza con una direccion dada */
void snek_move(Snek* snek, Direction movement);

/** Desplaza a la serpiente hacia abajo la cantidad indicada */
void snek_fall(Snek* snek, uint8_t fall);

/** Hace una copia de la serpiente */
Snek* snek_copy(Snek* snek);

/** Libera los nodos del snek recursivamente de la cabeza a la cola */
void nodes_destroy(Node* node);

/** Libera la memoria de un snek */
void snek_destroy(Snek* snek);
