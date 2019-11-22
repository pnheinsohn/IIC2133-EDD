#pragma once

#include <stdint.h>

struct node;
typedef struct node Node;

struct node
{
  uint64_t value;
  Node* next;
};

typedef struct
{
  Node* head;
  Node* tail;
} List;


Node* node_new(uint64_t valor);

List* list_new();

void list_append(List* self, uint64_t value);

void list_print(List* self);

void list_destroy(List* self);