#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo-pdf.h>
#include "drawing.h"

#define MAXSIZE 1024

/** El mutex de dibujo */
static pthread_mutex_t drawing_mutex;

/** Obtiene el valor escalado para X */
static double scale_x(Content* cont, double x)
{
  return ((x - cont -> min_x) / (cont -> max_x - cont -> min_x)) * cont -> width;
}

/** Obtiene el valor escalado para Y */
static double scale_y(Content* cont, double y)
{
  return (1 - (y - cont -> min_y) / (cont -> max_y - cont -> min_y)) * cont -> height;
}

/** Escala el radio de una circunferencia */
static double scale_r(Content* cont, double r)
{
  return (cont -> width / (cont -> max_x - cont -> min_x)) * r;
}

static void cairo_dip_alpha(cairo_t* cr, Color color, double alpha)
{
  cairo_set_source_rgba(cr,color.R,color.G, color.B, alpha);
}

static void cairo_circle(cairo_t* cr, double xc, double yc, double rad)
{
  cairo_arc(cr, xc, yc, rad, 0, 2*M_PI);
}

static void cairo_line(cairo_t* cr, double x1, double y1, double x2, double y2)
{
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
}

void     drawing_draw_vector  (Content* cont, double x, double y)
{
  cairo_t* cr = cairo_create(cont -> vector_image);

  cairo_dip_alpha(cr, cont -> current_color, 1);

  double size = 1;

  cairo_circle(cr,scale_x(cont,x),scale_y(cont,y), size);
  cairo_fill(cr);

  cairo_destroy(cr);
}

/** Dibuja un segmento de recta entre dos puntos */
void drawing_draw_segment (Content* cont, double xi, double yi, double xf, double yf)
{
  cairo_t* cr = cairo_create(cont -> vector_image);

  cairo_dip_alpha(cr, cont -> current_color, 1);
  cairo_line(cr, scale_x(cont, xi), scale_y(cont, yi), scale_x(cont, xf), scale_y(cont, yf));
  cairo_stroke(cr);

  cairo_destroy(cr);
}

void drawing_draw_circle  (Content* cont, double cx, double cy, double r)
{
  cairo_t* cr = cairo_create(cont -> vector_image);

  cairo_dip_alpha(cr, cont -> current_color, 1);

  cairo_circle(cr, scale_x(cont, cx), scale_y(cont, cy), scale_r(cont, r));
  cairo_stroke(cr);

  cairo_destroy(cr);

}

/** Dibuja sobre el canvas dado */
bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
  pthread_mutex_lock(&drawing_mutex);

  cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);

  cairo_paint(cr);

  cairo_set_source_surface(cr, cont -> vector_image, 0, 0);

  cairo_paint(cr);

  pthread_mutex_unlock(&drawing_mutex);

	return true;
}

static void init_vector_image(int height, int width, Content* cont)
{
  /* El formato de imagen: R G B de 8 bits cada uno */
  cairo_format_t format = CAIRO_FORMAT_ARGB32;
  /** La imagen vectorial misma */
  cont -> vector_image = cairo_image_surface_create(format, width, height);
}

/** Genera los contenedores para las imagenes */
Content* drawing_init(double min_x, double min_y, double max_x, double max_y)
{
  Content* cont = malloc(sizeof(Content));

  cont -> min_x = min_x;
  cont -> min_y = min_y;
  cont -> max_x = max_x;
  cont -> max_y = max_y;

  cont -> height = MAXSIZE;
  cont -> width = MAXSIZE;

	double span_x = max_x - min_x;
	double span_y = max_y - min_y;

	if (span_y > span_x)
	{
		cont -> width *= (span_x / span_y);
	}
	else
	{
		cont -> height *= (span_y / span_x);
	}

  cont -> current_color = color_init(1, 1, 1);

  /* Inicializa la imagen vectorial */
  init_vector_image(cont -> height, cont -> width, cont);

	pthread_mutex_init(&drawing_mutex, NULL);

  return cont;
}

/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	double width = cont -> width;
	double height = cont -> height;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

  surface = cairo_pdf_surface_create (filename, width, height);

	cr = cairo_create(surface);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = *cont;

	/* Dibuja el estado actual */
	drawing_draw(cr, &aux);

  cairo_surface_write_to_png(surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont)
{
  cairo_surface_destroy(cont -> vector_image);
  free(cont);
  pthread_mutex_destroy(&drawing_mutex);
}
