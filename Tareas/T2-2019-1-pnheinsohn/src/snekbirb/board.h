#pragma once
#include "snek.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

/** Elementos posibles que pueden haber en el tablero */
typedef enum celltype {
  EMPTY = 0,
  WALL = 1,
  SPIKE = 2
} CellType;

/** Celda de la matriz */
typedef struct cell {
  /** Tipo de celda */
  CellType type;
  /** ALtura de la celda (distancia a la espina o suelo) */
  uint8_t height;
  /** Tipo de celda a la que cae */
  CellType fall_type;
} Cell;

/** Estructura del tablero */
typedef struct board {
  /** Snek del puzle */
  Snek *snek;
  /** Alto del tablero */
  uint8_t height;
  /** Ancho del tablero */
  uint8_t width;
  /** Matriz de celdas del tablero de height x width */
  Cell **cells;
  /** Fila del portal */
  uint8_t portal_row;
  /** Columna del portal */
  uint8_t portal_col;
  /** Matriz de numeros random para calcular hash */
  uint64_t **randoms;  
  /** Hash del estado actual */
  uint64_t hash;
} Board;

////////////////////////////////////////////////////////////////////////////////
//                                Funciones                                   //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa el tablero a partir del archivo */
Board *board_init(FILE *file);

/** Destruye el tablero liberando todo */
void board_destroy(Board *board);

/** Copia el snek y hace el movimiento, luego retorna el nuevo snek. Si el
movimiento no es valido o mata a la serpiente, retorno NULL */
Snek *board_move(Board *board, Direction dir);

/** Retorna True si en el estado del tablero se resolvio el puzzle */
bool board_is_win(Board *board);

////////////////////////////////////////////////////////
//                      Ayudantia                     //
////////////////////////////////////////////////////////

/** Calcula el hash inicial del snek */
uint64_t hash_snake(Snek* snek);

/** Calcula el hash inicial del tablero */
void hash_board(Board *board);

/** Calcula el hash incremental del tablero dada una direccion */
uint64_t incremental_board(Board *board, Direction movement);
