#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/** Funcion que ve si una palabra es un palindromo */
bool isPalindrome(char* word, int i, int j)
{
  while (i < j)
  {
    if (word[i] != word[j])
    {
      return false;
    }
    i++;
    j--;
  }
  return true;
}

/** Funcion recursiva que resuelve el problema usando una tabla */
int minPalindrome(char* word, int i, int j, int** tabla){

  // Si ya tengo el resultado guardado en la tabla, lo retorno
  if (tabla[i][j] != -1)
  {
    return tabla[i][j];
  }
  // Si la palabra es un palindromo, almaceno el resultado en la tabla y lo retorno 1
  if (isPalindrome(word, i, j))
  {
    tabla[i][j] = 1;
    return 1;
  }

  // Calculo el minimo de palindromos iterando sobre las subdivisiones
  // Mi valor minimo lo inicializo en j - i + 1 porque es el numero de letras del substring
  int min = j - i + 1;
  int pal;
  for (int k = i; k < j; k++)
  {
    // Calculo el numero minimo de palindromos recursivamente para la division
    pal = minPalindrome(word, i, k, tabla) + minPalindrome(word, k + 1, j, tabla);
    // Me quedo con el minimo
    if (pal < min)
    {
      min = pal;
    }
  }

  // Almaceno el resultado en la tabla y lo retorno
  tabla[i][j] = min;
  return min;
}

int main(int argc, char *argv[]) {
  if (argc != 2)
  {
    printf("Modo de uso: ./program <word>\n");
    return 0;
  }
  char* word = argv[1];
  int len = strlen(word);
  printf("Tamaño del input: %i\n", len);

  // Creo la tabla de tamanio len(palabra) * len(palabra)
  int** tabla = malloc(sizeof(int*) * len);
  for (int i = 0; i < len; i++)
  {
    tabla[i] = malloc(sizeof(int) * len);
    for (int j = 0; j < len; j++)
    {
      tabla[i][j] = -1;
    }
  }

  // Calculo el numero minimo de palindromos
  printf("Numero de palindromos: %i\n", minPalindrome(word, 0, len - 1, tabla));

  // Libero la tabla
  for (int i = 0; i < len; i++)
  {
    free(tabla[i]);
  }
  free(tabla);

  return 0;
}
