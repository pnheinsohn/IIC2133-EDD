#pragma once

/****************************************************************************/
/*                                Watcher                                   */
/*                                                                          */
/* Modulo a cargo de la visualizacion del problema.                         */
/****************************************************************************/

/* Módulo estandar de numeros enteros*/
#include <stdint.h>

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open     (uint8_t height, uint8_t width);
/** Indica el estado de una celda para la siguiente iteracion */
void watcher_set_cell (uint8_t row, uint8_t col, uint8_t state);
/** Hace efectivos todos los cambios realizados y los muestra en la ventana */
void watcher_refresh  ();
/** Cierra y libera los recursos de la ventana */
void watcher_close    ();
