#pragma once

#include <stdint.h>

/** Define un color en R G B */
struct color
{
	/** Red channel */
  uint8_t R;
	/** Green channel */
	uint8_t G;
	/** Blue channel */
	uint8_t B;
};
/** Define un color en R G B */
typedef struct color Color;

/** Inicializa un color */
Color color_init(uint8_t R, uint8_t G, uint8_t B);
/** Escala un color por un factor dado */
Color color_scale(Color c, double scale);
