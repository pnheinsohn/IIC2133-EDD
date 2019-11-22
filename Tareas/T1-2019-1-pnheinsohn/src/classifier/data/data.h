#pragma once

#include <stdio.h>

/** Variable global que indica el numero de labels distintos*/
int N_LABELS;

/** Estructura de un vector. Tiene una posicion y un label */
typedef struct vector
{
  /** Identificador */
  int id;
  /** Posicion x, y */
  double pos[2];
  /** Label del vector */
  int label;
} Vector;

/** Struct que maneja los vectores de los datasets */
typedef struct data
{
  /** Numero de vectores */
  int count;
  /** Arreglo de vectores */
  Vector** vectors;
} Data;


/** Lee un archivo y guarda los vectores en el struct Data */
Data* data_init(FILE* file);

/** Funcion que calcula la afinidad */
double affinity(double dist, double min, double max);

/** Funcion que calcula la distancia de 2 vectores */
double distance(Vector* v1, Vector* v2);

/** Determina el label del objetivo a partir de sus vecinos cercanos */
int get_label(Vector** neighbours, int k, Vector* objective);

/** Libera los vectores */
void data_destroy(Data* data);

void data_printer(Data* data);
