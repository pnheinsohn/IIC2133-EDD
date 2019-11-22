#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t** tabla;

uint64_t binomial(int n, int k)
{
  if(n == k) return 1;
  if(k == 0) return 1;

  // Si no está calculado
  if(tabla[n][k] == 0)
  {
    tabla[n][k] = binomial(n - 1, k) + binomial(n - 1, k - 1);
  }

  return tabla[n][k];
}

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf("Modo de uso: ./binomial <n> <k>\n");
		return 0;
	}

	int n = atoi(argv[1]);
	int k = atoi(argv[2]);

  // Inicializamos la tabla entera con 0s (calloc hace eso)
  // Ya que el binomial nunca puede ser 0, sabemos que un 0
  // indica que ese valor no ha sido calculado.
  tabla = calloc(n + 1, sizeof(uint64_t*));
  for(int row = 0; row < n + 1; row++)
  {
    tabla[row] = calloc(k + 1, sizeof(uint64_t));
  }

  printf("%d sobre %d = %lu\n", n, k, binomial(n, k));

	return 0;
}
