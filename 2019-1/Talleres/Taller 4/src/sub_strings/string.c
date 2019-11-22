#include "string.h"
#include <math.h>
#include <stdio.h>

/** Calcula el hash de un string en el intervalo dado desde 0 */
uint64_t initial_hash(char* str, int s, int e, int base)
{
  // Inicialmente tengo hash 0
  uint64_t hash = 0;

  // El factor es b^potencia, inicialmente potencia = 0 por lo que factor = 1
  uint64_t factor = 1;

  // Itero por el string sumando los elementos por las potencias
  for (int i = e; i >= s; i--)
  {
    // Sumo el valor del caracter multiplicado por la potencia correspondiente
    hash += factor * (int) str[i];
    // Aumento la potencia
    factor *= base;
  }

  // Retorno el hash
  return hash;
}

/** Calcula el hash del string sacando el char out y poniendo el char in */
uint64_t incremental_hash(char out, char in, int base, int exp, uint64_t old_hash)
{
  // Hash nuevo parto desde el anterior
  uint64_t hash = old_hash;

  // Calculo b^exp
  uint64_t factor = pow(base, exp);

  // Primero resto el aporte del primer caracter
  hash -= out * factor;

  // Luego multiplico todo por la base
  hash *= base;

  // Finalmente sumo caracter nuevo
  hash += in;

  // Retorno el hash
  return hash;
}

/** Compara dos strings y retorna True si son iguales */
bool str_compare(char* str1, char* str2, int len)
{
  for (int i = 0; i < len; i++)
  {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}
