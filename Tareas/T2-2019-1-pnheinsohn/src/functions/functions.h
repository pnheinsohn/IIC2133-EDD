#pragma once
#include "../snekbirb/board.h"
#include <stdlib.h>

////////////////////////////////
//          Structs           //
////////////////////////////////

/** Estructura para representar nodos en una Queue */
typedef struct nodeQueue {
  /** Snek copia */
  Snek *snek;
  /** Hash del nodo actual */
  uint64_t hash;
  int dirs_amount;
  Direction *dirs;
  struct nodeQueue *next;
} NodeQueue;

/** Estructura para representar entradas en una HashTable */
typedef struct hashEntry {
  /** Hash del nodo actual */
  uint64_t hash;
  struct hashEntry *next;
} HashEntry;

/** Cola para hacer BFS */
typedef struct queue {
  NodeQueue *head;
  NodeQueue *tail;
} Queue;

typedef struct hashTable {
  uint64_t size;
  uint64_t num_entries;
  HashEntry **entries;
} HashTable;

//////////////////////////////
//          Inits           //
//////////////////////////////

/** Inicializa una queue, retorna NULL si falla */
Queue *init_queue(Board *board);

/** Inicializa una hash table vacía, retorna NULL si falla */
HashTable *init_hash_table(uint64_t size);

//////////////////////////////////
//          Functions           //
//////////////////////////////////

/** Obtengo el proximo Snek y hash a revisar */
NodeQueue *deque(Queue *queue);

/** Agrego un NodeQueue a la queue */
void append_queue(Queue *queue, Snek *snek, uint64_t hash, Direction *dirs, int dirs_amount);

/** Calculo el indice del hash en la tabla */
uint64_t hash_index(HashTable *table, uint64_t hash);

/** Agrego una entrada a la tabla de hash */
uint64_t hash_insert(HashTable *table, uint64_t hash);

/** Revisa si un estado está en la tabla o no */
bool hash_in_table(HashTable *table, uint64_t hash);

/////////////////////////////////
//          Printers           //
/////////////////////////////////

void queue_printer(Queue *queue);

void snek_printer(Snek *snek);

//////////////////////////////
//          Frees           //
//////////////////////////////

void destroy_queue(Queue *queue);

void destroy_hash_table(HashTable *hash_table);
