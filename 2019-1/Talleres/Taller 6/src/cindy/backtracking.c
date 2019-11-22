#include <stdlib.h>
#include "backtracking.h"

/** Funcion de backtracking */
int* backtracking(Board* board, int i)
{
  // Si solucione el puzle, retorno la solucion
  if (board_is_win(board))
  {
    // Creo arreglo de solucion y lo retorno
    return malloc(sizeof(int) * i);
  }

  // Itero sobre las celdas que podrian moverse
  int mov[4] = {-2, 2, 1, -1};
  for (int m = 0; m < 4; m++)
  {
    // Posicion a mover
    int pos = board -> empty + mov[m];

    // Si el movimiento no es valido, no hago nada
    if (!is_valid(board, pos)) continue;

    // Almaceno la posicion vacia
    int empty = board -> empty;

    // Hago el movimiento
    board_move(board, pos);

    // Veo si puedo resolver el puzle recursivamente
    int* solution = backtracking(board, i + 1);
    if (solution)
    {
      // Agrego el paso que hice a la solucion
      solution[i] = pos;
      return solution;
    }

    // Deshago el movimiento
    board_move(board, empty);
  }

  // Retorno false porque no encontre un movimiento que me lleve a la solucion
  return NULL;
}
