/* Módulo a cargo de la visualización del programa */
#include "../watcher/watcher.h"
/* Módulo estándar de Input / Output */
#include <stdio.h>
/* Módulo estándar de C, para uso de memoria en el Heap*/
#include <stdlib.h>
/* Módulo estándar de números enteros */
#include <stdint.h>
/* Módulo estándar de Unix, para hacer pausas en el programa */
#include <unistd.h>
/* Aplicadas x mi */
#include "structs.h"


int main(int argument_count, char** arguments)
{
	if (argument_count != 3)
	{
		printf("Modo de uso: %s <estado inicial> <generaciones>\n\n", arguments[0]);
		printf("\testado inicial: archivo de texto plano que contiene la descripción inicial de la comunidad bacteriana\n\n");
		printf("\tgeneraciones: número de generaciones totales que deberán simularse\n");
		return 1;
	}

	/* El primer parametro es el nombre del archivo */
	char* filename = arguments[1];

	/* El segundo parametro es la cantidad de generaciones a simular */
	uint32_t iterations = atoi(arguments[2]);

	/* Abrimos el archivo y creamos el stream de lectura */
	FILE* file = fopen(filename, "r");

	/* Las variables donde guardaremos el alto y ancho */
	uint8_t M, N;
	/* Leemos el alto y el ancho */
	fscanf(file, "%hhu %hhu", &M, &N);
	/* Abrimos la ventana con las dimensiones dadas */
	watcher_open(M, N);

	Node*** table = malloc(sizeof(Node**)*M);
    for (int i=0; i < M; i++) {
        Node** nodes = malloc(sizeof(Node*)*N);
        table[i] = nodes;
    }

    for (int i=0; i < M; i++) {
        for (int j=0; j < N; j++) {
            Node* node = node_init(table);
            fscanf(file, "%i", &(node -> state));
            table[i][j] = node;
			watcher_set_cell(i, j, table[i][j] -> state);
        }
    }
	/* Cerramos el archivo una vez que lo terminamos de leer */
	fclose(file);

	watcher_refresh();
		
	node_assignment(M, N, table);

    for (int k=0; k < iterations; k++) {
		// matrix_printer(M, N, table);
        for (int i=0; i < M; i++) {
            for (int j=0; j < N; j++) {
				watcher_set_cell(i, j, get_state(table[i][j]));
				watcher_refresh();
            }
        }
		for (int i=0; i < M; i++) {
			for (int j=0; j < N; j++) {
				table[i][j] -> state = get_state(table[i][j]);
			}
		}
    }

	/* Hacemos efectivos los cambios de la ventana, mostrando el estado inicial */

	for (int i=0; i < M; i++){
		for (int j=0; j < N; j++){
			free(table[i][j] -> neighbours);
			free(table[i][j]);
		}
		free(table[i]);
    }
    free(table);

	/* Cerramos la ventana */
	watcher_close();
	return 0;
}
