#include <math.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <stdbool.h>
#include "kdtree.h"
#include "../functions/heap.h"

/* Un nodo no permite un hijo con MAX_LEAF_VECTORS vectores */
#define MAX_LEAF_VECTORS 7

//////////////////////////////////////
//          Privadas Utiles         //
//////////////////////////////////////

static bool id_not_in_heap(Heap *heap, int id) {
  for (int i = 0; i < heap->size; i++) {
    if (id == heap->array[i]->vector->id) return false;
  }
  return true;
}

static void fill_heap(Data *train, KDTree *kd, Heap *heap, Vector *objective) {
  if (heap->counter == heap->size) return;  /* condición recursiva */
  KDTree *priority_aux;
  KDTree *other_aux;
  if (kd->mid_index != -1) {  /* No soy un nodo hoja */
    if (kd->coordenates[0] <= objective->pos[0] && objective->pos[0] <= kd->coordenates[2] &&
        kd->coordenates[1] <= objective->pos[1] && objective->pos[1] <= kd->coordenates[3]) {
      priority_aux = kd->left;
      other_aux = kd->right;
    } else {
      priority_aux = kd->right;
      other_aux = kd->left;
    }
    fill_heap(train, priority_aux, heap, objective);
    if (heap->counter < heap->size) fill_heap(train, other_aux, heap, objective);
  }
  else {  /* Soy nodo hoja */
    for (int i = 0; i < kd->x_array->len; i++) {
      heap_insert(heap, train->vectors[kd->x_array->array[i]], distance(train->vectors[kd->x_array->array[i]], objective));
      if (heap->counter == heap->size) return;
    }
  }
}

static void get_k_neighbours(Data *train, KDTree *kd, Heap *heap, Vector *objective) {
  if (!collision(objective->pos[0], objective->pos[1], distance(objective, heap->array[0]->vector), 
      kd->coordenates[0], kd->coordenates[1], kd->coordenates[2], kd->coordenates[3])) return;  
  if (kd->mid_index == -1) {  // Nodo hoja
    for (int i = 0; i < kd->x_array->len; i++) {
      double dist = distance(objective, train->vectors[kd->x_array->array[i]]);
      if (dist < heap->array[0]->distance && id_not_in_heap(heap, kd->x_array->array[i])) {
        heap_remove(heap);
        heap_insert(heap, train->vectors[kd->x_array->array[i]], dist);
      }
    }
  } else {
    get_k_neighbours(train, kd->left, heap, objective);
    get_k_neighbours(train, kd->right, heap, objective);
  }
}

static KDTree *recursive_kdtree_init(Data *train, AuxIndex *x_array, AuxIndex *y_array) {
  double dim_coords[5];
  get_dim_and_coords(train, x_array, y_array, dim_coords);
  KDTree *kd = malloc(sizeof(KDTree));
  kd->dimension = dim_coords[0];
  kd->coordenates[0] = dim_coords[1];  // Min x
  kd->coordenates[1] = dim_coords[2];  // Max x
  kd->coordenates[2] = dim_coords[3];  // Min y
  kd->coordenates[3] = dim_coords[4];  // Max y
  kd->x_array = x_array;
  kd->y_array = y_array;
  kd->left = NULL;
  kd->right = NULL;
  if (x_array->len <= MAX_LEAF_VECTORS) {
    kd->mid_index = -1;
    return kd;
  }

  int mid_index;
  int arrays_len;
  if (x_array->len % 2 == 0) {
    mid_index = x_array->len / 2 - 1;
    arrays_len = mid_index + 1;
  } else {
    mid_index = x_array->len / 2;
    arrays_len = mid_index;
  }
  AuxIndex *x_left_array = new_aux_index(mid_index + 1);
  AuxIndex *x_right_array = new_aux_index(arrays_len);
  AuxIndex *y_left_array = new_aux_index(mid_index + 1);
  AuxIndex *y_right_array = new_aux_index(arrays_len);

  int l = 0, r = 0;
  bool deleted_index = false;
  if (kd->dimension == 0) {  /* Corte en eje x */
    kd->mid_index = x_array->array[mid_index];
    Vector *pivot = train->vectors[x_array->array[mid_index]];
    for (int i = 0; i < mid_index + 1; i++) {  /* Relleno por la izq x */
      x_left_array->array[i] = x_array->array[i];
    }
    for (int i = mid_index + 1, j = 0; i < x_array->len; i++, j++) {  /* Relleno por la der x */
      x_right_array->array[j] = x_array->array[i];
    }
    for (int i = 0; i < y_array->len; i++) {
      if (pivot->id == train->vectors[y_array->array[i]]->id) {  /* Vector mediana */
        deleted_index = true;
        y_left_array->array[l] = y_array->array[i];
        l++;
      } else if (pivot->pos[0] > train->vectors[y_array->array[i]]->pos[0]) {  /* menores a la izq */
        y_left_array->array[l] = y_array->array[i];
        l++;
      } else if (pivot->pos[0] == train->vectors[y_array->array[i]]->pos[0]){
        if (pivot->pos[1] > train->vectors[y_array->array[i]]->pos[1]) {  /* empate menores a la izq */
          y_left_array->array[l] = y_array->array[i];
          l++;
        } else if (pivot->pos[1] < train->vectors[y_array->array[i]]->pos[1] || deleted_index){  /* empate mayores a la der */
          y_right_array->array[r] = y_array->array[i];
          r++;
        } else {
          y_left_array->array[l] = y_array->array[i];
          l++;
        }
      } else {  /* mayores a la der */
        y_right_array->array[r] = y_array->array[i];
        r++;
      }
    }
  } else {  /* Corte en eje y */
    kd->mid_index = y_array->array[mid_index];
    Vector *pivot = train->vectors[y_array->array[mid_index]];
    for (int i = 0; i < mid_index + 1; i++) {
      y_left_array->array[i] = y_array->array[i];
    }
    for (int i = mid_index + 1, j = 0; i < y_array->len; i++, j++) {
      y_right_array->array[j] = y_array->array[i];
    }
    for (int i = 0; i < x_array->len; i++) {
      if (pivot->id == train->vectors[x_array->array[i]]->id) {
        deleted_index = true;
        x_left_array->array[l] = x_array->array[i];
        l++;
      } else if (pivot->pos[1] > train->vectors[x_array->array[i]]->pos[1]) {
        x_left_array->array[l] = x_array->array[i];
        l++;
      } else if (pivot->pos[1] == train->vectors[x_array->array[i]]->pos[1]){
        if (pivot->pos[0] > train->vectors[x_array->array[i]]->pos[0]) {
          x_left_array->array[l] = x_array->array[i];
          l++;
        } else if (pivot->pos[0] < train->vectors[x_array->array[i]]->pos[0] || deleted_index) {
          x_right_array->array[r] = x_array->array[i];
          r++;
        } else {
          x_left_array->array[l] = x_array->array[i];
          l++;
        }
      } else {
        x_right_array->array[r] = x_array->array[i];
        r++;
      }
    }
  }
  kd->left = recursive_kdtree_init(train, x_left_array, y_left_array);
  kd->right = recursive_kdtree_init(train, x_right_array, y_right_array);
  return kd;
}

