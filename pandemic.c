#include "pandemic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Crear un país dinámicamente
Pais *crear_pais(char nombre[]) {
  Pais *nuevo = (Pais *)malloc(sizeof(Pais));
  strcpy(nuevo->nombre, nombre);
  nuevo->sigt = NULL;
  nuevo->ant = NULL;
  nuevo->vecinos = NULL; // inicialmente no tiene vecinos
  nuevo->num_vecinos = 0;
  // inicializar problemas a cero
  for (int i = 0; i < 3; i++)
    nuevo->problemas[i] = 0;
  return nuevo;
}

// crear mapa
Mapa *crear_mapa() {
  Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));
  mapa->inicio = NULL;
  mapa->final = NULL;
  return mapa;
}

// agregar país a la lista doblemente enlazada
void agregar_pais(Mapa *mapa, char nombre[]) {
  Pais *nuevo = crear_pais(nombre);
  if (mapa->inicio == NULL) {
    mapa->inicio = nuevo;
    mapa->final = nuevo;
  } else {
    mapa->final->sigt = nuevo;
    nuevo->ant = mapa->final;
    mapa->final = nuevo;
  }
}

// buscar país por nombre
Pais *buscar_pais(Mapa *mapa, char nombre[]) {
  Pais *actual = mapa->inicio;
  while (actual != NULL) {
    if (strcmp(actual->nombre, nombre) == 0)
      return actual;
    actual = actual->sigt;
  }
  return NULL;
}

// agregar vecino a un país
void agregar_vecino(Pais *pais, Pais *vecino) {
  if (pais->vecinos == NULL) {
    pais->vecinos = (Pais **)malloc(sizeof(Pais *));
    pais->vecinos[0] = vecino;
    pais->num_vecinos = 1;
  } else {
    pais->num_vecinos++;
    pais->vecinos =
        (Pais **)realloc(pais->vecinos, pais->num_vecinos * sizeof(Pais *));
    pais->vecinos[pais->num_vecinos - 1] = vecino;
  }
}

// imprimir mapa con vecinos
void imprimir_mapa(Mapa *mapa) {
  Pais *actual = mapa->inicio;
  printf("--- MAPA DE PAISES Y SUS VECINOS ---\n\n");
  while (actual != NULL) {
    printf("Pais: %s\n", actual->nombre);
    if (actual->vecinos != NULL && actual->num_vecinos > 0) {
      printf("Vecinos: ");
      for (int i = 0; i < actual->num_vecinos; i++) {
        printf("%s", actual->vecinos[i]->nombre);
        if (i < actual->num_vecinos - 1)
          printf(", ");
      }
      printf("\n");
    } else {
      printf("Vecinos: Ninguno\n");
    }
    printf("\n");
    actual = actual->sigt;
  }
}

// asignar problemas aleatorios a 9 países
Pais **asignar_problemas(Mapa *mapa) {
  Pais **seleccionados = (Pais **)malloc(9 * sizeof(Pais *));
  int count = 0;
  int total_paises = 0;

  // Contar países
  Pais *actual = mapa->inicio;
  while (actual != NULL) {
    total_paises++;
    actual = actual->sigt;
  }

  if (total_paises < 9)
    return NULL;

  // Seleccionar 9 países aleatorios únicos
  while (count < 9) {
    int n = rand() % total_paises;
    actual = mapa->inicio;
    for (int i = 0; i < n; i++)
      actual = actual->sigt;

    // Verificar repetidos
    int repetido = 0;
    for (int j = 0; j < count; j++) {
      if (seleccionados[j] == actual) {
        repetido = 1;
        break;
      }
    }
    if (!repetido) {
      seleccionados[count] = actual;
      count++;
    }
  }

  // Asignar valores según indicaciones
  for (int i = 0; i < 9; i++) {
    if (i < 3) {                          // Primeros 3: alto
      seleccionados[i]->problemas[0] = 3; // inundaciones
      seleccionados[i]->problemas[1] = 2; // sequias
      seleccionados[i]->problemas[2] = 0; // deforestación
    } else if (i < 6) {                   // Siguientes 3: medio
      seleccionados[i]->problemas[0] = 2;
      seleccionados[i]->problemas[1] = 1;
      seleccionados[i]->problemas[2] = 0;
    } else { // Últimos 3: bajo
      seleccionados[i]->problemas[0] = 1;
      seleccionados[i]->problemas[1] = 1;
      seleccionados[i]->problemas[2] = 1;
    }

    // los otros dos problemas opcionales aleatorios
    seleccionados[i]->problemas[3] = rand() % 4; // perdida de biodiversidad
    seleccionados[i]->problemas[4] = rand() % 4; // desplazamiento de población
  }

  return seleccionados;
}

