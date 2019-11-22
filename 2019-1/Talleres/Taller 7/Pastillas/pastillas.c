#include <stdio.h>
#include <stdlib.h>

double*** tabla;

double pastillas(int e, int m, int dias)
{
	// Si tengo un valor negativo de pastillas retorno 0
	if(e < 0 || m < 0) return 0;

	// Transformo valores a doubles para poder dividir con decimales
	double enteras = e;
	double mitades = m;

	double probabilidad = 0;

	// Si ya calcule este valor, lo retorno
	if(tabla[e][m][dias] > 0)
	{
		return tabla[e][m][dias];
	}

	// Caso base>: Si estoy en el dia 0 retorno la porcion de mitades que hay
	if(dias == 0)
	{
		probabilidad = mitades / (enteras + mitades);
	}
	else
	{
		// Caso recursivo: Sumo la probabilidad de las 2 opciones
		probabilidad += (enteras / (enteras + mitades)) * pastillas(enteras - 1, mitades + 1, dias - 1);
		probabilidad += (mitades / (enteras + mitades)) * pastillas(enteras, mitades - 1, dias - 1);
	}

	// Almaceno el resultado en la tabla
	tabla[e][m][dias] = probabilidad;

	// Retorno la probabilidad
	return probabilidad;
}

int main(int argc, char const *argv[])
{
	if (argc != 4)
	{
		printf("Modo de uso: ./pastillas <enteras> <mitades> <dias>\n");
		return 0;
	}
	int enteras = atoi(argv[1]);
	int mitades = atoi(argv[2]);
	int dias = atoi(argv[3]);

	int cantidad = enteras + mitades;

	// Creo la tabla con 3 dimensiones
	tabla = malloc(sizeof(double**) * (cantidad + 1));
	for(int e = 0; e < cantidad + 1; e++)
	{
		tabla[e] = malloc(sizeof(double*) * (cantidad + 1));
		for(int m = 0; m < cantidad + 1; m++)
		{
			tabla[e][m] = malloc(sizeof(double) * (dias + 1));
			for(int d = 0; d < dias + 1; d++)
			{
				tabla[e][m][d] = -1;
			}
		}
	}

	// Calculo la probabilidad
	printf("Dia %d: %lf\n", dias, pastillas(enteras, mitades, dias - 1));

	// Libero la tabla
	for(int e = 0; e < cantidad + 1; e++)
	{
		for(int m = 0; m < cantidad + 1; m++)
		{
			free(tabla[e][m]);
		}
		free(tabla[e]);
	}
	free(tabla);

	return 0;
}
