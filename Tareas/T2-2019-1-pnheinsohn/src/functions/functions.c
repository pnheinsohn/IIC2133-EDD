#include "functions.h"
#include <stdio.h>

/////////////////////////
//        inits        //
/////////////////////////

/** Inicializa un nodo para una queue, retorna NULL si falla */
static NodeQueue *init_queue_node(Snek *snek, uint64_t hash, Direction *dirs, int dirs_amount) {
  NodeQueue *new = malloc(sizeof(NodeQueue));
  if (new == NULL) return NULL;

  new->snek = snek;
  new->hash = hash;
  new->dirs = dirs;
  new->dirs_amount = dirs_amount;
  new->next = NULL;

  return new;
}

/** Inicializa una queue, retorna NULL si falla */
Queue *init_queue(Board *board) {
  Queue *new = malloc(sizeof(Queue));
  if (new == NULL) return NULL;
  NodeQueue *node = init_queue_node(board->snek, board->hash, NULL, 0);
  node->dirs = malloc(sizeof(Direction));

  new->head = node;
  new->tail = node;

  return new;
}

/** Inicializa una hash table vacía, retorna NULL si falla */
HashTable *init_hash_table(uint64_t size) {
  if (size <= 0) return NULL;

  HashTable *new = malloc(sizeof(HashTable));
  if (!new) return NULL;

  new->entries = malloc(sizeof(HashEntry *) * size);
  if (!new->entries) return NULL;

  for (int i = 0; i < size; i++) new->entries[i] = NULL;

  new->size = size;
  new->num_entries = 0;

  return new;
}

//////////////////////////
//      Functions       //
//////////////////////////

/** Obtengo el proximo Snek y hash a revisar */
NodeQueue *deque(Queue *queue) {

  if (!queue->head) return NULL;
  NodeQueue *node = queue->head;
  queue->head = node->next;
  return node;
}

/** Agrego un NodeQueue a la queue */
void append_queue(Queue *queue, Snek *snek, uint64_t hash, Direction *dirs, int dirs_amount) {
  NodeQueue *new = init_queue_node(snek, hash, dirs, dirs_amount);
  if (!queue->head) {
    queue->head = new;
    queue->tail = new;
  } else {
    queue->tail->next = new;
    queue->tail = new;
  }
}

/** Calculo el indice del hash en la tabla */
uint64_t hash_index(HashTable *hash_table, uint64_t hash) {
  return hash % hash_table->size;
}

/** Agrego una entrada a la tabla de hash */
uint64_t hash_insert(HashTable *hash_table, uint64_t hash) {
  if (!hash_table) return -1;

  uint64_t i = hash_index(hash_table, hash);
  HashEntry *entry = hash_table->entries[i];

  // Llego hasta el final de la lista ligada
  while (entry) entry = entry->next;

  entry = malloc(sizeof(HashEntry));
  if (!entry) return -1;

  // Actualizo el hash
  entry->hash = hash;

  // Lo pongo al inicio de la lista ligada
  entry->next = hash_table->entries[i];
  hash_table->entries[i] = entry;

  // Actualizo la cantidad de entradas y tamaño en caso de ser necesario
  hash_table->num_entries++;
  if (hash_table->num_entries >= hash_table->size * 0.7) {
    hash_table->size *= 2;
    hash_table = (HashTable *)realloc(hash_table, hash_table->size);
    if (!hash_table) return -1;
  }

  return i;
}

/** Revisa si un estado está en la tabla o no */
bool hash_in_table(HashTable *hash_table, uint64_t hash) {
  uint64_t i = hash_index(hash_table, hash);
  HashEntry *entry = hash_table->entries[i];

  while (entry) {
    if (entry->hash == hash) return true;
    entry = entry->next;
  }
  return false;
}

////////////////////////////
//        Printers        //
////////////////////////////

void queue_printer(Queue *queue) {
  NodeQueue *head = queue->head;
  printf("Heads Queue: [");
  while (head) {
    printf("(%i, %i), ", head->snek->head->row, head->snek->head->col);
    head = head->next;
  }
  printf("]\n");
}

void snek_printer(Snek *snek) {
  Node *head = snek->head;
  printf("Snek Dirs: [");
  while (head) {
    printf("%i, ", head->direction);
    head = head->next;
  }
  printf("]\n");
}

/////////////////////
//      Frees      //
/////////////////////

static void queue_nodes_destroy(NodeQueue *node) {
  if (node) {
    queue_nodes_destroy(node->next);
    snek_destroy(node->snek);
    free(node->dirs);
    free(node);
  }
}

void destroy_queue(Queue *queue) {
  queue_nodes_destroy(queue->head);
  free(queue);
}

static void destroy_hash_table_entry(HashEntry *entry) {
  if (entry) {
    destroy_hash_table_entry(entry->next);
    free(entry);
  }
}

void destroy_hash_table(HashTable *hash_table) {
  for (int i = 0; i < hash_table->size; i++) {
    HashEntry *entry = hash_table->entries[i];
    if (entry) destroy_hash_table_entry(entry);
  }
  free(hash_table->entries);
  free(hash_table);
}