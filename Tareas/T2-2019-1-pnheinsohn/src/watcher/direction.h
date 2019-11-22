#pragma once

/** Representa una dirección */
enum direction
{
  UP = 0,
  LEFT = 1,
  RIGHT = 2,
  DOWN = 3,
  HEAD = 4
};
typedef enum direction Direction;


/** Entrega el opuesto de una dirección x */
// Direction OPPOSITE(Direction x);

#define OPPOSITE(x) (3 - (x))
