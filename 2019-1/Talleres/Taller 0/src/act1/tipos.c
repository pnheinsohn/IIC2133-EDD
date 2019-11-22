// En este archivo .c están las definiciones de las funciones del modulo
#include "tipos.h"
/** Módulo estándar de Input y Output */
#include <stdio.h>

/** Esta funcion imprime algo segun el color que se le pase */
void identifica_color(Color c)
{
   // Hace algo distinto en cada caso.
  switch(c)
  {
    case Rojo: 
      // Estamos en el caso rojo
      printf("El color de la pasión\n");
      // Salimos del switch
      break;
    case Verde:
      // Estamos en el caso verde
      printf("El color de los bosques\n");
      // Salimos del switch
      break;
    case Azul:
      // Estamos en el caso azul
      printf("El color de los mares\n");
      // Salimos del switch
      break;
    default:
      // Por si no cayó en ningún otro caso
      printf("No conozco ese color");
  }
}

/** Función sin valor de retorno que saluda a un Gato */
void saludar_gatito(Gato g)
{
  if(g.pelo)
  {
    printf("Lindo gatito\n");
  }
  else
  {
    printf("Linda abominación\n");
  }  
}

/** Función sin valor de retorno que saluda a un Perro */
void saludar_perrito(Perro p)
{
  if(p.numero_de_patas <= 4)
  {
    printf("Lindo perrito\n");
  }
  else
  {
    printf("Lindo perrito?\n");
  }  
}