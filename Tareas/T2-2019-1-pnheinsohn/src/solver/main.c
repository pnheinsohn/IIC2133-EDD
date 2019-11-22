#include "../snekbirb/board.h"
#include "../watcher/watcher.h"
#include "../functions/functions.h"

int main(int argc, char* argv[]) {
  /////////////////////////
  //        Input        //
  /////////////////////////

  if (argc != 3) {
    printf("Modo de uso: ./snakebirb test.txt output.txt\n");
    return 0;
  }

  ////////////////////////////////
  //          Work Here         //
  ////////////////////////////////

  FILE* file = fopen(argv[1], "r");
  Board* board = board_init(file);
  hash_board(board);
  fclose(file);

  // Inicializo cola y HashTable
  Queue *queue = init_queue(board);
  uint64_t hash_table_size = sizeof(HashTable) * 1000000;
  HashTable *hash_table = init_hash_table(hash_table_size);

  Snek *aux_child;
  int dirs_amount = 0;
  Direction *aux_dirs;
  Direction *actual_dirs;
  uint64_t aux_hash = board->hash;
  while (true) {
    NodeQueue* queue_node = deque(queue);
    board->snek = queue_node->snek;
    board->hash = queue_node->hash;
    aux_dirs = queue_node->dirs;
    dirs_amount = queue_node->dirs_amount;

    free(queue_node);

    if (board_is_win(board)) break;

    for (int i = 0; i < 4; i++) {     
      aux_child = board_move(board, i);
      if (!aux_child) {
        continue;
      }
      aux_child->hash = board->snek->hash;
      aux_hash = incremental_board(board, i);
      if (hash_in_table(hash_table, aux_hash)) {
        snek_destroy(aux_child);
        continue;
      }
      board->hash = aux_hash;
      hash_insert(hash_table, board->hash);
      actual_dirs = malloc(sizeof(Direction) * (dirs_amount + 1));
      for (int j = 0; j < dirs_amount; j++) {
        actual_dirs[j] = aux_dirs[j];
      }
      actual_dirs[dirs_amount] = i;
      append_queue(queue, aux_child, board->hash, actual_dirs, dirs_amount + 1);
    }
    free(aux_dirs);
    snek_destroy(board->snek);
  }

  // printf("%i\n", dirs_amount);
  // for (int i = 0; i < dirs_amount; i++) {
  //   printf("%i\n", actual_dirs[i]);
  // }

  //////////////////////////////
  //          Output          //
  //////////////////////////////

  FILE* fout = fopen(argv[2], "w");
  fprintf(fout, "%d\n", dirs_amount);
  for (int i = 0; i < dirs_amount; i++) {
    fprintf(fout, "%u\n", aux_dirs[i]);
  }
  fclose(fout);

  ////////////////////////////////////////
  //           Free's & Return          //
  ////////////////////////////////////////

  free(aux_dirs);
  destroy_queue(queue);
  snek_destroy(board->snek);
  board_destroy(board);
  destroy_hash_table(hash_table);
  return 0;
}
