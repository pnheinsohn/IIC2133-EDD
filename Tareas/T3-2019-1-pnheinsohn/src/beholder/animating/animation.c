#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "../drawing/drawing.h"
#include <math.h>
#include <locale.h>

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
    /* Si alguna de las lecturas falla, dejamos de llamar */
    if(fscanf(stdin, "%s", command))
    {
			/* Cerrar ventana */
      if(!strcmp(command, "X"))
      {
				gtk_main_quit();
				break;
      }
      /* Celda objetivo */
      else if(!strcmp(command, "G"))
      {
        int row, col;
        fscanf(stdin, "%d %d", &row, &col) ? : abort();
        pthread_mutex_lock(&drawing_mutex);
				drawing_cell_set_goal(cont, row, col);
				pthread_mutex_unlock(&drawing_mutex);
      }
			/* Grado de la celda */
			else if(!strcmp(command, "D"))
			{
				int row, col;
				int deg;
				fscanf(stdin, "%d %d %d", &row, &col, &deg) ? : abort();
				pthread_mutex_lock(&drawing_mutex);
				drawing_cell_set_degree(cont, row, col, deg);
				pthread_mutex_unlock(&drawing_mutex);
			}
			/* Estado de la celda */
			else if(!strcmp(command, "S"))
			{
				int row, col;
				int status;
				fscanf(stdin, "%d %d %d", &row, &col, &status) ? : abort();
				pthread_mutex_lock(&drawing_mutex);
				drawing_cell_set_status(cont, row, col, status == 1);
				pthread_mutex_unlock(&drawing_mutex);
			}
      /* Volver a dibujar la ventana */
			else if(!strcmp(command, "R"))
			{
        gtk_widget_queue_draw(canvas);
			}
      /* Guardar imagen */
			else if(!strcmp(command, "H"))
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

/** Aborta la rutina que actualiza el tablero */
void animation_abort()
{
	pthread_cancel(*update_thread);
	free(update_thread);
	update_thread = NULL;
}
