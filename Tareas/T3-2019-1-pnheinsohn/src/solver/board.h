#pragma once
#include "../watcher/watcher.h"

#include <stdio.h>
#include <stdlib.h>
#include "./util.h"

typedef struct movement {
    uint8_t *row;
    uint8_t *col;
    Direction dir;
} Movement;

typedef enum cellType {
    EMPTY = 0,
    PAINTED = 1,
    OBJECTIVE = 2,
} CellType;

typedef struct cell {
    int index;
    int degree;
    Position pos;
    CellType type;
} Cell;

typedef struct moveChanges {
    Position *positions;
    int amount_painted;
} MoveChanges;

typedef struct board {
    int width;
    int height;
    // min < x < max && min < y < max
    int range[4];
    Cell ***cells;
    Position goal;
    int n_bridges;    
    Cell **bridges;
    int n_candidates;
    int amount_undos;
    Cell **candidates;
    bool reached_goal;
    MoveChanges **used_bridges;
} Board;

/** Inicializa el tablero con celdas vacias*/
Board *board_init(int height, int width, int n_bridges);

bool board_is_win(Board *board);

bool board_is_valid(Board *board, Cell *bridge, Direction dir, Cell **other_bridges, int left_bridges);

void board_move(Board *board, Cell *bridge, Direction dir);

void board_undo(Board *board, int i);

/** Imprime el tablero */
void board_printer(Board *board);

/** Destruye el tablero */
void board_destroy(Board *board);