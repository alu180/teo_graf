#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

GrafoMatriz* criar_grafo_matriz(int n) {
    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    g->n = n;

    // aloca um vetor de n ponteiros (uma linha para cada vertice)
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        // calloc ja inicializa tudo com 0 (sem aresta)
        g->adj[i] = calloc(n, sizeof(int));
    }
    return g;
}

void inserir_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1; // grafo nao-direcionado: a aresta vale nos dois sentidos
}

void remover_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
}

int grau_matriz(GrafoMatriz *g, int v) {
    int grau = 0;
    for (int i = 0; i < g->n; i++) {
        grau += g->adj[v][i];
    }
    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return 0;
    return g->adj[u][v];
}

void imprimir_grafo_matriz(GrafoMatriz *g) {
    printf("   ");
    for (int j = 0; j < g->n; j++) printf("%d ", j);
    printf("\n");

    for (int i = 0; i < g->n; i++) {
        printf("%d: ", i);
        for (int j = 0; j < g->n; j++) {
            printf("%d ", g->adj[i][j]);
        }
        printf("\n");
    }
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}