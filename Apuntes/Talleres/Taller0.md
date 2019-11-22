# Taller 0
## Act 3

Stack: Región especial de la memoria que guarda temporalmente variables creadas por cada función. Es una estructura de datos LIFO, por lo que cada vez que se declara una nueva variable o función, esta será ingresada al stack. Por otra parte, cada vez que una función termina, todas las variables de dicha función son liberadas.

Heap: Región de la memoria que no es manejada automaticamente, por lo que hay que emplear calloc() o malloc() para usarla. Toda memoria que se emplee en el Heap debe ser liberada con free() cuando no se necesite nuevamente. En caso de que no se realice lo anterior, nuestro programa tendrá lo que se denomina "memory leak".

VALGRIND ./"archivo" para ejecutar/debuggear memory leaks e incorcondancias de memoria.

## Stack vs Heap
### Stack
* Very fast access
* Don't have to explicitly de-allocate variables
* Space is managed efficiently by CPU, memory will not become fragmented
* Local variables only
* Limit on stack size (OS-dependent)
* Variables cannot be resized

### Heap
* Variables can be accessed globally
* No limit on memory size
* (relatively) slower access
* No guaranteed efficient use of space, memory may become fragmented over time as blocks of memory are allocated, then freed
* You must manage memory (you're in charge of allocating and freeing variables)
* Variables can be resized using realloc()

## Estructura
### Clase
struct Perro {

    int patas;

};


### Con typedef
typedef struct Perro {

    int patas;

} Perro;


### Instanciación
Perro * init_Perro(int patas) {

    Perro * p = malloc(sizeof(Perro));

    p -> patas = patas;

    return p;

};


### Uso
int main() {

    Perro * p = init_Perro(4);

    Free(p);

};


## Código de arreglo
typedef struct Arr {

    int size;

    int count;

    int * array;

} ArrayList;


ArrayList * init_arr() {

    ArrayList * arreglo = malloc(sizeof(ArrayList));

    arreglo -> count = 0;

    arreglo -> size = 128;

    arreglo -> array = malloc(sizeof(int) * arreglo -> size);

    return arreglo;

};


void append(ArrayList* arreglo, int d) {  // N° que estoy agregando al arreglo

    if (arreglo -> count == arreglo -> size) {

        int * aux = malloc(sizeof(int) * 2 * arreglo -> size);

        for (int=0; i < arreglo -> size; i++) {

            aux[i] = arreglo -> array[i];

        }

        arreglo -> size *= 2;

        Free(arreglo -> array);

        arreglo -> array = aux;

    }

    arreglo -> array[arreglo -> count] = d;

    arreglo -> count++;

};


void destroy(ArrayList * arreglo) {

    Free(arreglo -> array);

    Free(arreglo);

};


int main() {

    ArrayList * arreglo = init_arr();

    append(arreglo, 3);

    destroy(arreglo);

    return 0;

};
