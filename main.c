#include "pandemic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL)); // inicio aleatorio

  Mapa *mapa = crear_mapa();

  // agregar países
  char *paises[] = {"Mexico",      "Belice",    "Guatemala",  "Honduras",
                    "El Salvador", "Nicaragua", "Costa Rica", "Panama",
                    "Colombia",    "Venezuela", "Guyana",     "Surinam",
                    "Ecuador",     "Peru",      "Bolivia",    "Paraguay",
                    "Chile",       "Argentina", "Uruguay",    "Brasil"};
  int num_paises = sizeof(paises) / sizeof(paises[0]);

  for (int i = 0; i < num_paises; i++)
    agregar_pais(mapa, paises[i]);

  // fronteras
  typedef struct {
    char *pais;
    char *vecinos[10]; // maximo 10 vecinos por país
  } Fronteras;

  Fronteras fronteras[] = {
      {"Mexico", {"Belice", "Guatemala", NULL}},
      {"Belice", {"Mexico", "Guatemala", NULL}},
      {"Guatemala", {"Mexico", "Belice", "El Salvador", "Honduras", NULL}},
      {"Honduras", {"Guatemala", "El Salvador", "Nicaragua", NULL}},
      {"El Salvador", {"Guatemala", "Honduras", NULL}},
      {"Nicaragua", {"Honduras", "Costa Rica", NULL}},
      {"Costa Rica", {"Nicaragua", "Panama", NULL}},
      {"Panama", {"Costa Rica", "Colombia", NULL}},
      {"Colombia", {"Panama", "Venezuela", "Ecuador", NULL}},
      {"Venezuela", {"Colombia", "Guyana", NULL}},
      {"Guyana", {"Venezuela", "Surinam", "Brasil", NULL}},
      {"Surinam", {"Guyana", "Brasil", NULL}},
      {"Ecuador", {"Colombia", "Peru", NULL}},
      {"Peru", {"Ecuador", "Bolivia", "Chile", "Brasil", "Colombia", NULL}},
      {"Bolivia", {"Brasil", "Paraguay", "Argentina", "Chile", "Peru", NULL}},
      {"Chile", {"Peru", "Bolivia", "Argentina", NULL}},
      {"Argentina",
       {"Chile", "Bolivia", "Paraguay", "Brasil", "Uruguay", NULL}},
      {"Uruguay", {"Argentina", "Brasil", NULL}},
      {"Paraguay", {"Bolivia", "Argentina", "Brasil", NULL}},
      {"Brasil",
       {"Uruguay", "Argentina", "Paraguay", "Bolivia", "Peru", "Colombia",
        "Venezuela", "Guyana", "Surinam", NULL}}};
  int num_fronteras = sizeof(fronteras) / sizeof(fronteras[0]);

  // asignar vecinos
  for (int i = 0; i < num_fronteras; i++) {
    Pais *pais_actual = buscar_pais(mapa, fronteras[i].pais);
    for (int j = 0; fronteras[i].vecinos[j] != NULL; j++) {
      Pais *vecino = buscar_pais(mapa, fronteras[i].vecinos[j]);
      agregar_vecino(pais_actual, vecino);
    }
  }

  // mostrar todos los países
  imprimir_mapa(mapa);

  // asignar problemas y obtener 9 países aleatorios
  Pais **seleccionados = asignar_problemas(mapa);

  // 9 países seleccionados
  imprimir_problemas_seleccionados(seleccionados, 9);

  // jugadores
  // jugador ONU
  int n = rand() % num_paises;
  Pais *paisONU = mapa->inicio;
  Jugador *jugador_ONU = crear_jugador("ONU", paisONU);

  for (int i = 0; i < n; i++)
    jugador_ONU->actual = jugador_ONU->actual->sigt;

  // jugador Colectivo anarquista
  Pais *paisColectivo = mapa->inicio;
  Jugador *jugador_colectivo = crear_jugador("Colectivo", paisColectivo);
  do {
    int m = rand() % num_paises;
    for (int i = 0; i < m; i++)
      jugador_colectivo->actual = jugador_colectivo->actual->sigt;
  } while (jugador_colectivo->actual == jugador_ONU->actual);

  TablaHash *tabla = crear_tabla(10);

  // Ejemplo de insercion con texto como llave
  insertar_proyecto_texto(
      tabla, "Inundaciones",
      crear_proyecto("Construccion de diques",
                     "Construir diques para evitar desbordamientos en rios "
                     "cercanos a poblaciones.",
                     MEJORAR, "Mexico, Guatemala, Honduras"));

  insertar_proyecto_texto(
      tabla, "Inundaciones",
      crear_proyecto("Reforestacion de cuencas",
                     "Reforestar zonas altas para controlar escorrentia y "
                     "prevenir inundaciones.",
                     MEJORAR, "Costa Rica, Panama"));

  insertar_proyecto_texto(tabla, "Sequias",
                          crear_proyecto("Captacion de agua pluvial",
                                         "Implementar sistemas de recoleccion "
                                         "y almacenamiento de agua de lluvia.",
                                         MEJORAR,
                                         "Nicaragua, El Salvador, Honduras"));

  // printf("%d , %s ,%s", n_proyecto->clave, n_proyecto->descripcion,
  //        n_proyecto->nombre);
  // printf("%d , %s ,%s", n_proyecto2->clave, n_proyecto2->descripcion,
  //        n_proyecto2->nombre);

  // for (int turno = 1; turno <= 10; turno++) {
  //   printf("\n=== TURNO %d ===\n", turno);
  //   turno_jugador(jugador_ONU, tabla, mapa);
  //   turno_jugador(jugador_colectivo, tabla, mapa);
  //   aumentar_problemas(mapa); // se incrementan los problemas en 3 paises al
  //   azar
  // }

  printf("\n=== INICIO DEL JUEGO ===\n");
  for (int turno = 1; turno <= 3;
       turno++) { // podés ajustar el número de turnos
    printf("\n===== TURNO %d =====\n", turno);
    turno_jugador(jugador_ONU, tabla, mapa);
    turno_jugador(jugador_colectivo, tabla, mapa);
    aumentar_problemas(mapa);
  }

  return 0;
}
