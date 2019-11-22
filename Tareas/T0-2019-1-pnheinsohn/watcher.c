#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open(uint8_t height, uint8_t width)
{
	char command[256];

	sprintf(command, "%s %u %u", WATCHER, height, width);

	if(watcher) watcher_close();

	watcher = popen(command, "w");
}
/** Indica el estado de una celda para la siguiente iteracion */
void watcher_set_cell(uint8_t row, uint8_t col, uint8_t state)
{
	if(watcher)
	{
		if(fprintf(watcher, "CELL %u %u %u\n", row, col, state) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}
/** Hace efectivos todos los cambios realizados y los muestra en la ventana */
void watcher_refresh()
{
	if(watcher)
	{
		if(fprintf(watcher, "REFRESH\n") < 0)
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
    if(fprintf(watcher, "%s\n", "END") >= 0)
    {
      fflush(watcher);
      pclose(watcher);
    }
    watcher = NULL;
  }
}
