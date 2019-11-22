#include <stdio.h>
#include <stdlib.h>

// Structs
typedef struct node {
    int horse[2];
    int movements;
    struct node *next;
} Node;

typedef struct queue {
    int length;
    Node *head;
    Node *tail;
} Queue;

typedef struct board {
    int M;
    int N;
    int horse[2];
} Board;

// Board
Board *board_init(int M, int N, int row_o, int col_o) {
    Board *new = malloc(sizeof(Board));
    new->M = M;
    new->N = N;
    new->horse[0] = row_o;
    new->horse[1] = col_o;
    return new;
}

void board_destroy(Board *board) {
    free(board);
}

// Queue
static Node *node_init(int horse[], int movements) {
    Node *new = malloc(sizeof(Node));
    new->horse[0] = horse[0];
    new->horse[1] = horse[1];
    new->movements = movements;
    new->next = NULL;
    return new;
}

Queue *queue_init(int horse[], int movements) {
    Queue *new = malloc(sizeof(Queue));
    new->length = 1;
    new->head = node_init(horse, movements);
    new->tail = new->head;
    return new;
}

void queue_printer(Queue *queue) {
    Node *aux = queue->head;
    printf("[");
    while(aux) {
        printf("(%i, %i)", aux->horse[0], aux->horse[1]);
        aux = aux->next;
        if (aux) printf(", ");
    }
    printf("]\n");
}

void queue_append(Queue *queue, int horse[], int movements) {
    Node *nodo = node_init(horse, movements);
    if (queue->length == 0) {
        queue->head = nodo;
        queue->tail = nodo;
    } else {
        queue->tail->next = nodo;
        queue->tail = nodo;
    }
    queue->length++;
}

Node *queue_deque(Queue *queue) {
    if (queue->head) {
        Node *nodo = queue->head;
        queue->head = nodo->next;
        queue->length--;
        return nodo;
    }
    return NULL;
}

static void node_destroy(Node *node) {
    if (node) {
        node_destroy(node->next);
        free(node);
    }
}

void queue_destroy(Queue *queue) {
    node_destroy(queue->head);
    free(queue);
}


int main(int argc, char *argv[]) {
    int M, N, p, q, row_o, col_o, row_f, col_f;
    int *all_scores = malloc(sizeof(int));
    int n_scores = 1;
    while (scanf("%d %d %d %d %d %d %d %d", &M, &N, &p, &q, &row_o, &col_o, &row_f, &col_f) == 8) {
        if (M == 0 && N == 0 && p == 0 && q == 0 && row_o == 0 && col_o == 0 && row_f == 0 && col_f == 0) break;

        row_o--;
        col_o--;
        row_f--;
        col_f--;

        int **all_states = calloc(M, sizeof(int *));
        for (int i = 0; i < M; i++) all_states[i] = calloc(N, sizeof(int));
        all_states[row_o][col_o] = 1;
        
        all_scores[n_scores - 1] = -1;

        Board *board = board_init(M, N, row_o, col_o);
        Queue *queue = queue_init(board->horse, 0);
        while (queue->length > 0) {
            // queue_printer(queue);
            Node *nodo = queue_deque(queue);
            if (nodo->horse[0] == row_f && nodo->horse[1] == col_f) {
                all_scores[n_scores - 1] = nodo->movements;
                free(nodo);
                break;
            }

            int up_right[2] = {nodo->horse[0] - p, nodo->horse[1] + q};
            if (up_right[0] >= 0 && up_right[1] < N && all_states[up_right[0]][up_right[1]] == 0) {
                all_states[up_right[0]][up_right[1]] = 1;  // Set as visited
                queue_append(queue, up_right, nodo->movements + 1);
            }

            int up_left[2] = {nodo->horse[0] - p, nodo->horse[1] - q};
            if (up_left[0] >= 0 && up_left[1] >= 0 && all_states[up_left[0]][up_left[1]] == 0) {
                all_states[up_left[0]][up_left[1]] = 1;  // Set as visited
                queue_append(queue, up_left, nodo->movements + 1);
            }

            int left_up[2] = {nodo->horse[0] - q, nodo->horse[1] - p};
            if (left_up[0] >= 0 && left_up[1] >= 0 && all_states[left_up[0]][left_up[1]] == 0) {
                all_states[left_up[0]][left_up[1]] = 1;  // Set as visited
                queue_append(queue, left_up, nodo->movements + 1);
            }

            int left_down[2] = {nodo->horse[0] + q, nodo->horse[1] - p};
            if (left_down[0] < M && left_down[1] >= 0 && all_states[left_down[0]][left_down[1]] == 0) {
                all_states[left_down[0]][left_down[1]] = 1;  // Set as visited
                queue_append(queue, left_down, nodo->movements + 1);
            }

            int down_left[2] = {nodo->horse[0] + p, nodo->horse[1] - q};
            if (down_left[0] < M && down_left[1] >= 0 && all_states[down_left[0]][down_left[1]] == 0) {
                all_states[down_left[0]][down_left[1]] = 1;  // Set as visited
                queue_append(queue, down_left, nodo->movements + 1);
            }

            int down_right[2] = {nodo->horse[0] + p, nodo->horse[1] + q};
            if (down_right[0] < M && down_right[1] < N && all_states[down_right[0]][down_right[1]] == 0) {
                all_states[down_right[0]][down_right[1]] = 1;  // Set as visited
                queue_append(queue, down_right, nodo->movements + 1);
            }

            int right_down[2] = {nodo->horse[0] + q, nodo->horse[1] + p};
            if (right_down[0] < M && right_down[1] < N && all_states[right_down[0]][right_down[1]] == 0) {
                all_states[right_down[0]][right_down[1]] = 1;  // Set as visited
                queue_append(queue, right_down, nodo->movements + 1);
            }

            int right_up[2] = {nodo->horse[0] - q, nodo->horse[1] + p};
            if (right_up[0] >= 0 && right_up[1] < N && all_states[right_up[0]][right_up[1]] == 0) {
                all_states[right_up[0]][right_up[1]] = 1;  // Set as visited
                queue_append(queue, right_up, nodo->movements + 1);
            }

            free(nodo);
        }

        n_scores++;
        all_scores = (int *) realloc(all_scores, sizeof(int) * n_scores);

        queue_destroy(queue);
        board_destroy(board);
        for (int i = 0; i < M; i++) {
            free(all_states[i]);
        }
        free(all_states);
    }
    // output
    for (int i = 0; i < n_scores - 1; i++) {
        printf("%i\n", all_scores[i]);
    }
    free(all_scores);
}