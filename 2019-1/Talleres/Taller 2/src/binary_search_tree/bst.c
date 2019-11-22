#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

/** Crea un nodo del binary tree con el key y value indicado */
BST* bst_init(int key, int value)
{
	BST* tree = malloc(sizeof(BST));

	tree -> key = key;
	tree -> value = value;

	tree -> right = NULL;
	tree -> left = NULL;

	return tree;
}

/** Inserta en el arbol dado la key y value indicadas */
void bst_insert(BST* tree, int new_key, int new_value)
{
	BST* arbol = bst_search(tree, new_key, true);
	arbol -> value = new_value;
}

/** Busca en el arbol el nodo con la key dada y retorna el nodo o NULL */
BST* bst_search(BST* tree, int key, bool insert)
{
	if (tree -> key == key){
		return  tree;
	}
	else if (key < tree -> key){
		if (tree -> left){
			return bst_search(tree -> left, key, insert);
		}
		else{
			if (insert){
				tree -> left = bst_init(key, 0);
			}
			return tree -> left;
		}
	}
	else {
		if (tree -> right){
			return bst_search(tree -> right, key, insert);
		}
		else{
			if(insert){
				tree -> right = bst_init(key, 0);
			}
			return tree -> right;
		}
	}
}

/** Destruye recursivamente el arbol */
void bst_destroy(BST* tree)
{
	if(tree){
		bst_destroy(tree -> left);
		bst_destroy(tree -> right);
		free(tree);
	}
}

/** Imprime los valores del arbol en el orden ascendente de
las claves*/
void bst_in_order(BST* tree)
{
	if(tree)
	{
		bst_in_order(tree->left);
		printf("%d, %d\n", tree->key, tree->value);
		bst_in_order(tree->right);
	}
}
