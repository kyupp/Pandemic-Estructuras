// Pandemic.h
#ifndef PANDEMIC_H
#define PANDEMIC_H

// Estructura que representa un país
typedef struct Pais {
  char nombre[50];   // Arreglo de caracteres con tamaño definido
  struct Pais *sigt; // Puntero al siguiente país
  struct Pais *ant;  // Puntero al país anterior
} Pais;

// Estructura que representa el mapa (lista doblemente enlazada)
typedef struct Mapa {
  Pais *inicio; // Primer país del mapa
  Pais *final;  // Último país del mapa
} Mapa;

// Prototipos de funciones
Pais *crear_pais(const char nombre[]);
Mapa *crear_mapa();
void imprimir_mapa(const Mapa *mapa);

#endif
