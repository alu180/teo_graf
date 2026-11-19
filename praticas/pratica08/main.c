#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

static void imprimir_agm(const char *nome, Aresta *agm, int qtd, int total) {
    printf("%s:\n", nome);
    for (int i = 0; i < qtd; i++)
        printf("  %d -- %d  (peso %d)\n", agm[i].u, agm[i].v, agm[i].peso);
    printf("  Peso total: %d\n\n", total);
}

static void testar_grafo(const char *titulo, int n, Aresta *arestas, int m) {
    Aresta *agm_k = (Aresta *)malloc(n * sizeof(Aresta));
    Aresta *agm_p = (Aresta *)malloc(n * sizeof(Aresta));
    int qtd_k, qtd_p;

    printf("=== %s (V=%d, E=%d) ===\n", titulo, n, m);

    int total_k = kruskal(n, arestas, m, agm_k, &qtd_k);
    int total_p = prim(n, arestas, m, 0, agm_p, &qtd_p);

    imprimir_agm("Kruskal", agm_k, qtd_k, total_k);
    imprimir_agm("Prim", agm_p, qtd_p, total_p);

    if (total_k == total_p)
        printf("OK: os dois algoritmos deram o mesmo peso total.\n\n");
    else
        printf("erro: pesos diferentes\n\n");

    free(agm_k);
    free(agm_p);
}

int main(void) {
    /* Grafo 1: 5 vertices (resposta esperada = 16) */
    Aresta g1[] = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8},
        {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    };
    testar_grafo("Grafo 1", 5, g1, 7);

    /* Grafo 2: 4 vertices (resposta esperada = 19) */
    Aresta g2[] = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}
    };
    testar_grafo("Grafo 2", 4, g2, 5);

    /* Grafo 3: 6 vertices (resposta esperada = 14) */
    Aresta g3[] = {
        {0, 1, 4}, {0, 2, 4}, {1, 2, 2}, {2, 3, 3},
        {2, 5, 2}, {2, 4, 4}, {3, 4, 3}, {5, 4, 3}
    };
    testar_grafo("Grafo 3", 6, g3, 8);

    return 0;
}