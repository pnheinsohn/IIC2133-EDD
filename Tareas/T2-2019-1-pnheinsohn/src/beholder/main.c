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
	return argc == 2;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

  /* El archivo que representa el puzzle */
  char* filename = argv[1];

	/* Inicializa el contenedor */
	Content* cont = drawing_init(filename);

	/* Crea la ventana */
	window_create(cont);

	/* Libera los contenedores */
	drawing_free(cont);

	return 0;
}
