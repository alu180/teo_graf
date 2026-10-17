#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void executar(GrafoListaPonderado *g, int origem) {
    int *dist = malloc(g->n * sizeof(int));
    int *pred = malloc(g->n * sizeof(int));

    if (bellman_ford(g, origem, dist, pred)) {
        printf("Sem ciclo negativo. Distancias a partir de %d:\n", origem);
        for (int v = 0; v < g->n; v++) {
            if (dist[v] == INFINITO) {
                printf("  %d: INF (inalcancavel)\n", v);
            } else {
                printf("  %d: %3d  | caminho: ", v, dist[v]);
                imprimir_caminho(pred, v);
                printf("\n");
            }
        }
    } else {
        printf("Ciclo negativo detectado!\n");
        reconstruir_ciclo_negativo(g, dist, pred);
    }

    free(dist);
    free(pred);
}

int main(void) {
    /* Teste 1: pesos negativos, mas sem ciclo negativo */
    printf("=== Teste 1 ===\n");
    GrafoListaPonderado *g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1, 6);
    adicionar_aresta(g1, 0, 3, 7);
    adicionar_aresta(g1, 1, 2, 5);
    adicionar_aresta(g1, 1, 3, 8);
    adicionar_aresta(g1, 1, 4, -4);
    adicionar_aresta(g1, 2, 1, -2);
    adicionar_aresta(g1, 3, 2, -3);
    adicionar_aresta(g1, 3, 4, 9);
    adicionar_aresta(g1, 4, 0, 2);
    adicionar_aresta(g1, 4, 2, 7);
    executar(g1, 0);
    liberar_grafo(g1);

    /* Teste 2: COM ciclo negativo 1 -> 2 -> 3 -> 1 (custo -3) */
    printf("\n=== Teste 2 ===\n");
    GrafoListaPonderado *g2 = criar_grafo(4);
    adicionar_aresta(g2, 0, 1, 1);
    adicionar_aresta(g2, 1, 2, -1);
    adicionar_aresta(g2, 2, 3, -1);
    adicionar_aresta(g2, 3, 1, -1);
    executar(g2, 0);
    liberar_grafo(g2);

    return 0;
}