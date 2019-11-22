#pragma once

/****************************************************************************/
/*                                Watcher                                   */
/*                                                                          */
/* Módulo a cargo de la visualización del problema.                         */
/****************************************************************************/

/** Abre una ventana indicando cual es el espacio en R² que representará */
void watcher_open         (double min_x, double min_y, double max_x, double max_y);

/** Selecciona un color para las siguientes operaciones de dibujo */
void watcher_set_color    (double R, double G, double B);

/** Dibuja un segmento de recta de un punto a otro del color seleccionado */
void watcher_draw_segment (double xi, double yi, double xf, double yf);

/** Dibuja una circunferencia de radio r centrada en (cy, cy) con el color seleccionado */
void watcher_draw_circle  (double cx, double cy, double r);

/** Dibuja un vector en (x,y) del color seleccionado */
void watcher_draw_vector  (double x, double y);

/** Genera una imagen PNG con el contenido actual de la ventana */
void watcher_snapshot     (char* filename);

/** Cierra y libera los recursos de la ventana */
void watcher_close        ();
