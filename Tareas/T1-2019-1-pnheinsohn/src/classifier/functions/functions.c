#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
#include "../data/data.h"


AuxIndex *new_aux_index(int len) {
    AuxIndex *new = malloc(sizeof(AuxIndex) * len);
    new->array = malloc(sizeof(int) * len);
    new->len = len;
    return new;
}

Data *new_data_init(Data* train) {
    Data *new = malloc(sizeof(Data));
    new->vectors = malloc(sizeof(Vector *) * train->count);
    for (int i = 0; i < train->count; i++) {
        new->vectors[i] = malloc(sizeof(Vector));
        new->vectors[i]->id = train->vectors[i]->id;
        new->vectors[i]->label = train->vectors[i]->label;
        new->vectors[i]->pos[0] = train->vectors[i]->pos[0];
        new->vectors[i]->pos[1] = train->vectors[i]->pos[1];
    }
    new->count = train->count;
    return new;
}

int x_qsort(const void *p1, const void *p2) {
    const Vector *vector1 = *(Vector *const *) p1;
    const Vector *vector2 = *(Vector *const *) p2;
    if (vector1->pos[0] == vector2->pos[0]) return vector1->pos[1] * 100000 - vector2->pos[1] * 100000;
    return vector1 -> pos[0] * 100000 - vector2 -> pos[0] * 100000;
}

int y_qsort(const void *p1, const void *p2) {
    const Vector *vector1 = *(Vector *const *) p1;
    const Vector *vector2 = *(Vector *const *) p2;
    if (vector1->pos[1] == vector2->pos[1]) return vector1->pos[0] * 100000 - vector2->pos[0] * 100000;
    return vector1 -> pos[1] * 100000 - vector2 -> pos[1] * 100000;
}

void get_dim_and_coords(Data *train, AuxIndex *x_array, AuxIndex *y_array, double dim_coords[]) {
    double min_x = train->vectors[x_array->array[0]]->pos[0];
    double min_y = train->vectors[y_array->array[0]]->pos[1];
    double max_x = train->vectors[x_array->array[x_array->len - 1]]->pos[0];
    double max_y = train->vectors[y_array->array[y_array->len - 1]]->pos[1];    
    
    if (max_x - min_x >= max_y - min_y) dim_coords[0] = 0;
    else dim_coords[0] = 1;

    dim_coords[1] = min_x;
    dim_coords[2] = min_y;
    dim_coords[3] = max_x;
    dim_coords[4] = max_y;
}

/** Printers */
void data_printer(Data *data) {
  for (int i = 0; i < data->count; i++) {
    printf("(%.2f, %.2f) | ", roundf(data->vectors[i]->pos[0] * 10) / 10, roundf(data->vectors[i]->pos[1] * 10) / 10);
  }
}
