#pragma once

#include <stdbool.h>

/** Representa la cuadricula donde van situadas las bacterias */
struct board
{
	/** La cuadricula de celdas que representan bacterias */
	int** matrix;
	/** Ancho de la cuadricula */
	int width;
	/** Alto de la cuadricula */
	int height;
};

/** Representa la cuadricula donde van situadas las bacterias */
typedef struct board Board;


/** Inicializa el tablero a partir de un archivo */
Board* board_init(char* filename);
/** Clona un tablero */
Board* board_clone(Board* board);
/** Calcula la siguiente configuracion del tablero a partir de la actual */
void board_next(Board* before, Board* after);
/** Libera todos los recursos asociados a una cuadricula */
void board_destroy(Board* board);
