#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pandemic.h"

int main(){
    srand(time(NULL)); // inicio aleatorio

    Mapa *mapa = crear_mapa();

    // agregar países 
    char *paises[]={
        "Mexico","Belice","Guatemala","Honduras","El Salvador","Nicaragua","Costa Rica",
        "Panama","Colombia","Venezuela","Guyana","Surinam","Ecuador","Peru","Bolivia","Paraguay","Chile",
        "Argentina","Uruguay","Brasil"
    };
    int num_paises = sizeof(paises)/sizeof(paises[0]);

    for(int i=0;i<num_paises;i++)
        agregar_pais(mapa,paises[i]);

    // fronteras
    typedef struct {
        char *pais;
        char *vecinos[10]; // maximo 10 vecinos por país
    } Fronteras;

    Fronteras fronteras[] = {
        {"Mexico", {"Belice","Guatemala", NULL}},
        {"Belice", {"Mexico","Guatemala", NULL}},
        {"Guatemala", {"Mexico","Belice","El Salvador","Honduras", NULL}},
        {"Honduras", {"Guatemala","El Salvador","Nicaragua", NULL}},
        {"El Salvador", {"Guatemala","Honduras", NULL}},
        {"Nicaragua", {"Honduras","Costa Rica", NULL}},
        {"Costa Rica", {"Nicaragua","Panama", NULL}},
        {"Panama", {"Costa Rica","Colombia", NULL}},
        {"Colombia", {"Panama","Venezuela","Ecuador", NULL}},
        {"Venezuela", {"Colombia","Guyana", NULL}},
        {"Guyana", {"Venezuela","Surinam","Brasil", NULL}},
        {"Surinam", {"Guyana","Brasil", NULL}},
        {"Ecuador", {"Colombia","Peru", NULL}},
        {"Peru", {"Ecuador","Bolivia","Chile","Brasil","Colombia", NULL}},
        {"Bolivia", {"Brasil","Paraguay","Argentina","Chile","Peru", NULL}},
        {"Chile", {"Peru","Bolivia","Argentina", NULL}},
        {"Argentina", {"Chile","Bolivia","Paraguay","Brasil","Uruguay", NULL}},
        {"Uruguay", {"Argentina","Brasil", NULL}},
        {"Paraguay", {"Bolivia","Argentina","Brasil", NULL}},
        {"Brasil", {"Uruguay","Argentina","Paraguay","Bolivia","Peru","Colombia","Venezuela","Guyana","Surinam", NULL}}
    };
    int num_fronteras = sizeof(fronteras)/sizeof(fronteras[0]);

    // asignar vecinos
    for(int i=0;i<num_fronteras;i++){
        Pais *pais_actual = buscar_pais(mapa, fronteras[i].pais);
        for(int j=0; fronteras[i].vecinos[j]!=NULL; j++){
            Pais *vecino = buscar_pais(mapa, fronteras[i].vecinos[j]);
            agregar_vecino(pais_actual, vecino);
        }
    }

    // mostrar todos los países 
    imprimir_mapa(mapa);
    
    // asignar problemas y obtener 9 países aleatorios
    Pais **seleccionados = asignar_problemas(mapa);

    // 9 países seleccionados 
    imprimir_problemas_seleccionados(seleccionados,9);

    // jugadores 
    // jugador ONU
    int n = rand() % num_paises;
    Pais *jugador_ONU = mapa->inicio;
    for(int i=0; i<n; i++) jugador_ONU = jugador_ONU->sigt;

    // jugador Colectivo anarquista 
    Pais *jugador_colectivo;
    do {
        int m = rand() % num_paises;
        jugador_colectivo = mapa->inicio;
        for(int i=0; i<m; i++) jugador_colectivo = jugador_colectivo->sigt;
    } while(jugador_colectivo == jugador_ONU);

    // donde están los jugadores
    printf("\n--- UBICACION DE LOS JUGADORES ---\n");
    printf("ONU esta en: %s\n", jugador_ONU->nombre);
    printf("Colectivo anarquista esta en: %s\n", jugador_colectivo->nombre);

    return 0;
}
