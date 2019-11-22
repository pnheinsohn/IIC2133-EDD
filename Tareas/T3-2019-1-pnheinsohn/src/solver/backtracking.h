#pragma once

#include "board.h"

typedef struct solution {
    int n_movements;
    Movement **movements;
} Solution;

/** Finds a solution for the board */
void heuristic(Board *board);
Solution *backtracking(Board *board, int iteration, Cell **bridges, int n_bridges);

/** Frees the solution */
void solution_destroy(Solution *solution);
