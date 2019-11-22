#pragma once

/****************************************************************************/
/*                          Watcher Correction Edition                      */
/*                                                                          */
/*                Modulo acargo de la correcion de los alumnos              */
/****************************************************************************/

#include <stdbool.h>

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open(int height, int width);
/** Indica el estado de una celda para la siguiente iteracion */
void watcher_set_cell(int row, int col, int state);
/** Hace efectivos todos los cambios realizados y los muestra en la ventana */
void watcher_refresh();
/** Cierra y libera los recursos de la ventana */
void watcher_close();
