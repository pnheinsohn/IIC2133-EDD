#include <stdlib.h>

# pragma once


typedef struct node {
    int state;
    struct node** neighbours;
} Node;

Node* node_init(Node*** table);
void node_assignment(int M, int N, Node*** table);

int get_state(Node* node);

void matrix_printer(int M, int N, Node*** table);
