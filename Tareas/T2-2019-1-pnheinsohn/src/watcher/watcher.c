#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre una ventana para visualizar el problema dado el archivo del test */
void watcher_open(char* filename)
{
	setlocale(LC_ALL, "C");

	char command[256];

	sprintf(command, "%s %s", WATCHER, filename);

	if(watcher) watcher_close();

	watcher = popen(command, "w");
}

/** Dibuja el pájaro dada la posicion de su cabeza y el arreglo de direcciones */
void watcher_draw_bird(Snek* snek)
{
  	if(watcher)
  	{
  		if(fprintf(watcher, "B %d %d %d\n", snek -> size, snek -> head -> row, snek -> head -> col) < 0)
  		{
  			watcher_close();
  		}
  		else
  		{
  			fflush(watcher);

        for(Node* node = snek -> head -> next; node; node = node -> next)
        {
          if(fprintf(watcher, "%u\n", node -> direction) < 0)
          {
            watcher_close();
          }
          else
          {
            fflush(watcher);
          }
        }
  		}
  	}
}

/** Genera una imagen PNG con el contenido actual de la ventana */
void watcher_snapshot (char* filename)
{
	if(watcher)
	{
		if(fprintf(watcher, "S %s\n", filename) < 0)
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
