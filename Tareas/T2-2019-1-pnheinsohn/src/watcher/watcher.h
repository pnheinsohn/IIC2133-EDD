#pragma once
#include "../snekbirb/snek.h"
/****************************************************************************/
/*                                Watcher                                   */
/*                                                                          */
/* Módulo a cargo de la visualización del problema.                         */
/****************************************************************************/

/** Abre una ventana para visualizar el problema dado el archivo del test */
void watcher_open         (char* filename);
/** Dibuja el pájaro dada la posicion de su cabeza y el arreglo de direcciones */
void watcher_draw_bird    (Snek* snek);
/** Genera una imagen PNG con el contenido actual de la ventana */
void watcher_snapshot     (char* filename);
/** Cierra y libera los recursos de la ventana */
void watcher_close        ();
