#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "board.h"


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Modo de uso: %s <estado inicial> <iteraciones>\n\n", argv[0]);
		printf("\testado inicial: archivo de texto plano que contiene la descripción inicial de la comunidad bacteriana\n\n");
		printf("\titeraciones: número de iteraciones totales que deberá dar el juego de la vida\n\n");
		return 1;
	}

	Board *alpha = board_init(argv[1]);
	Board *beta = board_clone(alpha);


	int iterations = atoi(argv[2]);

	watcher_open(alpha->height, alpha->width);

	for (int row = 0; row < alpha->height; row++)
	{
		for (int col = 0; col < alpha->width; col++)
		{
			watcher_set_cell(row, col, alpha->matrix[row][col]);
		}
	}

	watcher_refresh();

	//sleep(1);


	for (int i = 0; i < iterations; i++)
	{
		board_next(alpha, beta);
		Board *aux = alpha;
		alpha = beta;
		beta = aux;
	}

	//usleep(1000);

	board_destroy(alpha);
	board_destroy(beta);

	watcher_close();

	return 0;
}
