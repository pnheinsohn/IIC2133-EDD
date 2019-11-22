#pragma once
#include "../data/data.h"

typedef struct auxIndex {
    int *array;
    int len;
} AuxIndex;

int x_qsort(const void *puntero1, const void *puntero2);
int y_qsort(const void *puntero1, const void *puntero2);

AuxIndex *new_aux_index(int len);
Data *new_data_init(Data *train);

void get_dim_and_coords(Data *train, AuxIndex *x_array, AuxIndex *y_array, double dim_coords[]);

void data_printer(Data *data);
