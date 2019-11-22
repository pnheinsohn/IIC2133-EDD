#include <stdio.h>
#include <stdbool.h>
#include "structs.h"


Node* node_init(Node*** table) {
    Node* n = malloc(sizeof(Node));
    n -> state = -1;
    n -> neighbours = malloc(sizeof(Node*)*8);
    return n;
}

void node_assignment(int M, int N, Node*** table) {
    for (int i=0; i < M; i++) {
        for (int j=0; j < N; j++) {
            if (i == 0 && j == 0) {  // (0, 0)
                table[i][j] -> neighbours[0] = table[M - 1][N - 1];
                table[i][j] -> neighbours[1] = table[M - 1][j];
                table[i][j] -> neighbours[2] = table[M - 1][j + 1];
                table[i][j] -> neighbours[3] = table[i][N - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[i + 1][N - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][j + 1];
            } else if (i == M - 1 && j == 0) {  // (M - 1, 0)
                table[i][j] -> neighbours[0] = table[i - 1][N - 1];
                table[i][j] -> neighbours[1] = table[i - 1][j];
                table[i][j] -> neighbours[2] = table[i - 1][j + 1];
                table[i][j] -> neighbours[3] = table[i][N - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[0][N - 1];
                table[i][j] -> neighbours[6] = table[0][j];
                table[i][j] -> neighbours[7] = table[0][j + 1];
            } else if (i == 0 && j == N - 1) {  // (0, N - 1)
                table[i][j] -> neighbours[0] = table[M - 1][j - 1];
                table[i][j] -> neighbours[1] = table[M - 1][j];
                table[i][j] -> neighbours[2] = table[M - 1][0];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][0];
                table[i][j] -> neighbours[5] = table[i + 1][j - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][0];
            } else if (i == M - 1 && j == N - 1) {  // (M - 1, N - 1)
                table[i][j] -> neighbours[0] = table[i - 1][j - 1];
                table[i][j] -> neighbours[1] = table[i - 1][j];
                table[i][j] -> neighbours[2] = table[i - 1][0];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][0];
                table[i][j] -> neighbours[5] = table[0][j - 1];
                table[i][j] -> neighbours[6] = table[0][j];
                table[i][j] -> neighbours[7] = table[0][0];
            } else if (i == 0) {  // Fila 0
                table[i][j] -> neighbours[0] = table[M - 1][j - 1];
                table[i][j] -> neighbours[1] = table[M - 1][j];
                table[i][j] -> neighbours[2] = table[M - 1][j + 1];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[i + 1][j - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][j + 1];
            } else if (j == 0) {  // Columna 0
                table[i][j] -> neighbours[0] = table[0][N - 1];
                table[i][j] -> neighbours[1] = table[0][j];
                table[i][j] -> neighbours[2] = table[0][j + 1];
                table[i][j] -> neighbours[3] = table[i][N - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[i + 1][N - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][j + 1];
            } else if (i == M - 1) {  // Fila M - 1
                table[i][j] -> neighbours[0] = table[i - 1][j - 1];
                table[i][j] -> neighbours[1] = table[i - 1][j];
                table[i][j] -> neighbours[2] = table[i - 1][j + 1];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[0][j - 1];
                table[i][j] -> neighbours[6] = table[0][j];
                table[i][j] -> neighbours[7] = table[0][j + 1];
            } else if (j == N - 1) {  // Columna N - 1
                table[i][j] -> neighbours[0] = table[i - 1][j - 1];
                table[i][j] -> neighbours[1] = table[i - 1][j];
                table[i][j] -> neighbours[2] = table[i - 1][0];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][0];
                table[i][j] -> neighbours[5] = table[i + 1][j - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][0];
            } else {  // Caso no borde
                table[i][j] -> neighbours[0] = table[i - 1][j - 1];
                table[i][j] -> neighbours[1] = table[i - 1][j];
                table[i][j] -> neighbours[2] = table[i - 1][j + 1];
                table[i][j] -> neighbours[3] = table[i][j - 1];
                table[i][j] -> neighbours[4] = table[i][j + 1];
                table[i][j] -> neighbours[5] = table[i + 1][j - 1];
                table[i][j] -> neighbours[6] = table[i + 1][j];
                table[i][j] -> neighbours[7] = table[i + 1][j + 1];
            }
        }
    }
}

static bool will_change(Node* node) {
    int counter = 0;
    if (node -> state == 0) {
        for (int i=0; i < 8; i++) {
            if (node -> neighbours[i] -> state == 1) {
                counter++;
            }
            if (counter == 3) {
                return true;
            }
        }
    } else if (node -> state == 1) {
        for (int i=0; i < 8; i++) {
            if (node -> neighbours[i] -> state == 2) {
                counter++;
            }
            if (counter == 3) {
                return true;
            }
        }
    } else if (node -> state == 2) {
        for (int i=0; i < 8; i++) {
            if (node -> neighbours[i] -> state == 0) {
                counter++;
            }
            if (counter == 3) {
                return true;
            }
        }
    }
    return false;
}

int get_state(Node* node) {
    if (node -> state == 0) {
        if (will_change(node)) {
            return 1;
        } else {
            return 0;
        }
    } else if (node -> state == 1) {
        if (will_change(node)) {
            return 2;
        } else {
            return 1;
        }
    } else {
        if (will_change(node)) {
            return 0;
        } else {
            return 2;
        }
    }
}

void matrix_printer(int M, int N, Node*** table) {
    printf("---------\n");
    for (int i=0; i < M; i++) {
        for (int j=0; j < N; j++) {
            printf("%i  ", table[i][j] -> state);
        }
        printf("\n\n");
    }
}