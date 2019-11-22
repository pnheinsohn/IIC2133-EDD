#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "../watcher/watcher.h"

/** Inicializa el tablero a partir de un archivo */
Board *board_init(char *filename)
{
	Board *board = malloc(sizeof(Board));

	FILE *file = fopen(filename, "r");

	fscanf(file, "%d", &board->height);
	fscanf(file, "%d", &board->width);

	board->matrix = calloc(board->height, sizeof(int *));

	for (int row = 0; row < board->height; row++)
	{
		board->matrix[row] = calloc(board->width, sizeof(int));

		for (int col = 0; col < board->width; col++)
		{
			int aux;
			fscanf(file, "%d", &aux);
			board->matrix[row][col] = aux;
		}
	}

	fclose(file);

	return board;
}

/** Clona un tablero */
Board *board_clone(Board *other)
{
	Board *board = malloc(sizeof(Board));
	board->height = other->height;
	board->width = other->width;

	board->matrix = calloc(board->height, sizeof(int *));
	for (int row = 0; row < board->height; row++)
	{
		board->matrix[row] = calloc(board->width, sizeof(int));

		for (int col = 0; col < board->width; col++)
		{
			board->matrix[row][col] = other->matrix[row][col];
		}
	}
	return board;
}

/** Se asegura de acceder a las celdas de la cuadrícula de forma toroidal */
static int board_cell(Board *board, int row, int col)
{
	/* Se le suma la dimension para eliminar numeros negativos */
	int real_row = (row + board->height) % board->height;
	int real_col = (col + board->width) % board->width;

	return board->matrix[real_row][real_col];
}

/** Cuenta la cantidad de vecinos enemigos que le ganan para una celda */
static int neighbour_count(Board *board, int row, int col, int value)
{
	int count = 0;
	/* De arriba hacia abajo */
	for (int drow = -1; drow <= 1; drow++)
	{
		/* De izquierda a derecha */
		for (int dcol = -1; dcol <= 1; dcol++)
		{
			/* Si ambos son cero, es la celda central, que no se cuenta */
			if (!drow && !dcol)
				continue;

			/* Si la celda le gana, aumentamos el contador */
			if ((value + 1) % 3 == board_cell(board, row + drow, col + dcol))
			{
				count++;
			}
		}
	}
	return count;
}

/** Decide, segun las reglas del juego, si una celda vive o muere */
static int decide_value(int previous, int enemy_neighbors)
{
	/* Dado el estado de una celda, si tiene más vecinos enemigos que el threshold, cambia */
	if (enemy_neighbors > 2)
	{
		return (previous + 1) % 3;
	}
	/* Si no se mantiene*/
	return previous;
}

/** Calcula la siguiente configuracion del tablero a partir de la actual */
void board_next(Board *before, Board *after)
{
	for (int row = 0; row < before->height; row++)
	{
		for (int col = 0; col < before->width; col++)
		{
			int enemy_neighbors = neighbour_count(before, row, col, before->matrix[row][col]);
			after->matrix[row][col] = decide_value(before->matrix[row][col], enemy_neighbors);
			watcher_set_cell(row, col, after->matrix[row][col]);
		}
	}
	watcher_refresh();
}

/** Libera los recursos asociados a una cuadricula */
void board_destroy(Board *board)
{
	for (int row = 0; row < board->height; row++)
	{
		free(board->matrix[row]);
	}
	free(board->matrix);
	free(board);
}
