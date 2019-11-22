#pragma once
#include <stdbool.h>
#include "../data/data.h"
#include "../functions/functions.h"

/** Estructura de un kdtree */
typedef struct kdtree {
    AuxIndex *x_array;
    AuxIndex *y_array;
    int dimension;
    int mid_index;
    double coordenates[4];
    struct kdtree *left;
    struct kdtree *right;
} KDTree;

/** Determina si el circulo definido por cx, cy, r choca con la caja */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My);

/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree *kd_init(Data *train);

/** Funcion que busca los k vecinos mas cercanos a objective
en el kd y los guarda en neighbours */
void knn(Vector **neighbours, KDTree *kd, Data *train_data, int k, Vector *objective);

/** Libera la memoria del kdtree */
void kd_destroy(KDTree *kd);

void kd_printer(KDTree *kd);
