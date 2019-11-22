#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include <pthread.h>
#include "../../snekbirb/snek.h"

/** Contiene la información de lo que ha de ser dibujado en el canvas */
struct content
{
	/** Ancho de la ventana */
	int width;
	/** Alto de la ventana */
	int height;
  /** Fila en la que está el portal */
  int portal_row;
  /** Columna en la que está el portal */
  int portal_col;
	/** Imagen vectorial que contiene los pajaros */
  cairo_surface_t* bird_layer;
  /** Imagen vectorial que contiene el fondo */
  cairo_surface_t* background_layer;
  /** Imagen vectorial que contiene el terreno */
  cairo_surface_t* terrain_layer;
  /** Imagen vectorial que contiene las caras de los pajaros */
  cairo_surface_t* face_layer;
  /** Imagen vectorial que contiene el portal (escalado)*/
  cairo_surface_t* portal_layer;
  /** Color del pajaro */
  int bird_color;
};
/** Contiene la información de lo que ha de ser dibujado en el canvas */
typedef struct content Content;

/** Inicializa las herramientas de dibujo */
Content* drawing_init      (char* filename);
/** Dibuja el contenido sobre el canvas dado */
bool     drawing_draw      (cairo_t* cr, Content* cont);
/** Dibuja un pájaro en la ventana */
void     drawing_draw_bird (Content* cont, int row, int col, int size, Direction* array);
/** Elimina los pajaros de la ventana */
void     drawing_reset_birds (Content* cont);
/** Genera una imagen en PNG para un estado en particular */
void     drawing_snapshot  (Content* cont, char* filename);
/** Libera los recursos asociados a las herramientas de dibujo */
void     drawing_free      (Content* cont);
