# C
## Buenas Prácticas

Todo se ejecuta desde el archivo main.c que tiene la funcion main(), la idea es que solo esté esa función ahí, y no se definan estructuras. Sin embargo, uno no quiere acceder a todo lo que se implementa en ese archivo, por lo que se crea otro archivo struct.h que va a definir estructuras y "avisará" las funciones que main.c, es decir, no define código. Por otra parte, podemos tener el archivo struct.c que implementará netamente funciones internas que no son necesarias en nuestro main.c (mediante el uso de 'static funcName').

Atenti, solo se pueden importar los archivos cuya extensión es .h. Nunca importar un .c desde un .c, ya que no se sabe cuál va a ser el orden de compilación.
Mediante #pragma once evito algo que no recuerdo.

Cuando se compila un archivo .c, se generan archivos duplicados pero .o que se encargan de compilar solo los cambios al momento de recompilar.

## Lista Ligada
### Archivo linked_list.h
\#pragma once

typedef struct node Node;


struct node {

    int value;

    node* next;

};

// o bien puede ser

typedef struct node {
    
    int value;

    struct node* next;

} Node;


typedef struct linked_list {

    Node* head;

    Node* tail;

} LinkedList;


LinkedList* ll_init();


void ll_destroy(LinkedList* ll);


// Concatenar lista ligada
void concat(LinkedList* l1, LinkedList* l2);


### Archivo linked_list.c
\# include <stdlib.h>
\# include "linked_list.h"


LinkedList* ll_init() {

    LinkedList* ll = malloc(sizeof(LinkedList));

    ll -> head = NULL;

    ll -> tail = NULL;

    retrun ll;

}


void ll_destroy(LinkedList* ll) {

    if (ll -> head) {

        node_destroy(ll -> head);

    }

    free(ll);

}


void ll_append(LinkedList* ll, Node* no)


static Node* node_init(int value) {

    Node* node = malloc(sizeof(Node));

    node -> value = value;

    node -> next = NULL;

    return node;

}

static void node_destroy(Node* node) {

    if (node -> next) {

        node_destroy(node -> next);

    }

    free(node);

}


void concat(LinkedList* l1, LinkedList* l2) {

    last -> next = first;

}


### main.c
\# include <stdio.h>
\# include <stdint.h>
\# include <stdbool.h>
\# include "linked_list.h"


int main(int argc, char* argv[]) {  // argc tiene la cantidad de argumentos que recibe, mientas que argv es un arreglo de caracteres del Heap

    Node* head = node_init(0);

    Node* tail = head;

    for (int i = 0; i < 99; i++) {

        Node* new = node_init(i + 1);

        concat(tail, new);

        tail = new;

    }


    node_destroy(head);


    return 0;

}
