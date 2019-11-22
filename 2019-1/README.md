# IIC2133 - Estructuras de Datos y Algoritmos
## 2019-1

Bienvenido al sitio web del curso de Estructuras de Datos y Algoritmos. En esta página podrás encontrar la información administrativa del curso. En el repositorio podrás encontrar código ya preparado por tus ayudantes, junto con los eventuales enunciados de las tareas y las diapositivas de clases.

Asegúrate de echar una mirada a la [Wiki](https://github.com/IIC2133-PUC/2019-1/wiki) del curso, donde estarán todas las guías de cómo prepararte para poder trabajar en este ramo.

## Tabla de contenidos
 * [Equipo](#equipo)
     * [Profesor](#profesor)
     * [Ayudantes](#ayudantes)
 * [Acerca del curso](#acerca-del-curso) 
 * [Contenidos](#contenidos)
 * [Evaluación](#evaluación)
     * [Evaluaciones escritas](#evaluaciones-escritas)
     * [Tareas](#tareas)
 * [Política de Integridad Académica](#política-de-integridad-académica)

## Acerca del curso

Este curso enseña a analizar problemas, descomponerlos y elaborar algoritmos que los resuelvan en un tiempo óptimo. Esto se logra mediante la organización de la información en estructuras de datos y el uso de algoritmos capaces de aprovechar dichas estructuras. En particular se enseñan problemas de optimización en grafos, métodos de ordenación, y estructuración de la información en diccionarios.

El curso utiliza el lenguaje de programación C, ya que éste permite trabajar directa y limpiamente con los algoritmos aprendidos. Dado que este lenguaje trabaja en bajo nivel, se puede lograr escribir un programa que resuelva los problemas en tiempo óptimo no solo en la teoría, si no también en la práctica.
     
## Equipo

### Profesor

| Nombre               |  Email         |
|:-------------------- |:--------------|
| Yadran Eterovic Solano | yadran@ing.puc.cl |


### Ayudantes

| Nombre                | Email       | Github |
|:--------------------- |:-------------| :---------|
| Vicente Errázuriz | verrazuriz7@uc.cl | [@vichoeq](https://www.github.com/vichoeq) |
| Antonio López | alopez7@uc.cl | [@alopez7](https://www.github.com/alopez7) |
| Ignacio Hermosilla | ithermosilla@uc.cl | [@ignaciohermosillacornejo](https://www.github.com/ignaciohermosillacornejo) |
| Manuel Becker | mabecker@uc.cl| [@ManuelBeckerT](https://www.github.com/ManuelBeckerT) |
| Juan Espinosa | jgespinosa@uc.cl | [@jgespinosa10](https://www.github.com/jgespinosa10) |
| Jose Vigneaux | jtvigneaux@uc.cl | [@jtvigneaux](https://www.github.com/jtvigneaux) |
| José Morales | jmmorales@uc.cl | [@josemlira](https://www.github.com/josemlira) |
| Vicente Lubascher | velubascher@uc.cl | [@velubascher](https://www.github.com/velubascher) |
| Cristóbal Espinoza | caespinoza5@uc.cl | [@caespinoza5](https://www.github.com/caespinoza5) |
| Jaime Middleton | jamiddleton@uc.cl | [@Jamidd](https://www.github.com/Jamidd) |


## Contenidos

A continuación se presentan los contenidos del curso, no necesariamente en el orden en que serán impartidos.

* Estructuras elementales
 * Listas y colas LIFO y FIFO.
 * Heaps binarios.
* Diccionarios
 * Tablas de hash
 * Árboles de búsqueda
* Algoritmos
 * Ordenación
      * Ingenua ( *O*(n<sup>2</sup>) )
      * Inteligente ( *O*(n log(n)) )
      * Especializada ( *O*(n) )
 * Optimización en grafos
      * Orden Topológico
      * Cobertura
      * Búsqueda
      * Flujo
 * Técnicas Algorítmicas
      * Dividir y conquistar
      * Algoritmos Codiciosos
      * Programación Dinámica
      * Backtracking
* Strings

## Evaluación

El curso consta de una parte teórica, evaluada mediante evaluaciones escritas (también conocidas como controles), y una parte práctica, evaluada mediante tareas de programación en C, y asistencia a los talleres de los días viernes. Las asistencias y evaluaciones los talleres las encontrarán [aquí](https://docs.google.com/spreadsheets/d/13SDOUzKpQvF7OsdJR6Q7RbJbzHJuT7WUFcaVTGQLfF0/edit#gid=0).

### Evaluaciones Escritas

Habrá 7 controles y un examen, donde se evaluarán los aspectos más teóricos del contenido. Las notas de los controles se encontrarán [aquí](https://docs.google.com/spreadsheets/d/1LycqIq4lbAhLlCNbXuFScarkBFhlAc0BjHMoUivXIJg/edit#gid=0) .

| Evaluación | Fecha |
|:----------|:----------|
| C1 | 20 de marzo a las 14:00 | 
| C2 | 3 de abril a las 14:00 |
| C3 | 17 de abril a las 14:00 |
| C4 | 6 de mayo a las 14:00 |
| C5 | 22 de mayo a las 14:00 |
| C6 | 5 de junio a las 14:00 |
| C7 | 19 de junio a las 14:00 |
| Examen | 26 de junio a las 9:00 |

### Tareas

Habrá 5 tareas de programación en C, donde deberán resolver un problema complejo y analizarlo en un informe escrito. Las fechas serán anunciadas durante el semestre. Además, durante las primeras dos semanas se hará una actividad introductoria a C, la cual será evaluada. A medida que transcurra el semestre, los links con las notas a las respectivas tareas se irán agregando a continuación.

| Evaluación | Notas |
|:----------|:----------|
| T0 | [Notas T0](https://docs.google.com/spreadsheets/d/1CX4JEV6tfHoi9AkGuqm-P1aM2bEy3lbZY7OfrCTqKbQ/) | 
| T1 | [Notas T1](https://docs.google.com/spreadsheets/d/1Ud6C_J9nXiLpOl52lQUTOhi9bcfRcaKscOnMugTP1Hw/) |
| T2 | [Notas T2](https://docs.google.com/spreadsheets/d/1racNNVQR36SM1SFg9q7AzTrWTqTwmZQqn0kmDm_eDtA/) |

---

La nota final del curso se calcula de la siguiente manera:

```c++
double nota_final()
{
    /* La nota de la asistencia a los talleres */
    double AT;
    /* La nota de cada tarea */
    double T0,T1,T2,T3,T4;    
    /* La nota de cada evaluación escrita */
    double C1,C2,C3,C4,C5,C6,C7,Ex;

    /* Promedio de tareas */
    double NT = (T0 + T1 + T2 + T3 + T4 + AT - min(T0, T1, T2, T3, T4, AT)) / 5;
    /* Promedio de controles */
    double NC = (C1 + C2 + C3 + C4 + C5 + C6 + C7 - min(C1, C2, C3, C4, C5, C6, C7)) / 6;
    /* Promedio de las evaluaciones escritas */
    double NE;
    if(Ex >= NC)
    {
        NE = (Ex + NC) / 2;
    }
    else
    {
        NE = (Ex + 2NC) / 3;
    }
    
    double NF = (NT + NE) / 2;
    
    /* Es necesario aprobar las evaluaciones escritas por separado para aprobar el curso */
    if(NE < 4 || NF < 4)
    {
       return min(NE, NF);
    }
    else
    {
       return min(NF, 7);
    }
}
```
## Política de integridad académica

Este curso se adscribe a la política de integridad académica de la Escuela de Ingeniería y el Departamento de Computación.

---

Los alumnos de la Escuela de Ingeniería de la Pontificia Universidad Católica de Chile deben mantener un comportamiento acorde a la Declaración de Principios de la Universidad.  En particular, se espera que **mantengan altos estándares de honestidad académica**.  Cualquier acto deshonesto o fraude académico está prohibido; los alumnos que incurran en este tipo de acciones se exponen a un Procedimiento Sumario. Es responsabilidad de cada alumno conocer y respetar el documento sobre Integridad Académica publicado por la Dirección de Docencia de la Escuela de Ingeniería (disponible en SIDING).

Específicamente, para los cursos del Departamento de Ciencia de la Computación, rige obligatoriamente la siguiente política de integridad académica. Todo trabajo presentado por un alumno para los efectos de la evaluación de un curso debe ser hecho individualmente por el alumno, sin apoyo en material de terceros.  Por “trabajo” se entiende en general las interrogaciones escritas, las tareas de programación u otras, los trabajos de laboratorio, los proyectos, el examen, entre otros.

**En particular, si un alumno copia un trabajo, o si a un alumno se le prueba que compró o intentó comprar un trabajo, obtendrá nota final 1.1 en el curso y se solicitará a la Dirección de Docencia de la Escuela de Ingeniería que no le permita retirar el curso de la carga académica semestral.**

Por “copia” se entiende incluir en el trabajo presentado como propio, partes hechas por otra persona.  **En caso que corresponda a “copia” a otros alumnos, la sanción anterior se aplicará a todos los involucrados**.  En todos los casos, se informará a la Dirección de Docencia de la Escuela de Ingeniería para que tome sanciones adicionales si lo estima conveniente. Obviamente, está permitido usar material disponible públicamente, por ejemplo, libros o contenidos tomados de Internet, siempre y cuando se incluya la referencia correspondiente y sea autorizado por los ayudantes.

Lo anterior se entiende como complemento al Reglamento del Alumno de la Pontificia Universidad Católica de 
Chile<sup><a name="pucCLBack">[1](#pucCL)</a></sup>.  Por ello, es posible pedir a la Universidad la aplicación de sanciones adicionales especificadas en dicho reglamento.

<sub>**<a name="pucCL">[1](#pucCL)</a>**: Reglamento del Alumno de la Pontificia Universidad Católica de Chile disponible en: http://admisionyregistros.uc.cl/alumnos/informacion-academica/reglamentos-estudiantiles [&#8593;](#pucCLBack)</sub>
