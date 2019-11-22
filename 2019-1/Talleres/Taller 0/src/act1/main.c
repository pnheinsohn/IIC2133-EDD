/****************************************************************************/
/*                            Acto 1: Tipos propios                         */
/****************************************************************************/


// Los tipos que se usan en el main están definidos en el archivo .h que está junto a este
// Asegurate de leerlo


/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>
/** Módulo en el que vamos a definir tipos de ejemplo en este taller */
#include "tipos.h"

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  /* Imprime el mensaje en la consola */
  printf("Bienvenido al Acto 1: Tipos propios\n");

  // Podemos usar el alias como un tipo cualquiera
  doble equis = 5.6;
  printf("El doble es %lf\n",equis);

  // Ambas formas son válidas
  enum color c1 = Rojo;
  identifica_color(c1);
  Color c2 = Azul;
  identifica_color(c2);

  identifica_color(Verde);

  // No compila ya que Amarillo no está en el enum
  //  identifica_color(Amarillo);

 
  // Creamos una instancia de gato

  Gato sphinx = (Gato)
  {
    .nombre = "Cleopatra",
    .pelo = false
  };

  saludar_gatito(sphinx);

  // Creamos una instancia de perro
  Perro cachupin = (Perro)
  {
    .raza = Otros,
    .nombre = "Cachupin",
    .numero_de_patas = 8
  };

  saludar_perrito(cachupin);



  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////  PELIGRO  /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // Los enum se representan internamente como numeros.
  // Si usas un numero, el compilador no puede verificar que sea correcto
  identifica_color(7);

  // Aqui tambien es posible usar variables no inicializadas
  Perro cachupin2;
  printf("N = %ld\n", cachupin2.numero_de_patas);

  return 0;
}
