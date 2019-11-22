#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "data.h"

////////////////////////////////////////////////////////////////////////////////
//                            Funciones privadas                              //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa un vector vacio */
static Vector* vector_init(double x, double y, int label, int id) {
  // Reservo memoria para el vector
  Vector* vector = malloc(sizeof(Vector));

  // Inicializo valores
  vector -> id = id;
  vector -> pos[0] = x;
  vector -> pos[1] = y;
  vector -> label = label;

  // Retorno el vector
  return vector;
}

/** Libera la memoria de un vector */
static void vector_destroy(Vector* vector) {
  // libero el vector
  free(vector);
}

////////////////////////////////////////////////////////////////////////////////
//                           Funciones publicas                               //
////////////////////////////////////////////////////////////////////////////////

/** Lee un archivo y guarda los datos en un struct Data con vectores y labels */
Data* data_init(FILE* file) {
  // Inicializo el data
  Data* data = malloc(sizeof(Data));

  // Leo el numero de vectores
  fscanf(file, "%d", &data -> count);

  // Creo arreglos para guardar los vectores
  data -> vectors = malloc(sizeof(Vector*) * data -> count);

  // Leo el resto del archivo
  for (int i = 0; i < data -> count; i++)
  {
    // Leo el label y las posiciones
    int label;
    double x, y;
    fscanf(file, "%d %lf %lf", &label, &x, &y);

    // Inicializo el vector
    data -> vectors[i] = vector_init(x, y, label, i);
  }

  // Retorno la estructura
  return data;
}

/** Funcion que calcula la afinidad de un punto */
double affinity(double dist, double min, double max)
{
  /*
  La afinidad de un vecino cercano depende de la distancia del vecino cercano
  mas cercano al objetivo (min) y la distancia del vecino cercano mas lejano al
  objetivo (max).

  La idea de la funcion es que la afinidad disminuya exponencialmente desde el
  vecino mas cercano al vecino mas lejano. Pero para que los vecinos mas lejanos
  tengan una afinidad no despreciable se fija la afinidad del mas lejano a 0.1 y
  la afinidad del mas cercano a 1.

  La funcion que cumple esa caracteristica es 10^((min - x) / (max - min))
  */

  // Si max = min, afinidad = 1
  if (min == max) return 1;

  return pow(10, (min - dist) / (max - min));

}

/** Funcion que calcula la distancia de 2 vectores */
double distance(Vector* v1, Vector* v2)
{
  double dx = pow(v1 -> pos[0] - v2 -> pos[0], 2);
  double dy = pow(v1 -> pos[1] - v2 -> pos[1], 2);
  return sqrt(dx + dy);
}

/** Determina el label del objetivo a partir de sus vecinos cercanos */
int get_label(Vector** neighbours, int k, Vector* objective)
{
  // Mayor y menor distancia al objetivo
  double min = INFINITY;
  double max = 0;

  // Calculo max y min
  for (int n = 0; n < k; n++)
  {
    double dist = distance(neighbours[n], objective);

    if (dist < min) min = dist;
    if (dist > max) max = dist;
  }

  // Arreglo de afinidad total
  double* total_affinity = calloc(N_LABELS, sizeof(double));

  // Sumo afinidades de todos los vecinos al label que corresponde
  for (int n = 0; n < k; n++)
  {
    // Distancia
    double dist = distance(neighbours[n], objective);

    // Sumo afinidad
    total_affinity[neighbours[n] -> label] += affinity(dist, min, max);
  }

  // Veo cual es el label con mayor afinidad
  double max_affinity = 0;
  int label = 0;
  for (int l = 0; l < N_LABELS; l++)
  {
    if (total_affinity[l] > max_affinity)
    {
      max_affinity = total_affinity[l];
      label = l;
    }
  }

  // Libero el arreglo de afinidades
  free(total_affinity);

  // Retorno el label correspondiente
  return label;
}

/** Libera toda la memoria de los vectores y labels de Data */
void data_destroy(Data* data) {
  // Libero los vectores
  for (int i = 0; i < data -> count; i++)
  {
    vector_destroy(data -> vectors[i]);
  }

  // Libero el arreglo de vectores
  free(data -> vectors);

  // Libero el struct data
  free(data);
}
