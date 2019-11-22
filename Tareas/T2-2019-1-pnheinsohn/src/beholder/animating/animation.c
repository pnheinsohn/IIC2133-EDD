#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "../drawing/drawing.h"
#include <math.h>
#include <locale.h>
#include "../../snekbirb/snek.h"
#include <stdint.h>

/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Lleva a cabo la actualización del tablero */
static void* update(void* ptr)
{
	setlocale(LC_ALL, "C");

	/* Desencapsula los parámetros */
	void** param = ptr;
	GtkWidget* canvas = param[0];
	Content* cont = param[1];
	free(param);

	char command[64];

  while(update_thread)
  {
    /* Si alguno de los numeros falla, dejamos de llamar */
    if(fscanf(stdin, "%s", command))
    {
			/* Cerrar ventana */
      if(!strcmp(command, "X"))
      {
				gtk_main_quit();
				break;
      }
      /* Dibujar pájaro */
			else if(!strcmp(command, "B"))
			{
        int size;
        fscanf(stdin, "%d", &size) ?: abort();

        int row, col;
        fscanf(stdin, "%d %d", &row, &col) ?: abort();

        Direction array[size - 1];
        for(int i = 0; i < size - 1; i++)
        {
          fscanf(stdin, "%u", &array[i]) ?: abort();
        }

        drawing_draw_bird(cont, row, col, size, array);
				gtk_widget_queue_draw(canvas);
			}
      // /* Resetear los pajaros */
      // else if(!strcmp(command, "R"))
      // {
      //   drawing_reset_birds(cont);
      // }
      // /* Actualizar la ventana */
      // else if(!strcmp(command, "U"))
      // {
      //   gtk_widget_queue_draw(canvas);
      // }
			/* Guardar imagen */
			else if(!strcmp(command, "S"))
			{
				char filename[64];
				fscanf(stdin, "%s", filename) ? : abort();
				drawing_snapshot(cont, filename);
			}
			/* Comando desconocido */
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

/** Inicializa el thread que animará el programa */
void animation_init(GtkWidget* canvas, gpointer user_data)
{
	/* Encapsula ambos parámetros dentro de un mismo puntero */
	void** param = calloc(2, sizeof(void*));
	param[0] = canvas;
	param[1] = user_data;

  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, param);
}

void animation_abort()
{
	pthread_cancel(*update_thread);
	free(update_thread);
	update_thread = NULL;
}
