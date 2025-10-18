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
  for (int i = 0; i < 5; i++)
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
  int opc = 0;

  printf("El jugador %s se encuentra en: %s\n", jugador->nombre,
         paisActual->nombre);

  if (paisActual->sigt != NULL)
    printf("1. Moverse a %s\n", paisActual->sigt->nombre);
  if (paisActual->ant != NULL)
    printf("2. Moverse a %s\n", paisActual->ant->nombre);

  printf("Seleccione una opcion: ");

  do {
    scanf("%d", &opc);

    if (opc == 1 && paisActual->sigt != NULL) {
      jugador->actual = paisActual->sigt;
    } else if (opc == 2 && paisActual->ant != NULL) {
      jugador->actual = paisActual->ant;
    } else {
      printf("Opcion invalida. Intente de nuevo: ");
      opc = 0;
    }
  } while (opc == 0);
}

Proyecto *crear_proyecto(char *nombre, char *descripcion, TipoAccion tipo,
                         char *paisesAplicados) {
  Proyecto *proyecto = malloc(sizeof(Proyecto));
  if (!proyecto)
    return NULL;

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
      unsigned int index = hash(proyecto->nombre, nueva_capacidad);
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
  unsigned int indice = hash(proyecto->nombre, tabla->capacidad);
  proyecto->sigt = tabla->tabla[indice];
  tabla->tabla[indice] = proyecto;
  tabla->cantidad++;
}

void imprimir_tabla_hash(TablaHash *tabla) {
  printf("\n--- PROYECTOS REGISTRADOS ---\n");
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *actual = tabla->tabla[i];
    if (actual != NULL) {
      printf("\n[Indice %d]\n", i);
      while (actual) {
        printf("Nombre: %s\n", actual->nombre);
        printf("Descripción: %s\n", actual->descripcion);
        printf("Países aplicados: %s\n", actual->paisesAplicados);
        printf("Tipo: %s\n", actual->tipo == MEJORAR ? "MEJORAR" : "EMPEORAR");
        printf("-----------------------------\n");
        actual = actual->sigt;
      }
    }
  }
}

// --------------------------------------------------------
// HASH CON LLAVE DE TEXTO
// --------------------------------------------------------

// Calcula el hash de una cadena usando multiplicador primo (31)

// Inserta proyecto usando el nombre como llave de texto
void insertar_proyecto_texto(TablaHash *tabla, char *clave,
                             Proyecto *proyecto) {
  if ((float)tabla->cantidad / tabla->capacidad > 0.7) {
    redimensionar_tabla(tabla);
  }

  unsigned int indice = hash(clave, tabla->capacidad);
  proyecto->sigt = tabla->tabla[indice];
  tabla->tabla[indice] = proyecto;
  tabla->cantidad++;
}

// Busca proyecto usando texto como llave
Proyecto *buscar_proyecto(TablaHash *tabla, char *nombreProyecto) {
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *actual = tabla->tabla[i];
    while (actual != NULL) {
      if (strcmp(actual->nombre, nombreProyecto) == 0) {
        return actual;
      }
      actual = actual->sigt;
    }
  }
  return NULL; // no encontrado
}

void turno_jugador(Jugador *jugador, TablaHash *tabla, Mapa *mapa) {
  int acciones = 4;
  int opcion;

  while (acciones > 0) {
    printf("\nTurno de %s (%s) - Acciones restantes: %d\n", jugador->nombre,
           jugador->actual->nombre, acciones);
    printf("1. Moverse a un pais vecino\n");
    printf("2. Consultar estado del pais actual\n");
    printf("3. Implementar proyecto\n");
    printf("Seleccione una accion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
      mover_jugador(jugador);
      acciones--;
    } else if (opcion == 2) {
      mostrar_estado_pais(jugador->actual);
    } else if (opcion == 3) {
      mostrar_proyectos(tabla);
      char nombreProyecto[100];
      printf("Ingrese el nombre exacto del proyecto que desea aplicar: ");
      scanf(" %[^\n]", nombreProyecto);
      Proyecto *p = buscar_proyecto(tabla, nombreProyecto);
      if (p) {
        aplicar_proyecto(jugador, p);
      } else {
        printf("Proyecto no encontrado.\n");
      }
      acciones--;
    } else {
      printf("Opcion invalida.\n");
    }
  }
}

