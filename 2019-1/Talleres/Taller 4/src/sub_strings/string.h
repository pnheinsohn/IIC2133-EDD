#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Calcula el hash de un string en el intervalo dado desde 0 */
uint64_t initial_hash(char* str, int s, int e, int base);

/** Calcula el hash del string sacando el char out y poniendo el char in */
uint64_t incremental_hash(char out, char in, int base, int exp, uint64_t old_hash);

/** Compara dos strings y retorna True si son iguales */
bool str_compare(char* str1, char* str2, int len);
