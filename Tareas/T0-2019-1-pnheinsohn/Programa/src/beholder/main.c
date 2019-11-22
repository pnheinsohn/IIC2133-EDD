#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#define WINDOW_MAX_SIZE 640.0

/** Nivel de escalamiento */
double scale;
/** Thread encargado de leer el input */
pthread_t* update_thread;
/** La imagen que estamos usando */
cairo_surface_t* image;
/** Metadata de la imagen */
int stride;
uint8_t* data;
/** La imagen (en formato patron) para poder dibujarla cuadriculada */
cairo_pattern_t* pattern;


/** Dibuja la imagen en la ventana */
gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
	cairo_scale(cr, scale, scale);
	cairo_set_source(cr, pattern);
  cairo_paint(cr);

  return TRUE;
}

/** Modifica el pixel de la imagen**/
void change_pixel_color(int row, int col, int red, int green, int blue)
{
  /* Nos aseguramos de que se terminen los cambios en la imagen */
  cairo_surface_flush(image);

  /* Cambiamos el pixel al color correspondiente*/
  data[row*stride + col*4 + 0] = red;
  data[row*stride + col*4 + 1] = green;
  data[row*stride + col*4 + 2] = blue;

  /* Marcamos que la imagen fue modificada para que Cairo la cargue */
  cairo_surface_mark_dirty(image);
}


/** Funcion que lee la siguiente jugada y actualiza el tablero segun eso */
void* update(void* canvas)
{
  int row, col;
	int color;
  char command[16];

  while(update_thread)
  {
    /* Si alguno de los numeros falla, dejamos de llamar */
    if(fscanf(stdin, "%s", command))
    {
      if(!strcmp(command, "END"))
      {
				gtk_main_quit();
				break;
      }
			else if(!strcmp(command, "CELL"))
			{
        // 255, 32, 110
        // 251, 255, 18
        // 12, 15, 10
				if(!scanf("%d %d %d", &row, &col, &color)) break;
        if (color == 0)
				{
          change_pixel_color(row, col, 255, 32, 110);
        }
        else if (color == 1)
				{
          change_pixel_color(row, col, 251, 255, 18);
        }
        else if (color == 2)
        {
          change_pixel_color(row, col, 12, 15, 10);
        }
        else // si imprimen este color (blanco) es porque hay un error 
        {
          change_pixel_color(row, col, 255, 255, 255);
        }
      }
			else if(!strcmp(command, "REFRESH"))
			{
        /* Actualizamos la ventana de manera asincrona */
        gtk_widget_queue_draw(canvas);
			}
			else
			{
				break;
			}
    }
    else
    {
      break;
    }

  }
  pthread_exit(NULL);
}


/** Inicializa el thread que animara el programa */
void spawn_updater(GtkWidget *widget, gpointer user_data)
{
  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, widget);
}

bool check_parameters(int argc, char** argv)
{
  if(argc != 3) return false;
  return true;
}

void init_image(int height, int width)
{
  /* El formato de imagen: R G B de 8 bits cada uno */
  cairo_format_t format = CAIRO_FORMAT_RGB24;
  /* El ancho en bits de la imagen */
  stride = cairo_format_stride_for_width (format, width);
  /* La información de los pixeles de la imagen completa */
  data   = malloc(stride * height);
  /* La imagen en sí */
  image  = cairo_image_surface_create_for_data(data, format, width, height, stride);


	/* Inicializamos la imagen */
  for(int j = 0; j < height; j++)
  {
    for(int i = 0; i <  width; i++)
    {
			/* Blue channel */
      data[j*stride + i*4 + 0] = 0;
			/* Green channel */
      data[j*stride + i*4 + 1] = 0;
			/* Red channel */
      data[j*stride + i*4 + 2] = 0;
			/* Alpha channel */
      data[j*stride + i*4 + 3] = 0;
    }
  }

	/* Pasamos la imagen a patron dado que necesitamos usar NEAREST NEIGHBOURS */
	pattern = cairo_pattern_create_for_surface(image);
	cairo_pattern_set_filter(pattern, CAIRO_FILTER_NEAREST);
}


/** Visualiza el estado del tablero */
int main(int argc, char** argv)
{
  /* Revisamos que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

  /* Las dimensiones de la matriz */
	int matrix_height = atoi(argv[1]);
	int matrix_width = atoi(argv[2]);

	/* Las dimensiones de la ventana deben ajustarse a la matriz */
	scale = WINDOW_MAX_SIZE / fmax(matrix_height, matrix_width);
	int img_height = scale * matrix_height;
	int img_width  = scale * matrix_width;

  /* Inicializamos la imagen*/
  init_image(matrix_height, matrix_width);

  /* Se cierra el canal para errores para que GTK no moleste */
  fclose(stderr);

  /* Inicializar GTK */
  gtk_init(0, NULL);

  /* Inicializar ventana */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Inicializar canvas */
  GtkWidget* drawingArea = gtk_drawing_area_new();

  /* Dimensiones del canvas */
  gtk_widget_set_size_request(drawingArea, img_width, img_height);

  /* Ligar eventos */
  g_signal_connect(drawingArea, "draw", G_CALLBACK(draw), image);
  g_signal_connect(drawingArea, "realize", G_CALLBACK(spawn_updater), NULL);

  /* Meter canvas a la ventana */
  gtk_container_add(GTK_CONTAINER(window), drawingArea);

  /* Mostrar todo */
  gtk_widget_show(drawingArea);
  gtk_widget_show(window);

  /* Comenzamos la ejecucion de GTK */
  gtk_main();

  /* Liberamos la imagen */
  cairo_surface_destroy(image);
	cairo_pattern_destroy(pattern);

	/* Liberamos la memoria */
  free(update_thread);
	update_thread = NULL;
	free(data);

  return 0;
}
