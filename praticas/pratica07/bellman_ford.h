#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define INFINITO INT_MAX
#define SEM_PRED -1

/* Uma aresta da lista de adjacência: u -> destino com um peso */
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *prox;
} Aresta;

/* Grafo direcionado ponderado com lista de adjacência */
typedef struct {
    int n;          /* número de vértices (0 .. n-1) */
    Aresta **adj;   /* adj[u] = lista de arestas que saem de u */
} GrafoListaPonderado;

/* Funções auxiliares do grafo */
GrafoListaPonderado *criar_grafo(int n);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

/* Retorna 1 se deu certo, 0 se existe ciclo negativo alcançável */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Encontra e imprime um ciclo negativo (usar quando bellman_ford retornar 0) */
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred);

/* Imprime o caminho da origem até destino usando pred[] */
void imprimir_caminho(int *pred, int destino);

#endif