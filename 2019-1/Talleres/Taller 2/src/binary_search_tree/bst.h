#pragma once
#include <stdbool.h>
/** Estructura de Árbol binario, con llave, valor, y sus dos hijos */
typedef struct binary_tree
{
	/** Clave por la cual se busca el nodo en el arbol */
	int key;
	/** Valor almacenado en el nodo */
	int value;
	/** Hijo izquierdo el nodo actual */
	struct binary_tree* left;
	/** Hijo derecho del nodo actual */
	struct binary_tree *right;
} BST;

/** Crea un nodo del binary tree con el key y value indicado */
BST* bst_init(int key, int value);

/** Inserta en el arbol dado la key y value indicadas */
void bst_insert(BST* tree, int new_key, int value);

/** Busca en el arbol el nodo con la key dada y retorna el nodo o NULL */
BST* bst_search(BST* tree, int key, bool insert);

/** Destruye recursivamente el arbol */
void bst_destroy(BST* tree);

/** Imprime los valores del arbol en el orden ascendente de
las claves*/
void bst_in_order(BST* tree);
