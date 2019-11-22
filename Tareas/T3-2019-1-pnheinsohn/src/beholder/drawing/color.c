#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "color.h"

/** Inicializa un color dentro de los rangos establecidos */
Color color_init(uint8_t R, uint8_t G, uint8_t B)
{
	return (Color)
	{
		.R = R,
		.G = G,
		.B = B,
	};
}

/** Escala un color por un factor dado */
Color color_scale(Color c, double scale)
{
  return (Color)
  {
    .R = fmax(0, fmin(c.R * scale, 255)),
    .G = fmax(0, fmin(c.G * scale, 255)),
    .B = fmax(0, fmin(c.B * scale, 255))
  };
}
