#pragma once

#include <stdint.h>

/** Define un color en R G B */
struct color
{
	/** Red channel */
  double R;
	/** Green channel */
	double G;
	/** Blue channel */
	double B;
};
/** Define un color en R G B */
typedef struct color Color;

/** Inicializa un color */
Color  color_init(double R, double G, double B);
