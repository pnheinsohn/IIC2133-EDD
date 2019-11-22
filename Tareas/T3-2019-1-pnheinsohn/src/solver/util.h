#pragma once

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************/
/*                                  Util                                    */
/*                                                                          */
/* Este módulo contiene funciones que pueden serte útiles para el manejo de */
/* coordenadas y direcciones                                                */
/****************************************************************************/

/** Representa una posición en el tablero */
typedef struct {
  /** Fila dentro de la matriz */
  uint8_t row;
  /** Columna dentro de la matriz */
  uint8_t col;

} Position;

/** Representa una dirección en la que se puede propagar un bloque */
typedef enum {
  UP = 0,
  LEFT = 1,
  RIGHT = 2,
  DOWN = 3,
} Direction;

/** Obtiene la posición al moverse en la dirección especificada */
Position position_shift(Position pos, Direction dir);
/** Calcula la distancia más larga entre ambos puntos */
uint8_t longest_distance(Position pos1, Position pos2);
/** Calcula la distancia más corta entre ambos puntos */
uint8_t shortest_distance(Position pos1, Position pos2);
/** Calcula la distancia manhattan entre ambos puntos */
uint16_t manhattan_distance(Position pos1, Position pos2);
/** Indica si dos posiciones son iguales */
bool position_eq(Position pos1, Position pos2);
/** Indica si dos posiciones están alineadas */
bool position_aligned(Position pos1, Position pos2);
/** Entrega la dirección de una posicion a otra. Asume que están alineadas */
Direction position_direction(Position from, Position to);
