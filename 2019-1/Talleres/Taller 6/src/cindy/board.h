#pragma once
#include <stdbool.h>

/** Tipos de casillas */
typedef enum cell
{
  EMPTY,
  BLUE,
  RED
} Cell;

/** Tablero de cindy */
typedef struct board
{
  /** Cantidad de piezas de cada color */
  int n;
  /** Numero de celdas */
  int size;
  /** Celdas */
  Cell* cells;
  /** Posicion de la celda vacia */
  int empty;
  /** Celdas rojas en posiciones azules */
  int red_wins;
  /** Celdas azules en posiciones rojas */
  int blue_wins;
} Board;

/** Inicializa el tablero con n de cada color y una celda vacia */
Board* board_init(int n);

/** Retorna True si el movimiento de la posicion dada es valido */
bool is_valid(Board* board, int position);

/** Mueve la pieza en la posicion dada a la casilla vacia del tablero */
void board_move(Board* board, int position);

/** Retorna True si el tablero esta en su estado solucion */
bool board_is_win(Board* board);

/** Libera el tablero */
void board_destroy(Board* board);
