#pragma once

#include "../data/data.h"


typedef struct node {
    Vector *vector;
    double distance;
} Node;

typedef struct heap {
    int size;
    int counter;
    Node **array;
} Heap;

Heap *heap_init(int size);
void heap_insert(Heap *heap, Vector *vector, double distance);
void heap_remove(Heap *heap);
void heap_printer(Heap *heap);
void destroy_heap(Heap *heap);