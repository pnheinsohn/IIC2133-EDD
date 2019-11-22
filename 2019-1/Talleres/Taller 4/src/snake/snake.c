#include "snake.h"
#include <math.h>

/** Calcula el hash inicial de la serpiente segun su forma */
uint64_t hash_snake(Snake snake)
{
  // El hash de la serpiente es equivalente al de strings con base = 4

  // Inicialmente tengo hash 0
  uint64_t hash = 0;

  // El factor es 4^potencia, inicialmente potencia = 0 por lo que factor = 1
  uint64_t factor = 1;

  // Itero por el string sumando los elementos por las potencias
  for (int i = snake.size - 2; i >= 0; i--)
  {
    // Sumo el valor del caracter multiplicado por la potencia correspondiente
    hash += factor * snake.directions[i];

    // Aumento la potencia
    factor *= 4;
  }

  // Retorno el hash
  return hash;
}

/** Calcula el hash inicial del tablero y lo almacena en el tablero */
void hash_board(Board* board)
{
  // Calculo el hash de a serpiente
  uint64_t snake_h = hash_snake(board -> snake);

  // Actualizo el hash en la serpiente
  board -> snake.hash = snake_h;

  // Retorno el xor entre h(snake) y el numero random de la posicion de la cabeza
  board -> hash =  snake_h ^ board -> randoms[board -> snake.posx][board -> snake.posy];
}

/** Calcula el hash incremental de la serpiente dada una direccion de la serpiente */
uint64_t incremental_snake(Snake* snake, Dir movement)
{
	// Si la serpiente se mueve hacia una dirección, la cabeza apuntará al cuerpo
	// en la dirección opuesta.
	Dir in = (movement + 2) % 4;

	// Utilizamos base de 4 (Ya que tenemos 4 direcciones: UP, DOWN, LEFT, RIGHT).

	int base = 4;

	// El exponente será el índice de la última dirección.

	int exp = snake -> size - 2;

	// La dirección a eliminar es la última de nuestro array directions.
	Dir out = snake->directions[snake->size - 2];

  // Hash nuevo parto desde el anterior
  uint64_t hash = snake->hash;

  // Calculo b^exp
  uint64_t factor = pow(base, exp);

  // Primero resto el aporte de la última dirección
  hash -= out;

  // Luego divido todo por la base
  hash /= base;

  // Finalmente sumo el aporte de la nueva dirección
  hash += in * factor;

  return hash;
}

/** Calcula el hash incremental del tablero dada una direccion de la serpiente */
uint64_t incremental_board(Board* board, Dir movement)
{

  // Calculamos el nuevo hash de la serpiente según su forma
  uint64_t hash = incremental_snake(&board->snake, movement);

  int x;
  int y;
  // Calculamos la posición de la serpiente
  if(movement == UP)
  {
    x = board->snake.posx;
    y = board->snake.posy - 1;
  }
  else if(movement == RIGHT)
  {
    x = board->snake.posx + 1;
    y = board->snake.posy;
  }
  else if(movement == DOWN)
  {
    x = board->snake.posx;
    y = board->snake.posy + 1;
  }
  else
  {
    x = board->snake.posx - 1;
    y = board->snake.posy;
  }

  // Retornamos el hash de la serpiente XOR número random de la casilla
  return board->randoms[x][y] ^ hash;
}
