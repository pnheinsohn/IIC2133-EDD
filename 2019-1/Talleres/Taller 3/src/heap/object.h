#pragma once

/** Struct que se va a almacenar en el heap. Tiene key y value */
typedef struct object
{
  /** Key por el cual se ordena en el heap */
  int key;
  /** Puede ser cualquier cosa. la idea es hacer un heap que guarde structs */
  int value;
} Object;

/** Inicializa el objeto */
Object* obj_init(int key, int value);

/** Destruye el objeto */
void obj_destroy(Object* obj);
