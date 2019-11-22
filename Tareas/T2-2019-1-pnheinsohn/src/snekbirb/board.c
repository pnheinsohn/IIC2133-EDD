#include "../random/pcg_basic.h"
#include "board.h"
#include <stdlib.h>


/** Inicializa el tablero a partir del archivo */
Board* board_init(FILE* file) {
  // Creo el tablero
  Board* board = malloc(sizeof(Board));

  // Leo el alto y ancho del tablero
  fscanf(file, "%hhu %hhu", &board->height, &board->width);
  // Creo la matriz y agrego los elementos del tablero
  board->cells = malloc(sizeof(Cell*) * board->height);
  for (int row = 0; row < board->height; row++) {
    board->cells[row] = malloc(sizeof(Cell) * board->width);
    for (int col = 0; col < board->width; col++) {
      // Leo el tipo de elemento en la posicion dada
      fscanf(file, "%u", &board->cells[row][col].type);
    }
  }

  // Veo para cada celda de la matriz, la altura con respecto al terreno y el
  // tipo de celda a la que cae
  for (int col = 0; col < board->width; col++) {
    uint8_t height = 1;
    CellType fall_type = SPIKE;
    for (int row = board->height - 1; row >= 0; row--) {
      if (board->cells[row][col].type == EMPTY) {
        board->cells[row][col].height = height;
        board->cells[row][col].fall_type = fall_type;
        height++;
      } else if (board->cells[row][col].type == WALL) {
        fall_type = WALL;
        height = 1;
      } else {
        fall_type = SPIKE;
        height = 1;
      }
    }
  }

  // Leo la posicion del portal
  fscanf(file, "%hhu %hhu", &board->portal_row, &board->portal_col);

  // Creo la serpiente
  board->snek = snek_init();

  // Leo alto, ancho y largo del snek
  uint8_t snek_row;
  uint8_t snek_col;
  uint8_t snek_length;
  fscanf(file, "%hhu %hhu %hhu", &snek_row, &snek_col, &snek_length);

  // Itero sobre las posiciones agregandolos al snek
  for (int i = 0; i < snek_length; i++) {
    // Leo la direccion de la siguiente parte del snek
    Direction dir;
    fscanf(file, "%u", &dir);

    // Calculo la posicion del nodo segun la direccion
    switch (dir) {
      case UP: {
        snek_row -= 1;
        break;
      }
      case DOWN: {
        snek_row += 1;
        break;
      }
      case LEFT: {
        snek_col -= 1;
        break;
      }
      case RIGHT: {
        snek_col += 1;
        break;
      }
      case HEAD:
        break;
    }

    // Creo el nodo y lo agrego al snek
    Node* node = node_init(snek_row, snek_col);
    snek_expand(board->snek, node);

    // Veo si el nodo se apoya en algo
    // Veo si el nodo se apoya en algo
    if (node -> row + 1 == board -> height) board -> snek -> spikes++;
    else {
      CellType type = board -> cells[node -> row + 1][node -> col].type;
      if (type == WALL) board -> snek -> grounds++;
      else if (type == SPIKE) board -> snek -> spikes++;
    }
  }

  //////////////////////////////////
  //          Rellenar            //
  //////////////////////////////////
  
  // Actualizo hash
  board->hash = 0;
  
  // Genero la matriz random
  board->randoms = malloc(sizeof(uint64_t *) * board->height);
  for (int i = 0; i < board->height; i++) {
    board->randoms[i] = malloc(sizeof(uint64_t) * board->width);
    for (int j = 0; j < board->width; j++) {
      board->randoms[i][j] = get_random();
    }
  }

  // Retorno el tablero
  return board;
}

/** Destruye el tablero liberando todo */
void board_destroy(Board* board) {
  // Libero la matriz de celdas
  for (int row = 0; row < board->height; row++) {
    free(board->cells[row]);
    free(board->randoms[row]);
  }
  free(board->cells);
  free(board->randoms);

  // Libero el tablero
  free(board);
}

