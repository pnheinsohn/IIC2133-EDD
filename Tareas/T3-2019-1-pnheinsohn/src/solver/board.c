#include "board.h"
#include "math.h"

//////////////////////////////
//          Inits           //
//////////////////////////////

/** Inicializa el tablero con celdas vacias*/
Board *board_init(int height, int width, int n_bridges) {
  Board *new = malloc(sizeof(Board));
  new->width = width;
  new->height = height;
  new->amount_undos = 0;
  new->n_candidates = 0;
  new->range[0] = (int) INFINITY;
  new->range[1] = (int) -INFINITY;
  new->range[2] = (int) INFINITY;
  new->range[3] = (int) -INFINITY;
  new->reached_goal = false;
  new->n_bridges = n_bridges;
  new->bridges = malloc(sizeof(Cell *) * n_bridges);
  new->candidates = malloc(sizeof(Cell *) * n_bridges);
  new->used_bridges = malloc(sizeof(MoveChanges *) * n_bridges);
  new->cells = malloc(sizeof(Cell **) * height);
  for (int i = 0; i < height; i++) {
    new->cells[i] = malloc(sizeof(Cell *) * width);
    for (int j = 0; j < width; j++) {
      new->cells[i][j] = malloc(sizeof(Cell));
      new->cells[i][j]->degree = 0;
      new->cells[i][j]->type = EMPTY;
    }
  }
  return new;
}

/////////////////////////////////
//          Functions          //
/////////////////////////////////

bool board_is_valid(Board *board, Cell *bridge, Direction dir, Cell **other_bridges, int left_bridges) {
  // Poda 1: No se puede ir afuera del cuadrante
  if (bridge->pos.col == board->range[0] && board->goal.col > bridge->pos.col && dir == LEFT) return false;
  if (bridge->pos.col == board->range[1] && board->goal.col < bridge->pos.col && dir == RIGHT) return false;
  if (bridge->pos.row == board->range[2] && board->goal.row > bridge->pos.row && dir == UP) return false;
  if (bridge->pos.row == board->range[3] && board->goal.row < bridge->pos.row && dir == DOWN) return false;

  // // Poda 2: Distacias de candidatos a objetivo no calzan
  // bool cut = true;
  // int degrees_left = 0;
  // for (int i = 0; i < left_bridges; i++) degrees_left += other_bridges[i]->degree;
  // if (degrees_left == 0) return true;
  // puts("---------");
  // printf("Bridge: (%i, %i) | Dir: %i\n", bridge->pos.row, bridge->pos.col, dir);
  // for (int i = 0; i < board->n_candidates; i++) {
  //   Cell *candidate = board->candidates[i];
  //   printf("Candidate: (%i, %i)\n", candidate->pos.row, candidate->pos.col);
  //   int manhattan = manhattan_distance(board->goal, candidate->pos);
  //   if (dir == UP) {
  //     if (candidate->pos.col < bridge->pos.col && bridge->pos.col < board->goal.col && bridge->pos.row - bridge->degree <= )
  //   }
  //   printf("Manhattan: %i | Degrees_left: %i\n", manhattan, degrees_left);
  //   if (manhattan <= degrees_left) cut = false;
  // }
  // printf("cut: %i\n", cut);
  // if (cut) return false;
  return true;
}

void board_move(Board *board, Cell *bridge, Direction dir) {
  // watcher_set_cell_degree(bridge->pos.row, bridge->pos.col, 0);
  // watcher_set_cell_status(bridge->pos.row, bridge->pos.col, true);

  Cell *aux_cell = bridge;
  int counter = bridge->degree;
  MoveChanges *change = board->used_bridges[bridge->index];
  free(change->positions);
  change->amount_painted = 0;
  change->positions = malloc(sizeof(Position) * bridge->degree);
  while (counter > 0) {
    Position new_pos = position_shift(aux_cell->pos, dir);
    if (new_pos.row == 255 || new_pos.row == board->height || new_pos.col == 255 || new_pos.col == board->width) {
      return;
    }
    aux_cell = board->cells[new_pos.row][new_pos.col];
    aux_cell->pos.row = new_pos.row;
    aux_cell->pos.col = new_pos.col;
    if (aux_cell->type == EMPTY) {
      change->positions[change->amount_painted] = new_pos;
      change->amount_painted++;
      aux_cell->type = PAINTED;
      counter--;
      // watcher_set_cell_status(aux_cell->pos.row, aux_cell->pos.col, true);
    }
    else if (aux_cell->type == OBJECTIVE) {
      // watcher_set_cell_status(aux_cell->pos.row, aux_cell->pos.col, true);
      board->reached_goal = true;
      return;
    }
  }
}

void board_undo(Board *board, int i) {
  MoveChanges *changed = board->used_bridges[i];
  for (int j = 0; j < changed->amount_painted; j++) {
    board->cells[changed->positions[j].row][changed->positions[j].col]->type = EMPTY;
    // watcher_set_cell_status(changed->positions[j].row, changed->positions[j].col, false);
  }
  // watcher_set_cell_degree(board->bridges[i]->pos.row, board->bridges[i]->pos.col, board->bridges[i]->degree);
}

////////////////////////////////
//          Printers          //
////////////////////////////////

/** Imprime el tablero */
void board_printer(Board *board) {
  printf(".");
  for (int i = 0; i < board->width - 1; i++) printf("---.");
  puts("---.");
  for (int i = 0; i < board->height; i++) {
    for (int j = 0; j < board->width; j++) {
      if (j == board->height - 1) printf("| %i |\n", board->cells[i][j]->type);
      else printf("| %i ", board->cells[i][j]->type);
    }
    if (i == board->width - 1) {
      printf("'");
      for (int j = 0; j < board->width - 1; j++) printf("---'");
      puts("---'");
    } else {
      printf("|");
      for (int j = 0; j < board->width - 1; j++) printf("---+");
      printf("---|\n");
    }
  }
}

//////////////////////////////
//          Frees           //
//////////////////////////////

/** Destruye el tablero */
void board_destroy(Board *board) {
  for (int i = 0; i < board->height; i++) {
    for (int j = 0; j < board->width; j++) free(board->cells[i][j]);
    free(board->cells[i]);
  }
  free(board->cells);
  free(board->bridges);
  free(board->candidates);
  for (int i = 0; i < board->n_bridges; i++) {
    free(board->used_bridges[i]->positions);
    free(board->used_bridges[i]);
  }
  free(board->used_bridges);
  free(board);
}