#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

int main(void) {
    /* ---------------------------------------------------------------
     * Teste 1: coloracao gulosa x Welsh-Powell num grafo com graus
     * bem diferentes entre si.
     * ------------------------------------------------------------- */
    printf("=== Teste 1: coloracao (gulosa x Welsh-Powell) ===\n");

    GrafoLista *g1 = criar_grafo(6);
    adicionar_aresta(g1, 0, 3);
    adicionar_aresta(g1, 0, 4);
    adicionar_aresta(g1, 1, 3);
    adicionar_aresta(g1, 1, 4);
    adicionar_aresta(g1, 1, 5);
    adicionar_aresta(g1, 2, 4);
    adicionar_aresta(g1, 2, 5);
    adicionar_aresta(g1, 3, 4);
    adicionar_aresta(g1, 4, 5);

    int num_cores_gulosa;
    int *cores_gulosa = coloracao_gulosa(g1, &num_cores_gulosa);
    printf("\n-- Coloracao gulosa (ordem 0,1,2,...) --\n");
    imprimir_cores(cores_gulosa, g1->num_vertices);
    printf("Total de cores usadas: %d\n", num_cores_gulosa);

    int num_cores_wp;
    int *cores_wp = coloracao_welsh_powell(g1, &num_cores_wp);
    printf("\n-- Coloracao Welsh-Powell (ordem por grau decrescente) --\n");
    imprimir_cores(cores_wp, g1->num_vertices);
    printf("Total de cores usadas: %d\n", num_cores_wp);

    free(cores_gulosa);
    free(cores_wp);
    liberar_grafo(g1);

    /* ---------------------------------------------------------------
     * Teste 2: grafo bipartido (ciclo par C4: 0-1-2-3-0)
     * ------------------------------------------------------------- */
    printf("\n=== Teste 2: grafo bipartido (ciclo par C4) ===\n");
    GrafoLista *g2 = criar_grafo(4);
    adicionar_aresta(g2, 0, 1);
    adicionar_aresta(g2, 1, 2);
    adicionar_aresta(g2, 2, 3);
    adicionar_aresta(g2, 3, 0);

    printf("E bipartido? %s\n", eh_bipartido(g2) ? "Sim" : "Nao");
    liberar_grafo(g2);

    /* ---------------------------------------------------------------
     * Teste 3: grafo NAO bipartido (ciclo impar C5: 0-1-2-3-4-0)
     * ------------------------------------------------------------- */
    printf("\n=== Teste 3: grafo nao-bipartido (ciclo impar C5) ===\n");
    GrafoLista *g3 = criar_grafo(5);
    adicionar_aresta(g3, 0, 1);
    adicionar_aresta(g3, 1, 2);
    adicionar_aresta(g3, 2, 3);
    adicionar_aresta(g3, 3, 4);
    adicionar_aresta(g3, 4, 0);

    printf("E bipartido? %s\n", eh_bipartido(g3) ? "Sim" : "Nao");
    liberar_grafo(g3);

    return 0;
}