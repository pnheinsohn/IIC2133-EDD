/* Este es un archivo .h = HEADER correspondiente a un modulo de tu programa */
/* Aqui se definen los tipos y se declaran las funciones de este modulo */
/* En el .h solo es necesario declarar lo que va a ser llamado desde otro archivo */

// Indica que este archivo solo debe compilarse una vez
// Esto debe ir al inicio de cada .h
#pragma once

/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>

///////////////////////////////// Aliases ////////////////////////////////////

// La sintaxis es "typedef <tipo> <alias>"

// Para los que les guste programar en español.
typedef double doble;

////////////////////////////////// Enums /////////////////////////////////////

// Un enum es un tipo que permite solo valores dentro de una lista

// El nombre del tipo es "enum color", y su dominio son los valores dados
enum color
{
  Rojo,
  Azul,
  Verde
};
// También podemos llamarlo "Color"
typedef enum color Color;

// Podemos declarar el enum y darle un alias en un solo paso.
// En este caso, "enum roca {}" es el nombre del tipo
typedef enum roca
{
  Ignea,
  Metamorfica,
  Sedimentaria
} Roca;

// En este caso "enum {}" es un tipo sin nombre
// que solo tiene sentido al darle un alias.
// En este caso, el tipo existe solo bajo el nombre "Raza"
typedef enum
{
  Dalmata,
  Labrador,
  Golden,
  Otros
} Raza;

///////////////////////////////// Structs ////////////////////////////////////

// Los structs son tipos que encapsulan varias variables relacionadas,
// cada una con su propio nombre y tipo

// En el caso de los alias, aplican las mismas reglas que en los enum

// Este tipo se llama "struct gato"
struct gato
{
  char* nombre;
  bool pelo;
};

// Pero nos podemos referir a el como "Gato"
typedef struct gato Gato;

// En este caso, "struct {}" es un tipo sin nombre 
// al que solo nos podemos referir como "Perro"
typedef struct
{
  Raza raza;
  char* nombre;
  uint64_t numero_de_patas;
} Perro;

// OJO: En C no existen "clases" ni conceptos como "herencia" y "polimorfismo"

///////////////////////////////// Funciones //////////////////////////////////

// Para mantener orden recomendamos declarar las funciones al final del .h

/** Esta funcion imprime algo segun el color que se le pase */
void identifica_color(Color c);
/** Función sin valor de retorno que saluda a un Gato */
void saludar_gatito(Gato g);
/** Función sin valor de retorno que saluda a un Perro */
void saludar_perrito(Perro p);

// La definición de estas funciones puedes encontrarlas en el .c del mismo nombre.
// Asegurate de leerlo.