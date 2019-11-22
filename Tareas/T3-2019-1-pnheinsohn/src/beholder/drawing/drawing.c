#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <cairo-pdf.h>
#include "drawing.h"
#include "../random/pcg_basic.h"

#define WINDOW_MAX_SIZE 640.0

/* El color para marcar las celdas leales al régimen */
Color loyal_color;
/* El color para marcar las celdas rebeldes */
Color rebel_color;
//
// loyal_color = color_init(249,202,30);
// rebel_color = color_init(70,79,171);
// blank_color = color_init(27,27,67);
// block_color = color_init(200, 200, 255);

/* El color para marcar las celdas sin asociación */
// Color blank_color = (Color){.R = 192, .G = 208, .B = 172};
Color blank_color = (Color){.R = 196, .G = 217, .B = 210};
// Color blank_color = (Color){.R = 27, .G = 27, .B = 67};
/* El color para marcar las celdas que están pintadas */
// Color block_color = (Color){.R = 200, .G = 200, .B = 255};
// Color block_color = (Color){.R = 34, .G = 60, .B = 51};
Color block_color = (Color){.R = 0, .G = 97, .B = 150};
/* El color para dibujar el texto */
// Color texto_color = (Color){.R = 27, .G = 27, .B = 27};
// Color texto_color = (Color){.R = 220, .G = 237, .B = 203};
Color texto_color = (Color){.R = 249, .G = 249, .B = 233};

Color goal_in_color = (Color){.R = 159, .G = 159, .B = 159};
Color goal_out_color = (Color){.R = 255, .G = 255, .B = 255};


static void cairo_dip(cairo_t* cr, Color c)
{
  double R = c.R, G = c.G, B = c.B;

  cairo_set_source_rgb(cr, R / 255.0, G / 255.0, B / 255.0);
}

/** Modifica el pixel de la imagen**/
static void change_pixel_color(BG* bg, int row, int col, Color color)
{
  /* Nos aseguramos de que se terminen los cambios en la imagen */
  cairo_surface_flush(bg -> image);

  /* Cambiamos el pixel al color correspondiente*/
  bg -> data[bg -> stride * row + col * 4 + 0] = color.B;
  bg -> data[bg -> stride * row + col * 4 + 1] = color.G;
  bg -> data[bg -> stride * row + col * 4 + 2] = color.R;

  /* Marcamos que la imagen fue modificada para que Cairo la cargue */
  cairo_surface_mark_dirty(bg -> image);
}

void cairo_rounded_rect(cairo_t* cr, Content* cont, double row, double col, double scale_x, double scale_y)
{
  double x = (col + (1.0 - scale_x) / 2.0) * cont -> scale;
  double y = (row + (1.0 - scale_x) / 2.0) * cont -> scale;

  double width = scale_x * cont -> scale;
  double height = scale_y * cont -> scale;

  double radius = width * 0.1;

  double degrees = M_PI / 180.0;

  cairo_new_sub_path (cr);
  cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path (cr);
}

/** Pinta la celda del color correspondiente */
void drawing_cell_set_status  (Content* cont, int row, int col, bool status)
{
  cont -> status_matrix[row][col] = status;
}
/** Escribe el número correspondiente sobre la celda */
void drawing_cell_set_degree  (Content* cont, int row, int col, int degree)
{
  cont -> degree_matrix[row][col] = degree;
}

/** Indica que la celda correspondiente es la celda objetivo */
void drawing_cell_set_goal    (Content* cont, int row, int col)
{
  cont -> goal_col = col;
  cont -> goal_row = row;
}

static void drawing_draw_block(cairo_t* cr, Content* cont, double row, double col, Color c)
{
  cairo_dip(cr, color_scale(c, 0.5));

  cairo_rounded_rect(cr, cont, row, col, 0.95, 0.95);

  cairo_fill(cr);

  cairo_dip(cr, c);

  cairo_rounded_rect(cr, cont, row, col, 0.95, 0.90);

  cairo_fill(cr);
}

static void drawing_draw_goal(cairo_t* cr, Content* cont)
{
  drawing_draw_block(cr, cont, cont -> goal_row, cont -> goal_col, goal_out_color);

  cairo_dip(cr, goal_in_color);

  cairo_rounded_rect(cr, cont, cont -> goal_row - 0.025, cont -> goal_col, 0.3, 0.3);

  cairo_fill(cr);
}

