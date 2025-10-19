#include "pandemic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));

  printf("=====================================\n");
  printf("      BIENVENIDOS A PANDEMIC\n");
  printf("=====================================\n\n");

  printf("INSTRUCCIONES DEL JUEGO:\n");
  printf("1. Cada jugador tiene 4 acciones por turno.\n");
  printf("2. Puedes moverte entre paises, consultar el estado del pais o "
         "aplicar proyectos.\n");
  printf("3. Al final de cada turno, se incrementara en 1 el valor de un "
         "problema en 3 paises seleccionados al azar.\n");
  printf("4. Ningun problema puede superar el valor 3, si se alcanza, se "
         "propaga a vecinos.\n");
  printf("5. Los proyectos reducen los problemas aplicados en el pais "
         "correspondiente.\n");
  printf("6. Se gana si se erradica un aspecto de la problematica en todos los "
         "paises de la region.\n");
  printf("7. Se pierde si quedan 3 paises o menos con posibilidades de salir "
         "del hueco.\n\n");
  printf(
      "UNA VEZ LEIDAS LAS INSTRUCCIONES, PULSA ENTER PARA EMPEZAR A JUGAR...");
  getchar(); // Espera a que el usuario pulse Enter

  // Crear mapa
  Mapa *mapa = crear_mapa();

  // Lista de países
  char *paises[] = {"Mexico",      "Belice",    "Guatemala",  "Honduras",
                    "El Salvador", "Nicaragua", "Costa Rica", "Panama",
                    "Colombia",    "Venezuela", "Guyana",     "Surinam",
                    "Ecuador",     "Peru",      "Bolivia",    "Paraguay",
                    "Chile",       "Argentina", "Uruguay",    "Brasil"};
  int num_paises = sizeof(paises) / sizeof(paises[0]);

  for (int i = 0; i < num_paises; i++)
    agregar_pais(mapa, paises[i]);

  // Fronteras (vecinos)
  typedef struct {
    char *pais;
    char *vecinos[10];
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
      {"Peru", {"Ecuador", "Bolivia", "Chile", "Brasil", NULL}},
      {"Bolivia", {"Peru", "Chile", "Paraguay", "Brasil", "Argentina", NULL}},
      {"Chile", {"Peru", "Bolivia", "Argentina", NULL}},
      {"Argentina",
       {"Chile", "Bolivia", "Paraguay", "Uruguay", "Brasil", NULL}},
      {"Uruguay", {"Argentina", "Brasil", NULL}},
      {"Paraguay", {"Bolivia", "Argentina", "Brasil", NULL}},
      {"Brasil",
       {"Uruguay", "Argentina", "Paraguay", "Bolivia", "Peru", "Colombia",
        "Venezuela", "Guyana", "Surinam", NULL}}};

  int num_fronteras = sizeof(fronteras) / sizeof(fronteras[0]);

  for (int i = 0; i < num_fronteras; i++) {
    Pais *p = buscar_pais(mapa, fronteras[i].pais);
    for (int j = 0; fronteras[i].vecinos[j] != NULL; j++) {
      Pais *v = buscar_pais(mapa, fronteras[i].vecinos[j]);
      agregar_vecino(p, v);
    }
  }

  imprimir_mapa(mapa);

  // Asignar problemas iniciales
  Pais **seleccionados = asignar_problemas(mapa);
  imprimir_problemas_seleccionados(seleccionados, 9);

  // Crear jugadores
  int n = rand() % num_paises;
  Pais *paisONU = mapa->inicio;
  for (int i = 0; i < n; i++)
    paisONU = paisONU->sigt;

  Jugador *ONU = crear_jugador("ONU", paisONU);

  int m = rand() % num_paises;
  Pais *paisColectivo = mapa->inicio;
  for (int i = 0; i < m; i++)
    paisColectivo = paisColectivo->sigt;

  while (paisColectivo == paisONU)
    paisColectivo = paisColectivo->sigt;

  Jugador *Colectivo = crear_jugador("Colectivo", paisColectivo);

  //-----------------------------------------------------
  //              TABLA HASH DE PROYECTOS
  //-----------------------------------------------------
  TablaHash *tabla = crear_tabla(10);

  // INUNDACIONES
  insertar_proyecto_texto(
      tabla, "Inundaciones",
      crear_proyecto("Sistemas de alerta temprana",
                     "Fortalecer los sistemas de alerta y desarrollar planes "
                     "de accion temprana para reducir el riesgo de desastres.",
                     MEJORAR, "", "Inundaciones",
                     "https://www.acnur.org/noticias/historias/"
                     "inundaciones-causadas-por-la-crisis-climatica-provocan-"
                     "nuevos-desplazamientos"));

  insertar_proyecto_texto(
      tabla, "Inundaciones",
      crear_proyecto("Inclusion de personas desplazadas",
                     "Incluir a personas refugiadas y desplazadas en medidas "
                     "de preparacion y resiliencia ante inundaciones.",
                     MEJORAR, "", "Inundaciones",
                     "https://www.acnur.org/noticias/historias/"
                     "inundaciones-causadas-por-la-crisis-climatica-provocan-"
                     "nuevos-desplazamientos"));

  insertar_proyecto_texto(
      tabla, "Inundaciones",
      crear_proyecto("Asistencia humanitaria",
                     "Suministrar articulos esenciales y ayudar a evacuar "
                     "familias afectadas por inundaciones.",
                     MEJORAR, "", "Inundaciones",
                     "https://www.acnur.org/noticias/historias/"
                     "inundaciones-causadas-por-la-crisis-climatica-provocan-"
                     "nuevos-desplazamientos"));

  // SEQUIAS
  insertar_proyecto_texto(
      tabla, "Sequias",
      crear_proyecto("Monitoreo climatic o",
                     "Reforzar el monitoreo de amenazas climaticas y "
                     "fortalecer sistemas de alerta temprana.",
                     MEJORAR, "", "Sequias",
                     "https://www.wwfca.org/nuestrotrabajo/clima_energia/"
                     "impacto_cambio_climatico_latinoamerica/"));

  insertar_proyecto_texto(
      tabla, "Sequias",
      crear_proyecto("Indice ASIS",
                     "Implementar el Indice de Estres Agricola (ASIS) para "
                     "emitir alertas tempranas en agricultura.",
                     MEJORAR, "", "Sequias",
                     "https://www.wwfca.org/nuestrotrabajo/clima_energia/"
                     "impacto_cambio_climatico_latinoamerica/"));

  insertar_proyecto_texto(
      tabla, "Sequias",
      crear_proyecto("Monitoreo urbano de agua",
                     "Reforzar el monitoreo de vulnerabilidad de sistemas "
                     "urbanos sin planes de contingencia.",
                     MEJORAR, "", "Sequias",
                     "https://www.wwfca.org/nuestrotrabajo/clima_energia/"
                     "impacto_cambio_climatico_latinoamerica/"));

  // DEFORESTACION
  insertar_proyecto_texto(
      tabla, "Deforestacion",
      crear_proyecto(
          "Proteccion de manglares",
          "Promover la proteccion de los manglares que almacenan mas carbono "
          "que la mayoria de bosques.",
          MEJORAR, "", "Deforestacion",
          "https://eos.com/es/blog/efecto-invernadero-y-deforestacion/"));

  insertar_proyecto_texto(
      tabla, "Deforestacion",
      crear_proyecto(
          "Reversion de la tala",
          "Implementar politicas para revertir la deforestacion causada por la "
          "tala para pastizales.",
          MEJORAR, "", "Deforestacion",
          "https://eos.com/es/blog/efecto-invernadero-y-deforestacion/"));

  insertar_proyecto_texto(
      tabla, "Deforestacion",
      crear_proyecto(
          "Uso sostenible del suelo",
          "Evitar cambios en el uso de la tierra que debilitan la capacidad "
          "natural de absorber lluvias.",
          MEJORAR, "", "Deforestacion",
          "https://eos.com/es/blog/efecto-invernadero-y-deforestacion/"));

  // PERDIDA DE BIODIVERSIDAD
  insertar_proyecto_texto(
      tabla, "Perdida de biodiversidad",
      crear_proyecto("Proteccion de zonas verdes",
                     "Proteger zonas verdes como parte de la accion climatica "
                     "para conservar la biodiversidad.",
                     MEJORAR, "", "Perdida de biodiversidad",
                     "https://defenders.org/es/blog/2022/12/"
                     "factores-de-perdida-de-biodiversidad-cambio-climatico"));

  insertar_proyecto_texto(
      tabla, "Perdida de biodiversidad",
      crear_proyecto(
          "Transformacion socioeconomica",
          "Adoptar transformaciones radicales en sistemas socioeconomicos para "
          "enfrentar la perdida de biodiversidad.",
          MEJORAR, "", "Perdida de biodiversidad",
          "https://defenders.org/es/blog/2022/12/"
          "factores-de-perdida-de-biodiversidad-cambio-climatico"));

  insertar_proyecto_texto(
      tabla, "Perdida de biodiversidad",
      crear_proyecto("Integracion regional",
                     "Priorizar la integracion regional para atenuar los "
                     "efectos del cambio climatico en ecosistemas.",
                     MEJORAR, "", "Perdida de biodiversidad",
                     "https://defenders.org/es/blog/2022/12/"
                     "factores-de-perdida-de-biodiversidad-cambio-climatico"));

  // DESPLAZAMIENTO DE POBLACION
  insertar_proyecto_texto(
      tabla, "Desplazamiento de poblacion",
      crear_proyecto(
          "Resiliencia comunitaria",
          "Impulsar iniciativas de resiliencia climatica para comunidades "
          "vulnerables.",
          MEJORAR, "", "Desplazamiento de poblacion",
          "https://www.acnur.org/que-hacemos/construir-un-futuro-mejor/"
          "desplazamiento-y-cambio-climatico"));

  insertar_proyecto_texto(
      tabla, "Desplazamiento de poblacion",
      crear_proyecto(
          "Energia sostenible para desplazados",
          "Proporcionar recursos sostenibles y energia limpia a personas "
          "refugiadas y desplazadas.",
          MEJORAR, "", "Desplazamiento de poblacion",
          "https://www.acnur.org/que-hacemos/construir-un-futuro-mejor/"
          "desplazamiento-y-cambio-climatico"));

  insertar_proyecto_texto(
      tabla, "Desplazamiento de poblacion",
      crear_proyecto(
          "Preparacion ante emergencias",
          "Mejorar la preparacion ante emergencias y rehabilitar alojamientos "
          "tras inundaciones.",
          MEJORAR, "", "Desplazamiento de poblacion",
          "https://www.acnur.org/que-hacemos/construir-un-futuro-mejor/"
          "desplazamiento-y-cambio-climatico"));

  //-----------------------------------------------------
  //                   INICIO DEL JUEGO
  //-----------------------------------------------------
  printf("\n=== INICIO DEL JUEGO ===\n");

  int turno = 0;
  while (!ganar(mapa) && !perder(mapa)) {
    printf("\n===== TURNO %d =====\n", turno);
    turno_jugador(ONU, tabla, mapa);
    turno_jugador(Colectivo, tabla, mapa);
    eliminar_pais(mapa);
    aumentar_problemas(mapa);
    turno++;
  }

  return 0;
}
