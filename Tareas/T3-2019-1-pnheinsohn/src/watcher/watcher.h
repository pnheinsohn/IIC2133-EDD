#pragma once

/****************************************************************************/
/*                                Watcher                                   */
/*                                                                          */
/* Modulo a cargo de la visualizacion del problema.                         */
/****************************************************************************/

#include <stdbool.h>

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open            (int height, int width);
/** Indica que la celda es la meta */
void watcher_set_cell_goal   (int row, int col);
/** Escribe un número dentro de esa celda */
void watcher_set_cell_degree (int row, int col, int degree);
/** Indica si una celda es un bloque o no */
void watcher_set_cell_status (int row, int col, bool is_block);
/** Indica a la ventana que debe volverse a dibujar con los cambios hechos */
void watcher_refresh         ();
/** Imprime la ventana en formato PNG */
void watcher_snapshot        (char* filename);
/** Cierra y libera los recursos de la ventana */
void watcher_close           ();
