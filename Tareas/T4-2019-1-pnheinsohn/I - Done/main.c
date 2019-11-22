#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 99999

// Structs
typedef struct node {
    int key[2];
    int value;
} Node;

typedef struct heap {
    int count;
    int size;
    Node **elements;
} Heap;

typedef struct cell {
    int height;
    int distance;
    bool visited;
} Cell;

typedef struct laberynth {
    int size;
    Cell ***cells;
    int position[2];
} Laberynth;

// Laberynth Functions
Cell *cell_init(int height) {
    Cell *new = malloc(sizeof(Cell));
    new->distance = INFINITY;
    new->height = height;
    new->visited = false;
    return new;
}

Laberynth *laberynth_init(int size) {
    Laberynth *new = malloc(sizeof(Laberynth));
    new->size = size;
    new->cells = malloc(sizeof(Cell **) * size);
    return new;
}

void laberynth_destroy(Laberynth *laberynth) {
    for (int i = 0; i < laberynth->size; i++) {
        for (int j = 0; j < laberynth->size; j++) free(laberynth->cells[i][j]);
        free(laberynth->cells[i]);
    }
    free(laberynth->cells);
    free(laberynth);
}

// Node Functions
Node *node_init(int key[], int value) {
    Node *new = malloc(sizeof(Node));
    new->key[0] = key[0];
    new->key[1] = key[1];
    new->value = value;
    return new;
}

void node_destroy(Node *node) {
    free(node);
}

// Heap Functions
static int value(Heap *heap, int i) {
    return heap->elements[i]->value;
}

static void swap(Heap *heap, int i1, int i2) {
    Node *node = heap->elements[i1];
    heap->elements[i1] = heap->elements[i2];
    heap->elements[i2] = node;
}

static void sift_down(Heap *heap, int pos) {
    int left = pos * 2 + 1;
    int right = left + 1;

    if (heap->count <= left) return;

    int smaller;
    if (heap->count == right || value(heap, left) < value(heap, right)) smaller = left;
    else smaller = right;

    if (value(heap, pos) > value(heap, smaller)) {
        swap(heap, pos, smaller);
        sift_down(heap, smaller);
    }
}

static void sift_up(Heap *heap, int pos) {
    if (!pos) return;
    int father = (pos - 1) / 2;
    if (value(heap, father) > value(heap, pos)) {
        swap(heap, pos, father);
        sift_up(heap, father);
    }
}

static Heap *heap_init(int size) {
    Heap *new = malloc(sizeof(Heap));
    new->count = 0;
    new->size = size;
    new->elements = malloc(sizeof(Node *));
    return new;
}

void heap_insert(Heap *heap, Node *node) {
    heap->elements[heap->count] = node;
    heap->count++;
    heap->elements = (Node **) realloc(heap->elements, sizeof(Node *) * (heap->count + 1));
    sift_up(heap, heap->count - 1);
}

Node *heap_pop(Heap *heap) {
    if (!heap->count) return NULL;
    Node *node = heap->elements[0];
    heap->count--;
    if (heap->count > 0) {
        heap->elements[0] = heap->elements[heap->count];
        heap->elements[heap->count] = NULL;
        sift_down(heap, 0);
    }
    else heap->elements[0] = NULL;
    return node;
}

void heap_destroy(Heap *heap) {
    for (int i = 0; i < heap->count; i++) node_destroy(heap->elements[i]);
    free(heap->elements);
    free(heap);
}

// Main alg Functions
int dijkstra(Laberynth *laberynth) {
    // Return value
    int value = -1;

    // Init heap
    Heap *heap = heap_init(laberynth->size);
    Node *start_node = node_init(laberynth->position, 0);
    heap_insert(heap, start_node);
    while (heap->count != 0) {
        Node *node = heap_pop(heap);
        laberynth->cells[node->key[0]][node->key[1]]->visited = true;

        // Found solution
        if (node->key[0] == laberynth->size - 1 && node->key[1] == laberynth->size - 1) {
            value = node->value;
            heap_destroy(heap);
            node_destroy(node);
            return value;
        }

        int row = node->key[0];
        int col = node->key[1];
        int actual_height = laberynth->cells[row][col]->height;

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (abs(i) == abs(j)) continue;
                int pos[2] = {row + i, col + j};
                // Dentro del tablero
                if (0 <= pos[0] && pos[0] < laberynth->size && 0 <= pos[1] && pos[1] < laberynth->size) {
                    if (laberynth->cells[pos[0]][pos[1]]->visited) continue;

                    int new_height = laberynth->cells[pos[0]][pos[1]]->height;
                    if ((actual_height >= new_height && actual_height - new_height <= 5)
                    || (actual_height < new_height && new_height - actual_height <= 2)) {
                        
                        int distance = node->value + 1;
                        if (distance >= laberynth->cells[pos[0]][pos[1]]->distance) continue;
                        laberynth->cells[pos[0]][pos[1]]->distance = distance;
                        Node *new = node_init(pos, distance);
                        heap_insert(heap, new);
                    }    
                }
            }
        }
        node_destroy(node);
    }
    heap_destroy(heap);
    // No solution
    return value;
}

int main(int argc, char *argv[]) {
    int N;
    int n_scores = 0;
    int *all_scores = malloc(sizeof(int));
    while (scanf("%d", &N) == 1) {
        if (N == 0) break;
        Laberynth *laberynth = laberynth_init(N);
        int height;
        for (int i = 0; i < laberynth->size; i++) {
            laberynth->cells[i] = malloc(sizeof(Cell *) * N);
            for (int j = 0; j < laberynth->size; j++) {
                scanf("%d", &height);
                laberynth->cells[i][j] = cell_init(height);
            }
        }
        scanf("%d %d", &laberynth->position[0], &laberynth->position[1]);
        all_scores[n_scores] = dijkstra(laberynth);
        n_scores++;
        all_scores = (int *) realloc(all_scores, sizeof(int) * (n_scores + 1));
        laberynth_destroy(laberynth);
    }
    // output
    for (int i = 0; i < n_scores; i++) {
        printf("%i\n", all_scores[i]);
    }
    free(all_scores);
}