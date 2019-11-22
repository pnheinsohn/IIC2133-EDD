#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>

#define WATCHER "./beholder"


/** Crea un archivo .txt para ir guardando las iteraciones */
void watcher_open(uint8_t height, uint8_t width)
{
	printf("%d %d\n", height, width);
}
/** Indica el estado de una celda para la siguiente iteracion */
void watcher_set_cell(uint8_t row, uint8_t col, uint8_t state)
{
	printf("%d %d %d\n", row, col, state);
}
/** Hace efectivos todos los cambios realizados y los escribe en la ventana */
void watcher_refresh()
{
	printf("REFRESH\n");
}
/** Cierra y libera los recursos de la ventana */
void watcher_close()
{
	printf("CLOSE\n");
}
