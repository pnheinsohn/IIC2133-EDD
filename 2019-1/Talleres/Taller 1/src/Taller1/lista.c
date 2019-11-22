#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

Node* node_new(uint64_t value)
{
  Node* n = malloc(sizeof(Node));
  n -> value = value;
  n -> next = NULL;
  return n;
}


List* list_new()
{
  List* new = malloc(sizeof(List));

  new -> head = NULL;
  new -> tail = NULL;

  return new;
}

void list_append(List* self, uint64_t value)
{
  Node* new = node_new(value);

  // No esta vacia
  if(self -> head)
  {
    self -> tail -> next = new;
  }
  // Está vacía
  else
  {
    self -> head = new;
  }
  
  self -> tail = new;
}

void list_print(List* self)
{
  Node* curr = self -> head;

  while(curr)
  {
    printf("%ld\n", curr -> value);
    curr = curr -> next;
  }
}

void list_destroy(List* self)
{
  Node* curr = self -> head;

  while(curr)
  {
    Node* prev = curr;
    curr = curr -> next;
    free(prev);
  }

  free(self);
}