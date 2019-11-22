/****************************************************************************/
/*                            Acto 0: Tipos básicos                         */
/****************************************************************************/

/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos (true / false) */
#include <stdbool.h>

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{
  /* Imprime el mensaje en la consola */
  printf("Bienvenido al Acto 0: Tipos básicos\n");

  /////////////////////////// Números enteros ////////////////////////////////

  // Los números enteros pueden ser signed o unsigned
  // Y de 8, 16, 32 y 64 bits.

  // Unsigned 8-bit integer. Rango = 0..255 (2^8 - 1)
  uint8_t x1 = 255;

  // El %d es equivalente al {} para formatear el string en python
  // %d = formato decimal
  printf("x1 = %d\n", x1);

  // Signed 64-bit integer
  int64_t x2 = -1237129871289371329;

  // %lld = long long decimal 
  printf("x2 = %lld\n", x2);

  //////////////////////////// Números reales ////////////////////////////////

  // De 32 bits
  float y1 = 0.598222;

  // %f = float
  printf("y1 = %f\n", y1);

  // De 64 bits
  double y2 = 0.123113;

  // %lf = long float
  printf("y2 = %lf\n", y2);

  //////////////////////////////// Booleans //////////////////////////////////

  // Bool es un valor que es verdadero o falso
  bool z1 = true;
  if(z1)
  {
    printf("z1 = true\n");
  }

  // Las comparaciones retornan un valor de verdad (z2 = false)
  bool z2 = 2 <= 1;
  if(z2)
  {
    printf("z2 = true\n");
  }

  // Se ocupa ! para negar (z3 = false)
  bool z3 = !z1;
  if(z3)
  {
    printf("z3 = true\n");
  }

  // Se ocupa || para decir "or" (z4 = true)
  bool z4 = z1 || z2;
  if(z4)
  {
    printf("z4 = true\n");
  }

  // Se ocupa && para decir "and" (z5 = false)
  bool z5 = z1 && z2;
  if(z5)
  {
    printf("z5 = true\n");
  }


  uint8_t contador = 0;

  // Ojo: Cualquier cosa distinta de 0 es considerada como true.
  while(123879)
  {
    contador++;

    if(contador > 200)
    {
      break;
    }
  }

  printf("El contador es %d\n", contador);

  /////////////////////////////// Caracteres /////////////////////////////////

  // Ojo: Con comillas simples
  char a = 'w';
  printf("La letra es %c\n", a);

  ///////////////////////////////// Strings //////////////////////////////////
  
  // OJO: El string tiene 11 letras, pero son realmente 12 caracteres
  // El último caracter es el caracter nulo, '\0', que indica el fin del string
  char s[12] = "Hola Mundo!";

  printf("El string es %s\n", s);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////  PELIGRO  /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // Overflow (compilador avisa)
  uint8_t w1 = 360;
  printf("w1 = %d\n", w1);

  // Overflow (compilador no avisa)
  uint8_t w2 = 240;
  w2 += 100;
  printf("w2 = %d\n", w2);

  // OJO: La división de dos números enteros da resultado entero (literales o variables)
  uint8_t siete = 7;
  uint8_t dos = 2;
  printf("7//2 = %u\n", siete/dos);

  // Si lo que quieres es un valor real, usa casting de tipos.
  printf("7/2 = %lf\n", ((double)(siete))/((double)(dos)));

  // Podemos declarar una variable sin inicializarla
  int64_t w3;
  // Y usarla de todos modos
  printf("w3 = %lld\n", w3);

  // El programa tiene que retornar 0 cuando terminó correctamente
  return 0;
}
