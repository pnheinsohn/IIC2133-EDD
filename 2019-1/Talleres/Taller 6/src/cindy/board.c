#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/** Inicializa el tablero con n de cada color y una celda vacia */
Board* board_init(int n)
{
  // Creo el tablero
  Board* board = malloc(sizeof(Board));

  // Inicializo el arreglo del tablero
  board -> n = n;
  board -> size = 2 * n + 1;
  board -> cells = malloc(sizeof(Cell) * board -> size);
  for (int p = 0; p < board -> size; p++)
  {
    if (p < n) board -> cells[p] = BLUE;
    else if (p > n) board -> cells[p] = RED;
    else board -> cells[p] = EMPTY;
  }

  // Posicion vacia
  board -> empty = n;

  // Numero de celdas que llegaron al otro lado
  board -> blue_wins = 0;
  board -> red_wins = 0;

  // Retorno el tablero
  return board;
}

/** Retorna True si el movimiento de la posicion dada es valido */
bool is_valid(Board* board, int position)
{
  // Si la posicion queda fuera del tablero, retorno false
  if (position < 0 || position >= board -> size) return false;

  // Si la celda a mover es la vacia, retorno false
  if (board -> cells[position] == EMPTY) return false;

  // Ahora veo las condiciones segun color
  if (board -> cells[position] == BLUE)
  {
    // Si esta a la derecha de la posicion vacia, retorno false
    if (position > board -> empty) return false;

    // Si estoy tratando de saltar otra pieza azul, retorno false
    if (position == board -> empty - 2 && board -> cells[position + 1] == BLUE)
    {
      return false;
    }
  }
  else if (board -> cells[position] == RED)
  {
    // Si esta a la izquierda de la posicion vacia, retorno false
    if (position < board -> empty) return false;

    // Si estoy tratando de saltar otra pieza roja, retorno false
    if (position == board -> empty + 2 && board -> cells[position - 1] == RED)
    {
      return false;
    }
  }

  // Si llego hasta aca es porque el movimiento es valido y retorno true
  return true;
}

/** Mueve la pieza en la posicion dada a la casilla vacia del tablero */
void board_move(Board* board, int position)
{
  // Mantengo los cantadores de victoria
  if (board -> cells[position] == BLUE)
  {
    // Si saco la pieza azul del lado derecho, resto un contador
    if (position > board -> n) board -> blue_wins--;

    // Si pongo la pieza azul del lado derecho, sumo un contador
    if (board -> empty > board -> n) board -> blue_wins++;
  }
  else if (board -> cells[position] == RED)
  {
    // Si saco la pieza roja del lado izquierdo, resto un contador
    if (position < board -> n) board -> red_wins--;

    // Si pongo la pieza roja del lado izquierdo, sumo un contador
    if (board -> empty < board -> n) board -> red_wins++;
  }

  // Hago el movimiento
  Cell aux = board -> cells[position];
  board -> cells[position] = board -> cells[board -> empty];
  board -> cells[board -> empty] = aux;

  // Actualizo la posicion vacia
  board -> empty = position;
}

/** Retorna True si el tablero esta en su estado solucion */
bool board_is_win(Board* board)
{
  // Veo si el numero de contadores de win rojos y azules son iguales a n
  return board -> blue_wins == board -> n && board -> red_wins == board -> n;
}

/** Libera el tablero */
void board_destroy(Board* board)
{
  // Libero el arreglo de celdas
  free(board -> cells);

  // Libero el tablero
  free(board);
}
