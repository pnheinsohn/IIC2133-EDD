#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
  // Creo una serpiente
  Snake snek;
  snek.size = 5;
  snek.posx = 2;
  snek.posy = 1;
  snek.directions = malloc(sizeof(Dir) * (snek.size - 1));
  snek.directions[0] = UP;
  snek.directions[1] = LEFT;
  snek.directions[2] = DOWN;
  snek.directions[3] = LEFT;

  // Creo un tablero
  Board board;
  board.height = 4;
  board.width = 4;
  board.snake = snek;
  board.randoms = malloc(sizeof(uint64_t*) * board.height);
  for (int i = 0; i < board.height; i++)
  {
    board.randoms[i] = malloc(sizeof(uint64_t) * board.width);
    for (int j = 0; j < board.width; j++)
    {
      board.randoms[i][j] = (uint64_t) rand() * pow(2,32) + (uint64_t) rand();
    }
  }

  // Calculo el hash inicial del tablero
  hash_board(&board);

  // Calculo el hash nuevo del tablero considerando que la serpiente de mueve hacia arriba
  printf("hash = %ld\n", incremental_board(&board, UP));

  // Libero la memoria de los numero random
  for (int i = 0; i < board.height; i++)
  {
    free(board.randoms[i]);
  }
  free(board.randoms);

  // Libero el arreglo de direcciones de la serpiente
  free(snek.directions);

  return 0;
}
