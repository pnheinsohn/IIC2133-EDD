#include "backtracking.h"

/////////////////////////////////
//          Functions          //
/////////////////////////////////

void heuristic(Board *board) {
  Cell *aux;
  for (int i = 0; i < board->n_bridges; i++) {
    if (board->bridges[i]->pos.row == board->goal.row || board->bridges[i]->pos.col == board->goal.col) {
      aux = board->bridges[board->n_bridges - 1 - board->n_candidates];
      board->bridges[board->n_bridges - 1 - board->n_candidates] = board->bridges[i];
      board->candidates[board->n_candidates] = board->bridges[i];
      board->bridges[i] = aux;
      board->n_candidates++;
    }
  }
}

Solution *backtracking(Board *board, int iteration, Cell **bridges, int n_bridges) {
  if (board->reached_goal) {
    Solution *sol = malloc(sizeof(Solution));
    sol->movements = malloc(sizeof(Movement *) * (iteration + 1));
    sol->n_movements = iteration;
    return sol;
  }
  Direction mov[4] = {UP, LEFT, RIGHT, DOWN};
  for (int i = 0; i < n_bridges; i++) {
    Cell **aux_bridges = malloc(sizeof(Cell *) * (n_bridges - 1));
    for (int j = 0; j < n_bridges; j++) {
      if (j < i) aux_bridges[j] = bridges[j];
      else if (j > i) aux_bridges[j - 1] = bridges[j];
    }
    for (int j = 0; j < 4; j++) {
      if (!board_is_valid(board, bridges[i], mov[j], aux_bridges, n_bridges - 1)) continue;
      board_move(board, bridges[i], mov[j]);
      // watcher_refresh();
      // usleep(500000);
      Solution *solution = backtracking(board, iteration + 1, aux_bridges, n_bridges - 1);
      if (solution) {
        solution->movements[iteration] = malloc(sizeof(Movement));
        solution->movements[iteration]->row = bridges[i]->pos.row;
        solution->movements[iteration]->col = bridges[i]->pos.col;
        solution->movements[iteration]->dir = mov[j];
        free(aux_bridges);
        return solution;
      }
      board_undo(board, bridges[i]->index);
      board->amount_undos++;
    }
    free(aux_bridges);
  }
  return NULL;
}

//////////////////////////////
//          Frees           //
//////////////////////////////

void solution_destroy(Solution *solution) {
  if (solution) {
    for (int i = 0; i < solution->n_movements; i++) {
      free(solution->movements[i]);
    }
    free(solution->movements);
    free(solution);
  }
}