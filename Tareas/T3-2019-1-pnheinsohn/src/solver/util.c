#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/** Mueve la posición en la dirección especificada */
Position position_shift(Position pos, Direction dir) {
  switch (dir) {
    case UP:
      return (Position){.row = pos.row - 1, .col = pos.col};
    case DOWN:
      return (Position){.row = pos.row + 1, .col = pos.col};
    case LEFT:
      return (Position){.row = pos.row, .col = pos.col - 1};
    case RIGHT:
      return (Position){.row = pos.row, .col = pos.col + 1};
  }

  fprintf(stderr, "Dirección inválida: %u\n", dir);
  abort();
}

/** Indica si dos posiciones son iguales */
bool position_eq(Position pos1, Position pos2) {
  return pos1.row == pos2.row && pos1.col == pos2.col;
}

/** Calcula la distancia más larga entre ambos puntos */
uint8_t longest_distance(Position pos1, Position pos2) {
  uint8_t dist_row = pos1.row > pos2.row ? pos1.row - pos2.row : pos2.row - pos1.row;
  uint8_t dist_col = pos1.col > pos2.col ? pos1.col - pos2.col : pos2.col - pos1.col;

  if (dist_row > dist_col) return dist_row;
  else return dist_col;
}

/** Calcula la distancia manhattan entre ambos puntos */
uint16_t manhattan_distance(Position pos1, Position pos2) {
  uint8_t dist_row = pos1.row > pos2.row ? pos1.row - pos2.row : pos2.row - pos1.row;
  uint8_t dist_col = pos1.col > pos2.col ? pos1.col - pos2.col : pos2.col - pos1.col;

  return dist_row + dist_col;
}

/** Calcula la distancia más corta entre ambos puntos */
uint8_t shortest_distance(Position pos1, Position pos2) {
  uint8_t dist_row = pos1.row > pos2.row ? pos1.row - pos2.row : pos2.row - pos1.row;
  uint8_t dist_col = pos1.col > pos2.col ? pos1.col - pos2.col : pos2.col - pos1.col;

  if (dist_row < dist_col) return dist_row;
  else return dist_col;
}

/** Indica si dos posiciones están alineadas */
bool position_aligned(Position pos1, Position pos2) {
  return pos1.row == pos2.row || pos1.col == pos2.col;
}
/** Entrega la dirección de una posicion a otra. Asume que están alineadas */
Direction position_direction(Position from, Position to) {
  // Estan en la misma fila
  if (from.row == to.row) {
    if (from.col < to.col) {
      return RIGHT;
    } else if (from.col > to.col) {
      return LEFT;
    } else {
      fprintf(stderr, "Calculando dirección de celdas iguales: [%hhu %hhu] -> [%hhu %hhu]\n", from.row, from.col, to.row, to.col);
      abort();
    }
  }
  // Estan en la misma columna
  else if (from.col == to.col) {
    if (from.row < to.row) {
      return DOWN;
    } else if (from.row > to.row) {
      return UP;
    } else {
      fprintf(stderr, "Calculando dirección de celdas iguales: [%hhu %hhu] -> [%hhu %hhu]\n", from.row, from.col, to.row, to.col);
      abort();
    }
  } else {
    fprintf(stderr, "Calculando dirección de celdas no alineadas: [%hhu %hhu] -> [%hhu %hhu]\n", from.row, from.col, to.row, to.col);
    abort();
  }
}
