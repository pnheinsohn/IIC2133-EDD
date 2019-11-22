#pragma once
#include <stdint.h>

/** Enum de direcciones */
typedef enum direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
} Dir;

/** Estructura de una serpiente */
typedef struct snake
{
  /** Posicion x de la cabeza de la serpiente */
  int posx;
  /** Posicion y de la cabeza de la serpiente */
  int posy;
  /** Tamanio de la serpiente (1 + numero de direcciones) */
  int size;
  /** Arreglo de direcciones de la serpiente desde la cabeza a la cola */
  Dir* directions;
  /** Hash actual de la serpiente */
  uint64_t hash;
} Snake;

/** Estructura del tablero */
typedef struct board
{
  /** Sepiente */
  Snake snake;
  /** Alto de tablero */
  int height;
  /** Ancho del tablero */
  int width;
  /** Matriz de numeros random para calcular hash */
  uint64_t** randoms;
  /** Hash del estado actual */
  uint64_t hash;
} Board;

/** Calcula el hash inicial de la serpiente segun su forma */
uint64_t hash_snake(Snake snake);

/** Calcula el hash inicial del tablero */
void hash_board(Board* board);

/** Calcula el hash incremental del tablero dada una direccion de la serpiente */
uint64_t incremental_board(Board* board, Dir movement);
