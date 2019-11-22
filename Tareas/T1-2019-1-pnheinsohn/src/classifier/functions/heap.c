#include <stdlib.h>
#include "heap.h"

//////////////////////////////////////////////////////////
//                  Funciones Privadas                  //
//////////////////////////////////////////////////////////

static void swap(Heap *heap, int i1, int i2) {
    Node *aux = heap->array[i1];
    heap->array[i1] = heap->array[i2];
    heap->array[i2] = aux;
}

static void sift_up(Heap *heap, int pos) {
    if (!pos) return;
    int father = (pos - 1) / 2;
    if (heap->array[father]->distance < heap->array[pos]->distance) {
        swap(heap, pos, father);
        sift_up(heap, father);
    }
}

static void sift_down(Heap *heap, int pos) {
    int left = pos * 2 + 1;
    int right = left + 1;
    if (heap->counter <= left) return;
    int greater;
    if (heap->counter == right || heap->array[left]->distance > heap->array[right]->distance) {
        greater = left;
    } else {
        greater = right;
    }
    if (heap->array[pos]->distance < heap->array[greater]->distance) {
        swap(heap, pos, greater);
        sift_down(heap, greater);
    }
}

//////////////////////////////////////////////////////////
//                  Funciones Publicas                  //
//////////////////////////////////////////////////////////
Heap *heap_init(int size) {
    Heap *heap = malloc(sizeof(Heap));
    heap->counter = 0;
    heap->size = size;
    heap->array = calloc(size, sizeof(Node *));
    return heap;
}

void heap_insert(Heap *heap, Vector *vector, double distance) {
    Node *new = malloc(sizeof(Node));
    new->vector = vector;
    new->distance = distance;
    heap->array[heap->counter] = new;
    heap->counter++;
    sift_up(heap, heap->counter - 1);
}

void heap_remove(Heap *heap) {
    if (!heap->counter) return;
    heap->counter--;
    free(heap->array[0]);
    heap->array[0] = heap->array[heap->counter];
    heap->array[heap->counter] = NULL;
    sift_down(heap, 0);
}

void destroy_heap(Heap *heap) {
    for (int i = 0; i < heap->counter; i++) {
        free(heap->array[i]);
    }
    free(heap->array);
    free(heap);
}

void heap_printer(Heap *heap) {
    if (heap->counter == 0) {
        printf("HEAP IS EMPTY\n");
    }
    printf("HEAP LEN: %i\n", heap->counter);
    for (int i = 0; i < heap->counter; i++) {
        printf("%lf, ", heap->array[i]->distance);
    }
    printf("\n");
}