void imprimir_problemas_seleccionados(Pais **seleccionados, int n) {
  const char *nombres_problemas[5] = {
      "Inundaciones", "Sequias", "Deforestacion", "Perdida de biodiversidad",
      "Desplazamiento de poblacion"};

  printf("--- PAISES SELECCIONADOS CON PROBLEMAS ---\n\n");
  for (int i = 0; i < n; i++) {
    printf("Pais: %s\n", seleccionados[i]->nombre);
    for (int j = 0; j < 5; j++) {
      printf("%s: %d", nombres_problemas[j], seleccionados[i]->problemas[j]);
      if (j < 4)
        printf(", ");
    }
    printf("\n\n");
  }
}

Jugador *crear_jugador(char *nombre, Pais *paisActual) {
  Jugador *nuevo = (Jugador *)malloc(sizeof(Jugador));
  strcpy(nuevo->nombre, nombre);
  nuevo->actual = paisActual;
  return nuevo;
}

void mover_jugador(Jugador *jugador) {
  Pais *paisActual = jugador->actual;
  printf("El jugador %s se encuentra en: %s \n1. %s \n2. %s\nA cúal país desea "
         "moverse: \n",
         jugador->nombre, paisActual->nombre, paisActual->sigt->nombre,
         paisActual->ant->nombre);
  int opc = 0;
  do {
    scanf("%d", &opc);

    if (opc == 1) {
      jugador->actual = paisActual->sigt;
    } else if (opc == 2) {
      jugador->actual = paisActual->ant;
    } else {
      printf("La opción digitada no es válida.");
    }
  } while (opc != 1 && opc != 2);
}
Proyecto *crear_proyecto(int clave, char *nombre, char *descripcion,
                         TipoAccion tipo, char *paisesAplicados) {
  Proyecto *proyecto = malloc(sizeof(Proyecto));
  if (!proyecto)
    return NULL;

  proyecto->clave = clave;
  strncpy(proyecto->nombre, nombre, sizeof(proyecto->nombre) - 1);
  proyecto->nombre[sizeof(proyecto->nombre) - 1] = '\0';

  strncpy(proyecto->descripcion, descripcion,
          sizeof(proyecto->descripcion) - 1);
  proyecto->descripcion[sizeof(proyecto->descripcion) - 1] = '\0';

  strncpy(proyecto->paisesAplicados, paisesAplicados,
          sizeof(proyecto->paisesAplicados) - 1);
  proyecto->paisesAplicados[sizeof(proyecto->paisesAplicados) - 1] = '\0';

  proyecto->tipo = tipo;
  proyecto->sigt = NULL;

  return proyecto;
}

unsigned int hash(int clave, int capacidad) { return clave % capacidad; }

TablaHash *crear_tabla(int capacidad) {
  TablaHash *tabla = malloc(sizeof(TablaHash));
  if (!tabla)
    return NULL;

  tabla->capacidad = capacidad;
  tabla->cantidad = 0;
  tabla->tabla = calloc(capacidad, sizeof(Proyecto *));
  if (!tabla->tabla) {
    free(tabla);
    return NULL;
  }
  return tabla;
}

void redimensionar_tabla(TablaHash *tabla) {
  int nueva_capacidad = tabla->capacidad * 2;
  Proyecto **nuevo_arreglo = calloc(nueva_capacidad, sizeof(Proyecto *));
  if (!nuevo_arreglo)
    return;

  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *proyecto = tabla->tabla[i];
    while (proyecto) {
      Proyecto *sigt = proyecto->sigt;
      unsigned int index = hash(proyecto->clave, nueva_capacidad);
      proyecto->sigt = nuevo_arreglo[index];
      nuevo_arreglo[index] = proyecto;
      proyecto = sigt;
    }
  }

  free(tabla->tabla);
  tabla->tabla = nuevo_arreglo;
  tabla->capacidad = nueva_capacidad;
}

void insertar_proyecto(TablaHash *tabla, int clave, Proyecto *proyecto) {
  if ((float)tabla->cantidad / tabla->capacidad > 0.7) {
    redimensionar_tabla(tabla);
  }
  unsigned int indice = hash(clave, tabla->capacidad);
  proyecto->sigt = tabla->tabla[indice];
  tabla->tabla[indice] = proyecto;
  tabla->cantidad++;
}

Proyecto *buscar_proyecto(TablaHash *tabla, int clave, char *nombreProyecto) {
  unsigned int indice = hash(clave, tabla->capacidad);
  Proyecto *actual = tabla->tabla[indice];
  while (actual) {
    if (strcmp(actual->nombre, nombreProyecto) == 0) {
      return actual;
    }
    actual = actual->sigt;
  }
  return NULL;
}
