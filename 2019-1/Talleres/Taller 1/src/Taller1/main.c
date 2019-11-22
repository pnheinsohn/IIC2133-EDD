#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(int argc, char const *argv[])
{
  List* list = list_new();

  for(int i = 0; i <= 1000; i++)
  {
    list_append(list, rand());
  }

  list_print(list);

  list_destroy(list);

  return 0;
}
