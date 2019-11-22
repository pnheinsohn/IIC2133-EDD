#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "window.h"
#include "drawing/drawing.h"
#include "drawing/color.h"
#include <math.h>

/** Revisa que la cantidad de parámetros entregada sea correcta */
static bool check_parameters(int argc, char** argv)
{
	return argc == 5;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

	/* Las dimensiones del espacio */
	double min_x = atof(argv[1]);
	double min_y = atof(argv[2]);
	double max_x = atof(argv[3]);
	double max_y = atof(argv[4]);

	/* Inicializa los contenedores */
	Content* cont = drawing_init(min_x, min_y, max_x, max_y);

	/* Crea la ventana */
	window_create(cont);

	/* Libera los contenedores */
	drawing_free(cont);

	return 0;
}
