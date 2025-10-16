#ifndef PANDEMIC_H
#define PANDEMIC_H

// Estructura que representa un país
typedef struct Pais {
    char nombre[50];
    struct Pais *sigt;         // Siguiente país en la lista
    struct Pais *ant;          // País anterior en la lista
    struct Pais **vecinos;  // Vecinos
    int num_vecinos;           // Cantidad de vecinos
     int problemas[3]; // 0: Inundaciones, 1: Sequias, 2: Deforestacion, 3: Perdida de biodiversidad, 4: Desplazamiento poblacional
} Pais;

// Estructura que representa el mapa (lista doblemente enlazada)
typedef struct Mapa {
    Pais *inicio; // Primer país del mapa
    Pais *final; // Último país del mapa
} Mapa;

// Prototipos
Pais *crear_pais(char nombre[]);
Mapa *crear_mapa();
void agregar_pais(Mapa *mapa, char nombre[]);
Pais *buscar_pais(Mapa *mapa, char nombre[]);
void agregar_vecino(Pais *pais, Pais *vecino);
void imprimir_mapa(Mapa *mapa);
void imprimir_vecinos(Pais *pais);
Pais **asignar_problemas(Mapa *mapa); // devuelve los 9 países seleccionados
void imprimir_problemas_seleccionados(Pais **seleccionados, int n);

#endif