/** Dibuja el problema en la ventana */
bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
  /* Para prevenir cambios mientras dibujamos */
  pthread_mutex_lock(&drawing_mutex);

  /* Hacemos calzar la ventana con la imagen  */
  cairo_scale(cr, cont -> scale, cont -> scale);
  /* Dibujaremos la imagen de fondo */
  cairo_set_source(cr, cont -> status_image -> pattern);
  /* Pintamos la imagen */
  cairo_paint(cr);
  /* Restauramos la ventana */
  cairo_scale(cr, 1.0 / cont -> scale, 1.0 / cont -> scale);

  /* Meta */
  drawing_draw_goal(cr, cont);


  /* Blocks */
  for(int row = 0; row < cont -> matrix_height; row++)
  {
    for(int col = 0; col < cont -> matrix_width; col++)
    {
      if(cont -> status_matrix[row][col])
      {
        drawing_draw_block(cr, cont, row, col, block_color);
      }
    }
  }



  /* Numbers */
  cairo_text_extents_t te;
  cairo_select_font_face (cr, "monospace",
      CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr, cont -> scale / 1.6);
  char text[2];

  for(int row = 0; row < cont -> matrix_height - 0; row++)
  {
    for(int col = 0; col < cont -> matrix_width - 0; col++)
    {
      if(!cont -> degree_matrix[row][col]) continue;

      sprintf(text,"%hhu",cont -> degree_matrix[row][col]);

      cairo_text_extents (cr, text, &te);

      double x = cont -> scale / 2 + cont -> scale * col - te.width / 2 - te.x_bearing;
      double y = cont -> scale / 2 + cont -> scale * (row - 0.015) - te.height / 2 - te.y_bearing;
      cairo_move_to (cr, x, y);
      cairo_text_path(cr, text);
      cairo_dip(cr, texto_color);
      cairo_fill(cr);
      // cairo_fill_preserve(cr);
      // cairo_set_line_width(cr, cont -> scale / 256);
      // cairo_set_source_rgb(cr, 0, 0, 0);
      // cairo_stroke(cr);
    }
  }

  pthread_mutex_unlock(&drawing_mutex);

	return true;
}

static BG* init_background(int height, int width)
{
  BG* bg = malloc(sizeof(BG));

  /* El formato de imagen: R G B de 8 bits cada uno */
  cairo_format_t format = CAIRO_FORMAT_ARGB32;
  /* El ancho en bits de la imagen */
  bg -> stride = cairo_format_stride_for_width (format, width);
  /* La información de los pixeles de la imagen completa */
  bg -> data   = malloc(bg -> stride * height);
  /* La imagen en sí */
  bg -> image  = cairo_image_surface_create_for_data(bg -> data, format, width, height, bg -> stride);

	/* Inicializamos la imagen */
  for(int row = 0; row < height; row++)
  {
    for(int col = 0; col <  width; col++)
    {
      double scale = pcg32_boundedrand(100001);
      scale /= 100000.0;
      scale /= 5.0;
      scale += 0.9;

      change_pixel_color(bg, row, col, color_scale(blank_color, scale));
			/* Alpha channel */
      bg -> data[bg -> stride * row + col * 4 + 3] = 255;
    }
  }

	/* Pasamos la imagen a patron dado que necesitamos usar NEAREST NEIGHBOURS */
	bg -> pattern = cairo_pattern_create_for_surface(bg -> image);
	cairo_pattern_set_filter(bg -> pattern, CAIRO_FILTER_NEAREST);

  return bg;
}

/** Genera los contenedores para las dos imagenes superpuestas */
Content* drawing_init(int height, int width)
{
  Content* cont = malloc(sizeof(Content));

  pthread_mutex_init(&drawing_mutex, NULL);

  cont -> goal_col = -1;
  cont -> goal_row = -1;

  /* Agregamos celdas en blanco a cada lado de la matriz */
  // height += 2;
  // width += 2;

  /* Dimensiones de la matriz */
  cont -> matrix_height = height;
  cont -> matrix_width = width;

  /* Los colores usados para representar los distintos tipos de celda */
  // loyal_color = color_init(249,202,30);
  // rebel_color = color_init(70,79,171);
  // blank_color = color_init(27,27,67);
  // block_color = color_init(200, 200, 255);

  /* La imagen con los colores de cada celda */
  cont -> status_image = init_background(height, width);

  /* Las dimensiones de la ventana deben ajustarse a la matriz */
	cont -> scale = WINDOW_MAX_SIZE / fmax(height, width);

  /* Dimensiones de la ventana */
	cont -> image_height = cont -> scale * height;
	cont -> image_width  = cont -> scale * width;

  /* Inicializamos las matrices */
  cont -> degree_matrix = calloc(height, sizeof(uint8_t*));
  cont -> status_matrix = calloc(height, sizeof(int8_t*));
  for(int row = 0; row < height; row++)
  {
    cont -> degree_matrix[row] = calloc(width, sizeof(uint8_t));
    cont -> status_matrix[row] = calloc(width, sizeof(int8_t));
  }

  return cont;
}

/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	double width = cont -> matrix_width * 64;
	double height = cont -> matrix_height * 64;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

	// surface = cairo_pdf_surface_create (filename, width, height);
	cr = cairo_create(surface);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = *cont;

  aux.scale = 64;
  aux.image_height = height;
  aux.image_width = width;

	/* Dibuja el estado actual */
	drawing_draw(cr, &aux);

  cairo_surface_write_to_png(surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont)
{
  /* Liberamos la imagen */
  cairo_surface_destroy(cont -> status_image -> image);
  cairo_pattern_destroy(cont -> status_image -> pattern);

  for(int row = 0; row < cont -> matrix_height; row++)
  {
    free(cont -> status_matrix[row]);
    free(cont -> degree_matrix[row]);
  }
  free(cont -> degree_matrix);
  free(cont -> status_matrix);

  free(cont -> status_image -> data);
  free(cont -> status_image);

  free(cont);
}
