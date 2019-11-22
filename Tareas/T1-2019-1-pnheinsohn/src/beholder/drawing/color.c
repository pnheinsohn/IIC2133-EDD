#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "color.h"

/** Inicializa un color dentro de los rangos establecidos */
Color color_init(double R, double G, double B)
{
	return (Color)
	{
		.R = R > 1 ? 1 : R < 0 ? 0 : R,
		.G = G > 1 ? 1 : G < 0 ? 0 : G,
		.B = B > 1 ? 1 : B < 0 ? 0 : B,
	};
}
