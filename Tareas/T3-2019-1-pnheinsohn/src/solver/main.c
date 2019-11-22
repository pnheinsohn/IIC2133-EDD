#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "board.h"
#include "backtracking.h"
#include "../watcher/watcher.h"

int main(int argc, char **argv) {
  //////////////////////////////
  //          Input           //
  //////////////////////////////

  if (argc != 3) {
    printf("Uso: %s <test> <out>\nDonde\n", argv[0]);
    printf("\t<test> es la ruta al archivo a resolver\n");
    printf("\t<out> es la ruta al archivo donde se imprimirá el output\n");
    return 1;
  }

  char *input_filename = argv[1];
  FILE *input_file = fopen(input_filename, "r");
  if (!input_file) {
    fprintf(stderr, "El archivo %s no se pudo abrir. ¿Seguro que existe?\n", input_filename);
    return 2;
  }

  int height;
  int width;
  fscanf(input_file, "%d %d", &height, &width);

  // Asignamos las celdas originales a Board
  int puentes;
  int row, col, degree;
  fscanf(input_file, "%d", &puentes);
  // Inicializamos el board con su largo y ancho
  Board *board = board_init(height, width, puentes);
  for (int i = 0; i < puentes; i++) {
    fscanf(input_file, "%d %d %d", &row, &col, &degree);
    if (col < board->range[0]) board->range[0] = col;
    if (col > board->range[1]) board->range[1] = col;
    if (row < board->range[2]) board->range[2] = row;
    if (row > board->range[3]) board->range[3] = row;
    board->cells[row][col]->type = PAINTED;
    board->cells[row][col]->degree = degree;
    board->cells[row][col]->pos = (Position) {.row = row, .col = col};
    board->bridges[i] = board->cells[row][col];
    board->bridges[i]->index = i;  // modify later
    board->used_bridges[i] = malloc(sizeof(MoveChanges));
    board->used_bridges[i]->amount_painted = 0;
    board->used_bridges[i]->positions = malloc(sizeof(Position));
  }

  // Asignamos las coordenadas de la meta al Board
  int goal_row, goal_col;
  fscanf(input_file, "%d %d", &goal_row, &goal_col);
  board->goal.row = goal_row;
  board->goal.col = goal_col;
  board->cells[board->goal.row][board->goal.col]->type = OBJECTIVE;
  fclose(input_file);

  //////////////////////////////////
  //        Funcionalidades       //
  //////////////////////////////////

  // watcher_open(board->height, board->width);
  // for (int i = 0; i < board->n_bridges; i++){
  //   watcher_set_cell_status(board->bridges[i]->pos.row, board->bridges[i]->pos.col, true);
  //   watcher_set_cell_degree(board->bridges[i]->pos.row, board->bridges[i]->pos.col, board->bridges[i]->degree);
  // }

  // watcher_set_cell_goal(board->goal.row, board->goal.col);

  // usleep(5000000);

  heuristic(board);
  Solution *solution = backtracking(board, 0, board->bridges, board->n_bridges);
  printf("Undos: %i\n", board->amount_undos);
  // watcher_refresh();
  // usleep(5000000);
  // watcher_close();


  /////////////////////////
  //        Output       //
  /////////////////////////

  // Abrimos el archivo en modo escritura
  char *output_filename = argv[2];
  FILE *output_file = fopen(output_filename, "w");

  if (!output_file) {
    fprintf(stderr, "El archivo %s no se pudo abrir. ¿Tienes los permisos necesarios?\n", input_filename);
    return 2;
  }

  // Imprimimos un mensaje de prueba en el archivo
  fprintf(output_file, "%i\n", solution->n_movements);
  for (int i = 0; i < solution->n_movements; i++) {
    fprintf(output_file, "%i %i %i\n", solution->movements[i]->row, solution->movements[i]->col, solution->movements[i]->dir);
  }

  // Cerramos el archivo
  fclose(output_file);
  
  ////////////////////////
  //        Frees       //
  ////////////////////////

  board_destroy(board);
  solution_destroy(solution);

  /* Retornamos 0 indicando que todo salió bien */
  return 0;
}
