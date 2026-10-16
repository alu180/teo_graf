#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

/* Valor usado para representar "distancia infinita" (vertice ainda nao alcancado) */
#define INFINITO INT_MAX

/* ======================= GRAFO PONDERADO ======================= */

/* Uma aresta na lista de adjacencia: "de u eu vou para destino gastando peso" */
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *prox;
} Aresta;

/* Grafo direcionado e ponderado representado por listas de adjacencia */
typedef struct {
    int num_vertices;
    Aresta **adj; /* adj[u] = primeira aresta que sai de u */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso);
void imprimir_grafo(GrafoListaPonderado *g);
void liberar_grafo(GrafoListaPonderado *g);

/* ======================= MIN-HEAP ======================= */

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vertice -> indice no heap (-1 se nao esta no heap) */
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *h);
int heap_vazio(MinHeap *h);
int esta_no_heap(MinHeap *h, int vertice);
void inserir(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);

/* ======================= DIJKSTRA ======================= */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif