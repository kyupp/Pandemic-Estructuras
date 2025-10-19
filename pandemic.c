#include "pandemic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----------------------------------------------
//            FUNCIONES DE MAPA
//----------------------------------------------
Pais *crear_pais(char nombre[]) {
  Pais *nuevo = (Pais *)malloc(sizeof(Pais));
  strcpy(nuevo->nombre, nombre);
  nuevo->sigt = NULL;
  nuevo->ant = NULL;
  nuevo->vecinos = NULL;
  nuevo->num_vecinos = 0;
  for (int i = 0; i < 5; i++)
    nuevo->problemas[i] = 0;
  return nuevo;
}

Mapa *crear_mapa() {
  Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));
  mapa->inicio = NULL;
  mapa->final = NULL;
  return mapa;
}

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

void eliminar_pais(Mapa *mapa) {
  Pais *pais = mapa->inicio;

  while (pais) {
    int eliminar =
        1; // Asumimos que debe eliminarse, salvo que se demuestre lo contrario

    // Verificar si TODOS los problemas son >= 3
    for (int i = 0; i < 5; i++) {
      if (pais->problemas[i] < 3) {
        eliminar = 0; // Si alguno es menor a 3, no se elimina
        break;
      }
    }

    // Guardar siguiente antes de eliminar
    Pais *siguiente = pais->sigt;

    // Si debe eliminarse
    if (eliminar) {
      printf(
          "País eliminado: %s (todos sus aspectos alcanzaron nivel 3 o más)\n",
          pais->nombre);

      // Caso 1: primero
      if (pais == mapa->inicio) {
        mapa->inicio = pais->sigt;
        if (mapa->inicio)
          mapa->inicio->ant = NULL;
      }
      // Caso 2: último
      else if (pais == mapa->final) {
        mapa->final = pais->ant;
        if (mapa->final)
          mapa->final->sigt = NULL;
      }
      // Caso 3: intermedio
      else {
        pais->ant->sigt = pais->sigt;
        pais->sigt->ant = pais->ant;
      }

      free(pais);
    }

    // Avanzar al siguiente
    pais = siguiente;
  }
}
Pais *buscar_pais(Mapa *mapa, char nombre[]) {
  Pais *actual = mapa->inicio;
  while (actual != NULL) {
    if (strcmp(actual->nombre, nombre) == 0)
      return actual;
    actual = actual->sigt;
  }
  return NULL;
}

void agregar_vecino(Pais *pais, Pais *vecino) {
  pais->num_vecinos++;
  pais->vecinos = realloc(pais->vecinos, pais->num_vecinos * sizeof(Pais *));
  pais->vecinos[pais->num_vecinos - 1] = vecino;
}

void imprimir_mapa(Mapa *mapa) {
  Pais *actual = mapa->inicio;
  printf("\n--- MAPA DE PAISES Y SUS VECINOS ---\n");
  while (actual != NULL) {
    printf("Pais: %s\nVecinos: ", actual->nombre);
    if (actual->num_vecinos == 0)
      printf("Ninguno");
    else {
      for (int i = 0; i < actual->num_vecinos; i++) {
        printf("%s", actual->vecinos[i]->nombre);
        if (i < actual->num_vecinos - 1)
          printf(", ");
      }
    }
    printf("\n\n");
    actual = actual->sigt;
  }
}

Pais **asignar_problemas(Mapa *mapa) {
  Pais **seleccionados = malloc(9 * sizeof(Pais *));
  int total = 0;
  Pais *actual = mapa->inicio;
  while (actual) {
    total++;
    actual = actual->sigt;
  }

  if (total < 9)
    return NULL;

  int count = 0;
  while (count < 9) {
    int n = rand() % total;
    actual = mapa->inicio;
    for (int i = 0; i < n; i++)
      actual = actual->sigt;

    int repetido = 0;
    for (int j = 0; j < count; j++)
      if (seleccionados[j] == actual)
        repetido = 1;

    if (!repetido)
      seleccionados[count++] = actual;
  }

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 5; j++)
      seleccionados[i]->problemas[j] = rand() % 4;

  return seleccionados;
}

void imprimir_problemas_seleccionados(Pais **seleccionados, int n) {
  const char *nombres[] = {"Inundaciones", "Sequias", "Deforestacion",
                           "Perdida de biodiversidad",
                           "Desplazamiento de poblacion"};
  printf("\n--- PAISES SELECCIONADOS CON PROBLEMAS ---\n\n");
  for (int i = 0; i < n; i++) {
    printf("Pais: %s\n", seleccionados[i]->nombre);
    for (int j = 0; j < 5; j++)
      printf("%s: %d%s", nombres[j], seleccionados[i]->problemas[j],
             j < 4 ? ", " : "\n\n");
  }
}

int cantidad_paises(Mapa *mapa) {
  int c = 0;
  for (Pais *p = mapa->inicio; p; p = p->sigt)
    c++;
  return c;
}

//----------------------------------------------
//            FUNCIONES DE JUGADOR
//----------------------------------------------
Jugador *crear_jugador(char *nombre, Pais *paisActual) {
  Jugador *nuevo = malloc(sizeof(Jugador));
  strcpy(nuevo->nombre, nombre);
  nuevo->actual = paisActual;
  return nuevo;
}

void mover_jugador(Jugador *jugador) {
  Pais *actual = jugador->actual;
  printf("\nJugador %s se encuentra en %s\n", jugador->nombre, actual->nombre);
  if (actual->ant)
    printf("1. Moverse a %s\n", actual->ant->nombre);
  if (actual->sigt)
    printf("2. Moverse a %s\n", actual->sigt->nombre);

  int opcion = 0;
  printf("Seleccione opcion: ");
  scanf("%d", &opcion);
  if (opcion == 1 && actual->ant)
    jugador->actual = actual->ant;
  else if (opcion == 2 && actual->sigt)
    jugador->actual = actual->sigt;
  else
    printf("Opcion invalida.\n");
}

//----------------------------------------------
//            FUNCIONES DE PROYECTO
//----------------------------------------------
Proyecto *crear_proyecto(char *nombre, char *descripcion, TipoAccion tipo,
                         char *paisesAplicados, char *problematica,
                         char *bibliografia) {
  Proyecto *p = malloc(sizeof(Proyecto));
  if (!p)
    return NULL;

  strncpy(p->nombre, nombre, sizeof(p->nombre) - 1);
  strncpy(p->descripcion, descripcion, sizeof(p->descripcion) - 1);
  strncpy(p->paisesAplicados, paisesAplicados, sizeof(p->paisesAplicados) - 1);
  strncpy(p->problematica, problematica, sizeof(p->problematica) - 1);
  strncpy(p->bibliografia, bibliografia, sizeof(p->bibliografia) - 1);

  p->nombre[sizeof(p->nombre) - 1] = '\0';
  p->descripcion[sizeof(p->descripcion) - 1] = '\0';
  p->paisesAplicados[sizeof(p->paisesAplicados) - 1] = '\0';
  p->problematica[sizeof(p->problematica) - 1] = '\0';
  p->bibliografia[sizeof(p->bibliografia) - 1] = '\0';
  p->tipo = tipo;
  p->sigt = NULL;

  return p;
}

//----------------------------------------------
//            FUNCIONES HASH
//----------------------------------------------
unsigned int hash(char *texto, int capacidad) {
  unsigned int h = 0;
  for (int i = 0; texto[i] != '\0'; i++)
    h = 31 * h + (unsigned char)texto[i];
  return h % capacidad;
}

TablaHash *crear_tabla(int capacidad) {
  TablaHash *tabla = malloc(sizeof(TablaHash));
  tabla->capacidad = capacidad;
  tabla->cantidad = 0;
  tabla->tabla = calloc(capacidad, sizeof(Proyecto *));
  return tabla;
}

void redimensionar_tabla(TablaHash *tabla) {
  int nueva_cap = tabla->capacidad * 2;
  Proyecto **nuevo = calloc(nueva_cap, sizeof(Proyecto *));
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *actual = tabla->tabla[i];
    while (actual) {
      Proyecto *sig = actual->sigt;
      unsigned int indice = hash(actual->problematica, nueva_cap);
      actual->sigt = nuevo[indice];
      nuevo[indice] = actual;
      actual = sig;
    }
  }
  free(tabla->tabla);
  tabla->tabla = nuevo;
  tabla->capacidad = nueva_cap;
}

void insertar_proyecto_texto(TablaHash *tabla, char *clave,
                             Proyecto *proyecto) {
  if ((float)tabla->cantidad / tabla->capacidad > 0.7)
    redimensionar_tabla(tabla);
  unsigned int i = hash(clave, tabla->capacidad);
  proyecto->sigt = tabla->tabla[i];
  tabla->tabla[i] = proyecto;
  tabla->cantidad++;
}

Proyecto *buscar_proyecto(TablaHash *tabla, char *nombreProyecto) {
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *actual = tabla->tabla[i];
    while (actual) {
      if (strcmp(actual->nombre, nombreProyecto) == 0)
        return actual;
      actual = actual->sigt;
    }
  }
  return NULL;
}

void imprimir_tabla_hash(TablaHash *tabla) {
  printf("\n--- PROYECTOS REGISTRADOS ---\n");
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *p = tabla->tabla[i];
    if (p) {
      printf("\n[Indice %d]\n", i);
      while (p) {
        printf("Problematica: %s\nNombre: %s\nDescripcion: %s\nPaises: %s\n",
               p->problematica, p->nombre, p->descripcion, p->paisesAplicados);
        printf("Bibliografia: %s\n\n", p->bibliografia);
        p = p->sigt;
      }
    }
  }
}

void mostrar_proyectos(TablaHash *tabla) {
  printf("\n--- PROYECTOS DISPONIBLES ---\n");
  for (int i = 0; i < tabla->capacidad; i++) {
    Proyecto *p = tabla->tabla[i];
    while (p) {
      printf("\n[%s]\n- %s\n  Descripcion: %s\n  Paises: %s\n  Fuente: %s\n",
             p->problematica, p->nombre, p->descripcion, p->paisesAplicados,
             p->bibliografia);
      p = p->sigt;
    }
  }
}

//----------------------------------------------
//            FUNCIONES DE JUEGO
//----------------------------------------------
void mostrar_estado_pais(Pais *pais) {
  const char *nombres[] = {"Inundaciones", "Sequias", "Deforestacion",
                           "Perdida de biodiversidad",
                           "Desplazamiento de poblacion"};
  printf("\n=== ESTADO DE %s ===\n", pais->nombre);
  for (int i = 0; i < 5; i++)
    printf("%s: %d\n", nombres[i], pais->problemas[i]);
  printf("=====================\n");
}

void aplicar_proyecto(Jugador *jugador, Proyecto *proyecto) {
  Pais *pais = jugador->actual;
  printf("\nAplicando proyecto '%s' en %s...\n", proyecto->nombre,
         pais->nombre);
  for (int i = 0; i < 5; i++)
    if (pais->problemas[i] > 0)
      pais->problemas[i]--;
  if (!strstr(proyecto->paisesAplicados, pais->nombre)) {
    if (strlen(proyecto->paisesAplicados) > 0) {
      strncat(proyecto->paisesAplicados, ", ",
              sizeof(proyecto->paisesAplicados) -
                  strlen(proyecto->paisesAplicados) - 1);
    }
    strncat(proyecto->paisesAplicados, pais->nombre,
            sizeof(proyecto->paisesAplicados) -
                strlen(proyecto->paisesAplicados) - 1);
  }

  printf("Proyecto aplicado con exito.\n");
  mostrar_estado_pais(pais);
}

void aumentar_problemas(Mapa *mapa) {
  Pais *actual;
  int total = 0;

  actual = mapa->inicio;
  while (actual) {
    total++;
    actual = actual->sigt;
  }

  if (total < 3)
    return;

  const char *nombresProblemas[5] = {"Inundaciones", "Sequias", "Deforestacion",
                                     "Perdida de biodiversidad",
                                     "Desplazamiento de poblacion"};

  for (int i = 0; i < 3; i++) {
    int n = rand() % total;
    actual = mapa->inicio;
    for (int j = 0; j < n; j++)
      actual = actual->sigt;

    int tipo = rand() % 5;

    if (actual->problemas[tipo] < 3) {
      actual->problemas[tipo]++;
      printf("\nProblema %s aumentado en %s\n", nombresProblemas[tipo],
             actual->nombre);
    } else {
      for (int v = 0; v < actual->num_vecinos; v++) {
        if (actual->vecinos[v]->problemas[tipo] < 3) {
          actual->vecinos[v]->problemas[tipo]++;
          printf("\nProblema %s aumentado en %s (vecino de %s)\n",
                 nombresProblemas[tipo], actual->vecinos[v]->nombre,
                 actual->nombre);
        }
      }
    }
  }
}

void turno_jugador(Jugador *jugador, TablaHash *tabla, Mapa *mapa) {
  int acciones = 4;
  while (acciones > 0) {
    printf("\nTurno de %s (%s) : Quedan %d acciones.\n", jugador->nombre,
           jugador->actual->nombre, acciones);
    printf("1. Moverse\n2. Consultar pais\n3. Aplicar proyecto\nOpcion: ");
    int op;
    scanf("%d", &op);
    if (op == 1) {
      mover_jugador(jugador);
      acciones--;
    } else if (op == 2) {
      mostrar_estado_pais(jugador->actual);
    } else if (op == 3) {
      mostrar_proyectos(tabla);
      char nombreP[100];
      printf("Ingrese el nombre del proyecto: ");
      scanf(" %[^\n]", nombreP);
      Proyecto *p = buscar_proyecto(tabla, nombreP);
      if (p)
        aplicar_proyecto(jugador, p);
      else
        printf("Proyecto no encontrado.\n");
      acciones--;
    }
  }
}

int ganar(Mapa *mapa) {
  int problematicaActual = 0;
  int cantidadDePaises = cantidad_paises(mapa);
  for (int i = 0; i < 5; i++) {
    problematicaActual = 0;
    for (Pais *p = mapa->inicio; p; p = p->sigt) {
      if (p->problemas[i] <= 0)
        problematicaActual++;
    }
    if (problematicaActual >= cantidadDePaises) {
      printf(
          "\n¡Ganaste! Se erradico un aspecto de la problematica en todos los "
          "paises de la region.\n");
      return 1;
    }
  }
  return 0;
}

int perder(Mapa *mapa) {
  if (cantidad_paises(mapa) <= 3) {
    printf("\nPerdiste. Quedan menos de 3 paises.\n");
    return 1;
  }
  return 0;
}
