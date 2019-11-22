#include "random/pcg_basic.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Modo de uso: %s <archivo> <alto> <ancho> <semilla>\n\n", argv[0]);
		printf("\tarchivo: archivo donde se guardará el estado generado\n\n");
		printf("\talto: cantidad de filas de la cuadrícula\n\n");
		printf("\tancho: cantidad de columnas de la cuadrícula\n\n");
		printf("\tsemilla: semilla aleatoria, para replicar resultados\n\n");
		return 1;
	}

	char* filename = argv[1];
	int height = atoi(argv[2]);
	int width = atoi(argv[3]);
	int seed = atoi(argv[4]);
	pcg32_srandom(0x853c49e6748fea9bULL ^ seed, 0xda3e39cb94b95bdbULL);

	FILE* outfile = fopen(filename, "w");

	fprintf(outfile, "%d %d\n", height, width);

	for(int row = 0; row < height; row++)
	{
		for(int col = 0; col < width; col++)
		{
			int value = pcg32_random() % 3;
			fprintf(outfile, "%d%c", value, col == width - 1 ? '\n' : ' ');
		}
	}
	fclose(outfile);

	return 0;
}
