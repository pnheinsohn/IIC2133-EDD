#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int main(int argc, char*argv[])
{
  if (argc != 4)
  {
    printf("Modo de uso: ./sub_strings big_string small_string b\n");
    return 0;
  }

  // Los strings en C funcionan como un arreglo de caracteres donde el ultimo
  // tiene valor 0. Esto permite imprimir un string sin dar el largo como input.

  //////////////////////////////////////////////////////////////////////////////
  //                              Leer input                                  //
  //////////////////////////////////////////////////////////////////////////////

  // Calculo el tamanio del string chico
  char* small_str = argv[1];
  int small_len = 0;
  while (small_str[small_len]) small_len += 1;

  // Calculo el tamanio del string grande
  char* big_str = argv[2];
  int big_len = 0;
  while (big_str[big_len]) big_len += 1;

  // Leo la base a usar
  int base = atoi(argv[3]);

  //////////////////////////////////////////////////////////////////////////////
  //                       Calcular hashes iniciales                          //
  //////////////////////////////////////////////////////////////////////////////

  // Calculo el hash del string chico
  uint64_t small_hash = initial_hash(small_str, 0, small_len - 1, base);

  // Calculo el hash del inicio del string grande
  uint64_t big_hash = initial_hash(big_str, 0, small_len - 1, base);

  //////////////////////////////////////////////////////////////////////////////
  //                          Contar repeticiones                             //
  //////////////////////////////////////////////////////////////////////////////

  // Itero por el string grande contando repeticiones del sub string
  int start = 0;
  int end = small_len - 1;
  int repetitions = 0;
  while (true)
  {
    if (small_hash == big_hash)
    {
      // Si el hash coincide comparo los strings caracter a caracter
      if (str_compare(small_str, &big_str[start], small_len)) repetitions++;
    }

    // Actualizo las posiciones de los indices
    start++;
    end++;

    // Si llegue al final del string, termino
    if (end == big_len) break;

    // Calculo el hash del nuevo sub string
    big_hash = incremental_hash(big_str[start - 1], big_str[end], base, small_len - 1, big_hash);
  }

  // Imprimo el numero de repeticiones
  printf("El string %s se repite %d veces en el string %s\n", small_str, repetitions, big_str);

  return 0;
}
