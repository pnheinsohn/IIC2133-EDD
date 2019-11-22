#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// functions
int max_two(int a, int b) {
    if (a >= b) return a;
    return b;
}

int max_three(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    else if (b >= a && b >= c) return b;
    else return c;
}

// Tree
typedef struct tree {
    int score;
    int total_score;
	bool setted;
    struct tree *mid;
    struct tree *left;
    struct tree *right;
} Tree;

Tree *tree_init(int score) {
    Tree *new = malloc(sizeof(Tree));
    new->score = score;
    new->total_score = score;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
	new->setted = false;
    return new;
}

int tree_max_path(Tree *actual_tree) {
    if (!actual_tree->left || actual_tree->setted) return actual_tree->total_score;  // Soy nodo hoja o ya lo calcule
    else if (!actual_tree->mid) {  // Tengo 2 hijos
        actual_tree->total_score += max_two(tree_max_path(actual_tree->left), tree_max_path(actual_tree->right));
		actual_tree->setted = true;
        return actual_tree->total_score;
    } else {
        actual_tree->total_score += max_three(tree_max_path(actual_tree->left), tree_max_path(actual_tree->right), tree_max_path(actual_tree->mid));
		actual_tree->setted = true;
        return actual_tree->total_score;
    }
}

void tree_destroy(Tree **trees, int n_trees) {
    for (int i = 0; i < n_trees; i++) {
        free(trees[i]);
    }
    free(trees);
}

int main(int argc, char *argv[]) {
	int levels;
	int *all_scores = malloc(sizeof(int));
	int n_scores = 0;
	while (scanf("%d", &levels) == 1) {
		if (levels == 0) break;
		n_scores++;
		all_scores = (int *) realloc(all_scores, n_scores * sizeof(int));
		
		int score;
		int n_nodes = 0;
		for (int i = 1; i <= levels; i++) n_nodes += i;
		
		Tree **actual_trees = malloc(sizeof(Tree*) * n_nodes);
		for (int i = 0; i < n_nodes; i++) {
			scanf("%d", &score);
			actual_trees[i] = tree_init(score);
		}

		for (int i = 0; i < levels; i++) {
			if (i < levels - 1) {
				int act_lvl_pos = 0;
				for (int j = 1; j <= i; j++) act_lvl_pos += j;
				for (int j = 0; j  <= i; j++) {
					actual_trees[act_lvl_pos + j]->left = actual_trees[act_lvl_pos + j + i + 1];
					actual_trees[act_lvl_pos + j]->right = actual_trees[act_lvl_pos + j + i + 2];
					if (i < levels - 2) actual_trees[act_lvl_pos + j]->mid = actual_trees[act_lvl_pos + j + 2 * (i + 2)];
				}
			}
		}

		all_scores[n_scores - 1] = tree_max_path(actual_trees[0]);
		tree_destroy(actual_trees, n_nodes);
	}
	for (int i = 0; i < n_scores; i++) {
		printf("%i\n", all_scores[i]);
	}
	free(all_scores);
	return levels;
}
