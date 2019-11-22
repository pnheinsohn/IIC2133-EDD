/****************************************************************************/
/*                            Acto 3: Heap y Stack                          */
/****************************************************************************/

/** Módulo donde definimos un par de tipos para el Acto 1 */
#include "tipos.h"
/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>
/** Módulo estandar de C, para solicitar memoria adicional */
#include <stdlib.h>

// Esta es una variable global donde marcaremos el "inicio del stack"
// Es simplemente la dirección de memoria de la primera Variable  del programa
// La idea es tomar la diferencia con dirección de memoria de nuestras variables
// para poder ver fácilmente su posición relativa dentro del stack.
void* stack_start;

// Segun la arquitectura de tu computador el stack puede crecer hacia arriba
// (las direcciones de memoria aumentan)
// O hacia abajo
// (las direcciones de memoria disminuyen)
// Esta funcion calcula la posicion de una variable relativa al inicio del stack
// Asume que la dirección de memoria provista está en el stack
int64_t direccion_relativa_en_stack(void* address)
{
  // El valor es positivo si crece hacia arriba, y negativo si crece hacia abajo
  return address - stack_start;  
}

/** Funcion que imprime tabs para denotar la profundidad a la que estamos */
void print_depth_prefix(uint32_t profundidad)
{
  for(int i = 0; i < profundidad; i++)
  {
    printf("\t");
  }
}

/** Funcion de ejemplo con una Variable  local */
void p(uint32_t profundidad)
{
  print_depth_prefix(profundidad);
  printf("Dentro de P a profundidad %d\n", profundidad);

  uint32_t variable_local;

  print_depth_prefix(profundidad);
  printf("Parametro de P en posicion %ld\n", direccion_relativa_en_stack(&profundidad));
  print_depth_prefix(profundidad);
  printf("Variable  de P en posicion %ld\n", direccion_relativa_en_stack(&variable_local));
}

/** Funcion de ejemplo con una Variable  local y una llamada a funcion */
void q(uint32_t profundidad)
{
  print_depth_prefix(profundidad);
  printf("Dentro de Q a profundidad %d\n", profundidad);

  p(profundidad + 1);
  
  uint32_t variable_local;

  print_depth_prefix(profundidad);
  printf("Parametro de Q en posicion %ld\n", direccion_relativa_en_stack(&profundidad));
  print_depth_prefix(profundidad);
  printf("Variable  de Q en posicion %ld\n", direccion_relativa_en_stack(&variable_local));
}

/** Funcion de ejemplo con una Variable  local y una llamada a funcion que llama a funcion */
void r(uint32_t profundidad)
{
  print_depth_prefix(profundidad);
  printf("Dentro de R a profundidad %d\n", profundidad);

  q(profundidad + 1);
  
  uint32_t variable_local;

  print_depth_prefix(profundidad);
  printf("Parametro de R en posicion %ld\n", direccion_relativa_en_stack(&profundidad));
  print_depth_prefix(profundidad);
  printf("Variable  de R en posicion %ld\n", direccion_relativa_en_stack(&variable_local));
}

// Nunca retornes punteros a una variable del stack, ya que pierden significado al salir 
// de la función
uint32_t* funcion_que_retorna_puntero_al_stack(uint32_t valor)
{
  uint32_t cuadrado = valor * valor;
  return &cuadrado;
}

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  // Usamos la dirección de esta variable como 0 relativo para las demás variables del STACK
  uint32_t primera_variable = 0;
  stack_start = &primera_variable;

  // Si llamamos a la función P, crea sus variables en el STACK
  p(0);
  // Al salir de P, el espacio usado por sus variables se marca como disponible
  printf("------------------------------\n");

  // Al llamar a Q, sus variables usaran la memoria disponible
  // Esta es la memoria que antes usaron las variables de P
  q(0);
  // Pero dentro de Q llamamos a P, por lo que esta a su vez utiliza nuevo espacio del STACK
  // Al salir de P la memoria que usaron sus variables se marca como disponible
  // Por lo que la variable local dentro de Q ahora habita en la memoria que acaba de usar P
  printf("------------------------------\n");

  // R llama a Q y esta a su vez llama a P. Fijate viendo los prints donde vive cada variable
  r(0); 
  printf("------------------------------\n");

  // Todas las variables locales se guardan en el STACK de memoria
  // Este no puede crecer ilimitadamente.
  // Si nuestro programa necesita más memoria, debemos recurrir al sistema operativo
  // Esto se hace solicitando al sistema operativo un bloque de memoria
  // Debemos especificar tamaño de este bloque
  // El conjunto de bloques solicitados se conoce como HEAP
  
  // MALLOC = Memory ALLOCation : solicita un bloque de memoria
  // El parámetro que recibe es el tamaño del bloque
  // Retorna la dirección de memoria donde comienza el bloque
  // En este caso estamos pidiendo exactamente 200 bytes de memoria
  uint32_t* a = malloc(200);

  // No hay diferencia en como se usa la memoria
  *a = 3299;
  printf("%d\n", *a);

  // Antes de terminar el programa debemos devolver la memoria solicitada
  // Al pasar la dirección del comienzo de un bloque el sistema operativo
  // lo libera entero ya que sabe su tamaño
  free(a);

  // Tratar de usar a ahora produce errores
  // printf("%d\n", *a);


  // En este caso estamos pidiendo exactamente la memoria necesaria para guardar un Perro
  Perro* b = malloc(sizeof(Perro));
  // La flecha para acceder a los elementos del struct es porque es un puntero
  b -> nombre = "Cojin";
  b -> numero_de_patas = 3;
  b -> raza = Labrador;

  free(b);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////  PELIGRO  /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // Como te habrás imaginado, el uso de punteros a variables del stack 
  // te puede generar problemas, ya que al salir del scope donde se creó esa variable
  // el puntero ya deja de tener significado y su valor puede cambiar inesperadamente
  // por solo llamar una función

  // Si tu compilador es generoso, este código correrá.
  // (Descomentalo para probarlo)
  // uint32_t* p1 = funcion_que_retorna_puntero_al_stack(3);
  // printf("P1 -> %d\n", *p1);
  // uint32_t* p2 = funcion_que_retorna_puntero_al_stack(7);
  // printf("P1 -> %d\n", *p1);
  // printf("P2 -> %d\n", *p2);

  return 0;
}