// Devuelve el nombre del problema según su índice
const char *nombreProblema(int tipo) {
  switch (tipo) {
  case 0:
    return "Inundaciones";
  case 1:
    return "Sequias";
  case 2:
    return "Deforestacion";
  case 3:
    return "Perdida de biodiversidad";
  case 4:
    return "Desplazamiento de poblacion";
  default:
    return "Desconocido";
  }
}

void aumentar_problemas(Mapa *mapa) {
  int total = 0;
  Pais *actual = mapa->inicio;

  // Contar países
  while (actual != NULL) {
    total++;
    actual = actual->sigt;
  }

  // Incrementar problemas en 3 países aleatorios
  for (int i = 0; i < 3; i++) {
    int random_index = rand() % total;
    actual = mapa->inicio;
    for (int j = 0; j < random_index; j++)
      actual = actual->sigt;

    int tipo = rand() % 5; // seleccionar problema aleatorio

    if (actual->problemas[tipo] < 3) {
      actual->problemas[tipo]++;
    } else {
      // Aumentar en los vecinos si ya está en 3
      for (int v = 0; v < actual->num_vecinos; v++) {
        if (actual->vecinos[v]->problemas[tipo] < 3)
          actual->vecinos[v]->problemas[tipo]++;
      }
    }

    printf("Problema %s aumentado en %s\n", nombreProblema(tipo),
           actual->nombre);
  }
}

void aplicar_proyecto(Jugador *jugador, Proyecto *proyecto) {
  Pais *pais = jugador->actual;

  printf("\nAplicando proyecto '%s' en %s...\n", proyecto->nombre,
         pais->nombre);

  // Cualquier proyecto puede aplicarse a los países afectados
  for (int i = 0; i < 5; i++) {
    if (pais->problemas[i] > 0)
      pais->problemas[i]--;
  }

  printf(" El proyecto '%s' ha reducido los problemas en %s.\n",
         proyecto->nombre, pais->nombre);

  mostrar_estado_pais(pais);
}

unsigned int hash(char *texto, int capacidad) {
  unsigned int hash = 0;
  for (int i = 0; texto[i] != '\0'; i++) {
    hash = 31 * hash + (unsigned char)texto[i]; // hash simple tipo Java
  }
  return hash % capacidad;
}

void mostrar_proyectos(TablaHash *tabla) {
  printf("\n--- PROYECTOS DISPONIBLES ---\n");

  // Array auxiliar para saber qué problemáticas ya se imprimieron
  char *problematica_impresa[tabla->capacidad * 2]; // suficiente tamaño
  int num_problematica = 0;

  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *actual = tabla->tabla[i];
    while (actual != NULL) {

      // Revisar si la problemática ya fue impresa
      int ya_impreso = 0;
      for (int k = 0; k < num_problematica; k++) {
        if (strcmp(problematica_impresa[k], actual->problematica) == 0) {
          ya_impreso = 1;
          break;
        }
      }

      if (!ya_impreso) {
        // Imprimir encabezado
        printf("\n[%s]\n", actual->problematica);
        // Recorrer toda la tabla y listar proyectos de esta problemática
        for (int j = 0; j < tabla->capacidad; j++) {
          Proyecto *aux = tabla->tabla[j];
          while (aux != NULL) {
            if (strcmp(aux->problematica, actual->problematica) == 0) {
              printf("• %s\n  Descripción: %s\n  Bibliografía: %s\n\n",
                     aux->nombre, aux->descripcion,
                     "https://ejemplo-bibliografia.org"); // temporal
            }
            aux = aux->sigt;
          }
        }
        // Guardar problemática como ya impresa
        problematica_impresa[num_problematica++] = actual->problematica;
      }

      actual = actual->sigt;
    }
  }
}

void mostrar_estado_pais(Pais *pais) {
  const char *problemas[5] = {"Inundaciones", "Sequias", "Deforestacion",
                              "Perdida de biodiversidad",
                              "Desplazamiento de poblacion"};

  printf("\n=== ESTADO ACTUAL DE %s ===\n", pais->nombre);
  for (int i = 0; i < 5; i++) {
    printf("%s: %d\n", problemas[i], pais->problemas[i]);
  }
  printf("===============================\n");
}
