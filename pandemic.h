#ifndef PANDEMIC_H
#define PANDEMIC_H

//---------------------------------------------------
//                  Estructuras
//---------------------------------------------------
// Estructura que representa un país
typedef struct Pais {
  char nombre[50];
  struct Pais *sigt;     // Siguiente país en la lista
  struct Pais *ant;      // País anterior en la lista
  struct Pais **vecinos; // Vecinos
  int num_vecinos;       // Cantidad de vecinos
  int problemas[5]; // 0: Inundaciones, 1: Sequias, 2: Deforestacion, 3: Perdida
                    // de biodiversidad, 4: Desplazamiento poblacional
} Pais;

// Estructura que representa el mapa (lista doblemente enlazada)
typedef struct Mapa {
  Pais *inicio; // Primer país del mapa
  Pais *final;  // Último país del mapa
} Mapa;

// Estructura Jugador
typedef struct Jugador {
  char nombre[50];
  struct Pais *actual;
} Jugador;

typedef enum { MEJORAR, EMPEORAR } TipoAccion;

// Estructura proyecto
typedef struct Proyecto {
  char nombre[100];
  char descripcion[300];
  char paisesAplicados[200];
  char problematica[50];
  TipoAccion tipo;
  struct Proyecto *sigt;
} Proyecto;

// Estructura Tabla Hash
typedef struct TablaHash {
  Proyecto **tabla;
  int capacidad;
  int cantidad;
} TablaHash;

//-----------------------------------------------
//            Prototipos
//-----------------------------------------------
// Prototipos Mapa
Pais *crear_pais(char nombre[]);
Mapa *crear_mapa();
void agregar_pais(Mapa *mapa, char nombre[]);
Pais *buscar_pais(Mapa *mapa, char nombre[]);
void agregar_vecino(Pais *pais, Pais *vecino);
void imprimir_mapa(Mapa *mapa);
void imprimir_vecinos(Pais *pais);
Pais **asignar_problemas(Mapa *mapa); // devuelve los 9 países seleccionados
void imprimir_problemas_seleccionados(Pais **seleccionados, int n);
int cantidadPaises(Mapa *mapa);

// Prototipos Jugador
Jugador *crear_jugador(char nombre[], Pais *paisActual);
void mover_jugador(Jugador *jugador);

// Prototipos Proyecto
Proyecto *crear_proyecto(char *nombre, char *descripcion, TipoAccion tipo,
                         char *paisesAplicados);

// Prototipos Hash
TablaHash *crear_tabla(int capacidad);
void redimensionar_tabla(TablaHash *tabla);
void aplicar_proyecto(Jugador *jugador, Proyecto *proyecto);
void imprimir_tabla_hash(TablaHash *tabla);

// Versión de hash con llave de texto
unsigned int hash(char *clave, int capacidad);
void insertar_proyecto_texto(TablaHash *tabla, char *clave, Proyecto *proyecto);
Proyecto *buscar_proyecto(TablaHash *tabla, char *nombreProyecto);

// Funciones de Juego
void turno_jugador(Jugador *jugador, TablaHash *tabla, Mapa *mapa);
void aumentar_problemas(Mapa *mapa);
void mostrar_estado_pais(Pais *pais);
void mostrar_proyectos(TablaHash *tabla);

// Comprobaciones Ganar - Perder
int ganar(Mapa *mapa);
int perder(Mapa *mapa);
void paisPerdido(Mapa *mapa);

#endif
