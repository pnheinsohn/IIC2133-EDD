#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open(int height, int width)
{
	char command[256];

	sprintf(command, "%s %d %d", WATCHER, height, width);

	if(watcher) watcher_close();

	watcher = popen(command, "w");
}

/** Escribe un número entre 1 y 4 inclusive dentro de esa celda */
void watcher_set_cell_degree (int row, int col, int degree)
{
	if(watcher)
	{
		if(fprintf(watcher, "D %d %d %d\n", row, col, degree) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica si una celda es o no bloque */
void watcher_set_cell_status (int row, int col, bool type)
{
	if(watcher)
	{
		if(fprintf(watcher, "S %d %d %d\n", row, col, type ? 1 : 0) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica que la celda es la meta */
void watcher_set_cell_goal   (int row, int col)
{
  if(watcher)
	{
		if(fprintf(watcher, "G %d %d\n", row, col) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica a la ventana que debe volverse a dibujar con los cambios hechos */
void watcher_refresh         ()
{
  if(watcher)
	{
		if(fprintf(watcher, "R\n") < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Imprime la ventana en formato PNG */
void watcher_snapshot(char* filename)
{
	if(watcher)
	{
		if(fprintf(watcher, "H %s\n", filename) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Cierra y libera los recursos de la ventana */
void watcher_close()
{
	if(watcher)
  {
    if(fprintf(watcher, "%s\n", "X") >= 0)
    {
      fflush(watcher);
      pclose(watcher);
    }
    watcher = NULL;
  }
}