/** Copia el snek y hace el movimiento, luego retorna el nuevo snek. Si el
movimiento no es valido o mata a la serpiente, retorno NULL */
Snek* board_move(Board* board, Direction dir) {
  // Snek
  Snek* snek = board->snek;

  // Calculo la posicion donde va a moverse la cabeza de la serpiente segun la direccion
  uint8_t delta_row = 0;
  uint8_t delta_col = 0;
  switch (dir) {
    case UP: {
      delta_row = -1;
      break;
    }
    case DOWN: {
      delta_row = 1;
      break;
    }
    case LEFT: {
      delta_col = -1;
      break;
    }
    case RIGHT: {
      delta_col = 1;
      break;
    }
    case HEAD:
      break;
  }
  uint8_t row = snek->head->row + delta_row;
  uint8_t col = snek->head->col + delta_col;

  // Si salgo del mapa, retorno NULL
  if (row >= board->height || row < 0 || col >= board->width || col < 0) {
    return NULL;
  }

  // Si en la posicion hay una pared o una espina, retorno false
  if (board->cells[row][col].type) return NULL;

  // Si el snek camina sobre si misma, retorno NULL
  for (Node* node = snek->head->next; node; node = node->next) {
    if (row == node->row && col == node->col) return NULL;
  }

  // Creo una copia de la serpiente
  Snek* copy = snek_copy(board->snek);

  // Si la cola esta apoyada en algo, actualizo el numero de apoyos
  uint8_t tail_row = copy->tail->row;
  uint8_t tail_col = copy->tail->col;
  Cell tail_cell = board->cells[tail_row][tail_col];
  if (tail_cell.height == 1) {
    if (tail_cell.fall_type == SPIKE)
      copy->spikes--;
    else
      copy->grounds--;
  }

  // Hago el movimiento
  snek_move(copy, dir);

  // Si la cabeza ahora se apoya en algo, actualizo los apoyos
  // Si la cola esta apoyada en algo, actualizo el numero de apoyos
  uint8_t head_row = copy->head->row;
  uint8_t head_col = copy->head->col;
  Cell head_cell = board->cells[head_row][head_col];
  if (head_cell.height == 1) {
    if (head_cell.fall_type == SPIKE)
      copy->spikes++;
    else
      copy->grounds++;
  }

  // Si quedo sobre el portal, retorno la serpiente
  if (copy->head->row == board->portal_row && copy->head->col == board->portal_col) {
    return copy;
  }

  // Si estoy apoyado solo en espinas, libero la serpiente nueva y retorno NULL
  if (copy->spikes > 0 && copy->grounds == 0) {
    snek_destroy(copy);
    return NULL;
  }

  // Si el snek cae, lo muevo hacia abajo
  if (copy->spikes == 0 && copy->grounds == 0) {
    // Veo cuanto tiene que caer y los nuevos apoyos
    uint8_t fall = board->height;
    uint8_t spikes = 0;
    uint8_t grounds = 0;
    for (Node* node = copy->head; node; node = node->next) {
      // Veo cuanto cae el nodo y donde cae
      uint8_t height = board->cells[node->row][node->col].height;
      uint8_t obj = board->cells[node->row][node->col].fall_type;
      if (height < fall) {
        fall = height;
        if (obj == SPIKE) {
          spikes = 1;
          grounds = 0;
        } else {
          grounds = 1;
          spikes = 0;
        }
      } else if (height == fall) {
        if (obj == SPIKE)
          spikes++;
        else
          grounds++;
      }
    }

    // Si la cabeza pasa por el portal antes de terminar la caida, dejo la
    // cabeza en la posicion del portal
    uint8_t h_c = copy->head->col;
    uint8_t h_r = copy->head->row;
    uint8_t port_c = board->portal_col;
    uint8_t port_r = board->portal_row;
    if (h_c == port_c && h_r <= port_r && port_r <= h_r + fall - 1) {
      snek_fall(copy, port_r - h_r);
      copy->grounds = 0;
      copy->spikes = 0;
    } else {
      // Si muere, retorno NULL
      if (spikes > 0 && grounds == 0) {
        snek_destroy(copy);
        return NULL;
      }

      // Sino, actualizo la posicion de los nodos y los apoyos
      snek_fall(copy, fall - 1);
      copy->grounds = grounds;
      copy->spikes = spikes;
    }
  }

  // Retorno el nuevo snek
  return copy;
}

/** Retorna True si en el estado del tablero se resolvio el puzzle */
bool board_is_win(Board* board) {
  // Posicion del portal
  uint8_t pr = board->portal_row;
  uint8_t pc = board->portal_col;

  // Posicion de la cabeza
  uint8_t hr = board->snek->head->row;
  uint8_t hc = board->snek->head->col;

  // Veo si son las mismas
  return pr == hr && pc == hc;
}

/////////////////////////////////
//          Ayudantia          //
/////////////////////////////////

uint64_t hash_snake(Snek* snek) {
  Node* aux = snek->head->next;
  uint64_t hash = 0;
  uint64_t factor = pow(4, snek->size - 2);
  while (aux) {
    hash += factor * aux->direction;
    factor /= 4;
    aux = aux->next;
  }
  return hash;
}

static uint64_t incremental_snake(Snek* snek, Direction movement) {
  Direction in = OPPOSITE(movement);

  // La dirección a eliminar es la última de nuestro array directions.
  Direction out = snek->tail->direction;

  // Actualizo Hash
  uint64_t hash = snek->hash;
  uint64_t factor = pow(4, snek->size - 2);
  hash -= out;
  hash /= 4;
  hash += in * factor;
  return hash;
}

void hash_board(Board* board) {
  board->snek->hash = hash_snake(board->snek);

  // Xor entre h(snek) y el numero random de la posicion de la cabeza
  board->hash = board->snek->hash ^ board->randoms[board->snek->head->row][board->snek->head->col];
}

uint64_t incremental_board(Board* board, Direction movement) {
  board->snek->hash = incremental_snake(board->snek, movement);
  uint8_t x;
  uint8_t y;
  // Calculamos la posición de la serpiente
  if (movement == UP) {
    x = board->snek->head->row - 1;
    y = board->snek->head->col;
  } else if (movement == RIGHT) {
    x = board->snek->head->row;
    y = board->snek->head->col + 1;
  } else if (movement == DOWN) {
    x = board->snek->head->row + 1;
    y = board->snek->head->col;
  } else if (movement == LEFT) {
    x = board->snek->head->row;
    y = board->snek->head->col - 1;
  }
  
  // Xor entre h(snek) y el numero random de la posicion de la cabeza
  return board->snek->hash ^ board->randoms[x][y];
}
