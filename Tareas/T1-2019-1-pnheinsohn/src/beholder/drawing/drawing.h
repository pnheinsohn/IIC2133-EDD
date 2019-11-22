#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include <pthread.h>

/** Contiene la información de lo que ha de ser dibujado en el canvas */
struct content
{
	/** Ancho de la ventana */
	int width;
	/** Alto de la ventana */
	int height;
	/** Color actual */
	Color current_color;
	/** Imagen vectorial, para dibuar los segmentos de recta */
	cairo_surface_t* vector_image;
	/* La escala de la ventana: min_x -> 0 */
	double min_x;
	/* La escala de la ventana: min_y -> height */
	double min_y;
	/* La escala de la ventana: max_x -> width */
	double max_x;
	/* La escala de la ventana: max_y -> 0 */
	double max_y;
};
/** Contiene la información de lo que ha de ser dibujado en el canvas */
typedef struct content Content;

/** Inicializa las herramientas de dibujo*/
Content* drawing_init					(double min_x, double min_y, double max_x, double max_y);
/** Dibuja sobre el canvas dado */
bool     drawing_draw         (cairo_t* cr, Content* cont);
/** Dibuja un vector en (x,y) */
void     drawing_draw_vector  (Content* cont, double x, double y);
/** Dibuja un segmento de recta entre dos puntos */
void     drawing_draw_segment (Content* cont, double xi, double yi, double xf, double yf);
/** Dibuja un circulo de radio "r" centrado en (cx, cy) */
void     drawing_draw_circle  (Content* cont, double cx, double cy, double r);
/** Genera una imagen en pdf para un estado en particular */
void     drawing_snapshot     (Content* cont, char* filename);
/** Libera los recursos asociados a las herramientas de dibujo */
void     drawing_free         (Content* cont);
