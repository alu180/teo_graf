#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

int main(void) {
    /*
     * 6 locais de uma cidade e o tempo
     * para ir de um ao outro. As ruas sao de mao unica (grafo direcionado).
     *
     *   0 = Casa       1 = Padaria     2 = Mercado
     *   3 = Faculdade  4 = Academia    5 = Praia (sem rua chegando la)
     */
    int n = 6;
    GrafoListaPonderado *g = criar_grafo(n);

    adicionar_aresta(g, 0, 1, 4); /* Casa      -> Padaria   : 4 min */
    adicionar_aresta(g, 0, 2, 1); /* Casa      -> Mercado   : 1 min */
    adicionar_aresta(g, 2, 1, 2); /* Mercado   -> Padaria   : 2 min */
    adicionar_aresta(g, 1, 3, 1); /* Padaria   -> Faculdade : 1 min */
    adicionar_aresta(g, 2, 3, 5); /* Mercado   -> Faculdade : 5 min */
    adicionar_aresta(g, 3, 4, 3); /* Faculdade -> Academia  : 3 min */

    printf("Grafo (lista de adjacencia):\n");
    imprimir_grafo(g);

    int origem = 0;
    int *dist = malloc(n * sizeof(int));
    int *pred = malloc(n * sizeof(int));

    dijkstra(g, origem, dist, pred);

    printf("\nMenores distancias a partir do vertice %d:\n", origem);
    for (int v = 0; v < n; v++) {
        if (dist[v] == INFINITO)
            printf("  %d: inalcancavel\n", v);
        else
            printf("  %d: %d\n", v, dist[v]);
    }

    printf("\nCaminhos minimos a partir do vertice %d:\n", origem);
    for (int v = 0; v < n; v++) {
        printf("  ate %d: ", v);
        reconstruir_caminho(origem, v, pred);
    }

    free(dist);
    free(pred);
    liberar_grafo(g);
    return 0;
}