#include <stdio.h>
#include <stdlib.h>
#include "backtracking.h"

int main(int argc, char*argv[])
{
  if (argc != 2)
  {
    printf("Modo de uso: ./cindy n\n");
    return 0;
  }

  // Leo el n
  int n = atoi(argv[1]);

  // Creo el tablero
  Board* board = board_init(n);

  // Hago backtracking
  int* solution = backtracking(board, 0);

  // Si hay solucion
  if (solution)
  {
    // Imprimo la solucion
    for (int i = 0; i < n * n + 2 * n; i++)
    {
      printf("%d ", solution[i]);
    }
    printf("\n");

    // Libero el arreglo de solucion
    free(solution);
  }
  else
  {
    printf("No hay solucion\n");
  }

  // Libero el tablero
  board_destroy(board);

  // Termino el programa
  return 0;
}
