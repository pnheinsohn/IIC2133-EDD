#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define WATCHER "./beholder"

#define MAXSIZE 512

static FILE* watcher = NULL;

/** Abre una ventana indicando cual es el espacio en R² que representará */
void watcher_open(double min_x, double min_y, double max_x, double max_y)
{

	setlocale(LC_ALL, "C");

	char command[256];

	sprintf(command, "%s %lf %lf %lf %lf", WATCHER, min_x, min_y, max_x, max_y);

	if(watcher) watcher_close();

	watcher = popen(command, "w");
}

/** Selecciona un color para las siguientes operaciones de dibujo */
void watcher_set_color (double R, double G, double B)
{
	if(watcher)
	{
		if(fprintf(watcher, "C %lf %lf %lf\n", R, G, B) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Dibuja un segmento de recta de un punto a otro del color seleccionado */
void watcher_draw_segment (double xi, double yi, double xf, double yf)
{
	if(watcher)
	{
		if(fprintf(watcher, "L %lf %lf %lf %lf\n", xi, yi, xf, yf) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Dibuja una circunferencia de radio r centrada en (cy, cy) */
void watcher_draw_circle  (double cx, double cy, double r)
{
	if(watcher)
	{
		if(fprintf(watcher, "R %lf %lf %lf\n", cx, cy, r) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Dibuja un vector en (x,y) del color seleccionado */
void watcher_draw_vector   (double x, double y)
{
	if(watcher)
	{
		if(fprintf(watcher, "V %lf %lf\n", x, y) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
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