////////////////////////////////////
//            Rellenar            //
////////////////////////////////////

/** Retorn true si el segmento de recta choca circulo */
static bool collision_line(double cx, double cy, double r, double x, double my, double My) {
  /*
    Esta funcion es para ayudarlos, no es importante entender su funcionamiento
    */

  // Veo si la recta intersecta al circulo
  double det = pow(r, 2) - pow(cx - x, 2);
  if (det < 0)
    return false;

  // Calculo los puntos de interseccion
  double y0 = cy + sqrt(det);
  double y1 = cy - sqrt(det);

  // Veo si alguno esta entre my y My
  if ((my <= y0 && y0 <= My) || (my <= y1 && y1 <= My))
    return true;

  // Retorno false
  return false;
}

/** Retorna true si el circulo definido por el heap toca al kdtree */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My) {
  /*
    Esta funcion es para ayudarlos, no es importante entender su funcionamiento
    */

  // Caso 1: El centro del circulo esta dentro de la caja
  if (cx >= mx && cx <= Mx && cy >= my && cy <= My)
    return true;

  // Caso 2: El centro de la caja esta dentro del circulo
  // Calculo centro de la caja
  double bx = (Mx + mx) / 2;
  double by = (My + my) / 2;

  // Veo si esta en el circulo
  if (pow(bx - cx, 2) + pow(by - cy, 2) <= pow(r, 2))
    return true;

  // Caso 3: Circunferencia choca con algun borde de la caja
  // Testeo con cada uno de los bordes
  if (collision_line(cx, cy, r, mx, my, My))
    return true;
  if (collision_line(cx, cy, r, Mx, my, My))
    return true;
  if (collision_line(cy, cx, r, my, mx, Mx))
    return true;
  if (collision_line(cy, cx, r, My, mx, Mx))
    return true;

  // Caso 5: No intersectan
  return false;
}

/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree *kd_init(Data *train) {
  Data* aux_data = new_data_init(train);

  AuxIndex *x_array = new_aux_index(train->count);
  AuxIndex *y_array = new_aux_index(train->count);

  qsort(aux_data->vectors, aux_data->count, sizeof(Vector *), x_qsort);
  
  for (int i = 0; i < aux_data->count; i++) {
    x_array->array[i] = aux_data->vectors[i]->id;
  }
  qsort(aux_data->vectors, aux_data->count, sizeof(Vector *), y_qsort);

  for (int i = 0; i < aux_data->count; i++) {
    y_array->array[i] = aux_data->vectors[i]->id;
  }
  data_destroy(aux_data);
  KDTree *kd = recursive_kdtree_init(train, x_array, y_array);
  // kd_printer(kd);
  return kd;
}

/** Busca los vecinos cercanos y los guarda en neighbours */
void knn(Vector **neighbours, KDTree *kd, Data *train_data, int k, Vector *objective) {

  Heap *heap = heap_init(k);

  fill_heap(train_data, kd, heap, objective);

  get_k_neighbours(train_data, kd, heap, objective);

  for (int i = 0; i < heap->size; i++) {
    neighbours[i] = heap->array[i]->vector;
  }
  destroy_heap(heap);
}

/** Libera la memoria del kdtree */
void kd_destroy(KDTree *kd) {
  if (kd) {
    kd_destroy(kd->left);
    kd_destroy(kd->right);
    free(kd->x_array->array);
    free(kd->y_array->array);
    free(kd->x_array);
    free(kd->y_array);
    free(kd);
  }
}

///////////////////////////////////
//            Printer            //
///////////////////////////////////

void kd_printer(KDTree *kd) {
  if (kd->mid_index != -1) {
    kd_printer(kd->left);
    kd_printer(kd->right);
  } else {
    for (int i = 0; i < kd->x_array->len; i++) {
      if (i == kd->x_array->len - 1) {
        printf("%i || ", kd->x_array->array[i]);
        break;
      }
      printf("%i, ", kd->x_array->array[i]);
    }
  }
}