#include <stdio.h>
#include "conectividade.h"
#include "planaridade.h"

static void imprimir_conectividade(const char *nome, GrafoLista *g) {
    printf("== %s ==\n", nome);
    printf("Vertices: %d | Arestas: %d\n", g->numVertices, contar_arestas(g));

    int articulacoes[100];
    int numArt = encontrar_articulacoes(g, articulacoes);
    printf("Pontos de articulacao (%d): ", numArt);
    for (int i = 0; i < numArt; i++) printf("%d ", articulacoes[i]);
    printf("\n");

    Aresta pontes[100];
    int numPontes = detectar_pontes(g, pontes);
    printf("Pontes (%d): ", numPontes);
    for (int i = 0; i < numPontes; i++) printf("(%d-%d) ", pontes[i].u, pontes[i].v);
    printf("\n");
}

static void imprimir_planaridade(const char *nome, GrafoLista *g) {
    printf("== %s ==\n", nome);
    printf("Vertices: %d | Arestas: %d\n", g->numVertices, contar_arestas(g));
    printf("Euler (m <= 3n-6): %s\n", eh_planar_euler(g) ? "ok" : "violado");
    if (g->numVertices <= PLANARIDADE_MAX_N) {
        printf("Contem subdivisao de K5/K3,3: %s\n", testar_subdivisao_k5_k33(g) ? "sim" : "nao");
    }
    printf("Resultado final (eh_planar): %s\n", eh_planar(g) ? "PLANAR" : "NAO-PLANAR");
    printf("\n");
}

int main(void) {
    printf("########## PARTE 1: CONECTIVIDADE ##########\n\n");

    /* Grafo com uma articulacao e uma ponte:
     * duas "pontas" (0-1-2 e 4-5-6) ligadas por uma ponte (2-3) e (3-4),
     * com o vertice 3 como articulacao. */
    GrafoLista *g1 = criar_grafo_lista(7);
    adicionar_aresta(g1, 0, 1);
    adicionar_aresta(g1, 1, 2);
    adicionar_aresta(g1, 0, 2);
    adicionar_aresta(g1, 2, 3); /* ponte */
    adicionar_aresta(g1, 3, 4); /* ponte */
    adicionar_aresta(g1, 4, 5);
    adicionar_aresta(g1, 5, 6);
    adicionar_aresta(g1, 4, 6);
    imprimir_conectividade("Grafo com pontes e articulacoes", g1);
    liberar_grafo_lista(g1);

    printf("\n########## PARTE 2: PLANARIDADE ##########\n\n");

    /* Grafo planar simples: um quadrado (ciclo de 4 vertices) */
    GrafoLista *quadrado = criar_grafo_lista(4);
    adicionar_aresta(quadrado, 0, 1);
    adicionar_aresta(quadrado, 1, 2);
    adicionar_aresta(quadrado, 2, 3);
    adicionar_aresta(quadrado, 3, 0);
    imprimir_planaridade("Ciclo C4 (planar)", quadrado);
    liberar_grafo_lista(quadrado);

    /* K5: grafo completo com 5 vertices -> NAO planar */
    GrafoLista *k5 = criar_grafo_lista(5);
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            adicionar_aresta(k5, i, j);
        }
    }
    imprimir_planaridade("K5 (nao-planar)", k5);
    liberar_grafo_lista(k5);

    /* K3,3: bipartido completo 3+3 -> NAO planar */
    GrafoLista *k33 = criar_grafo_lista(6);
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) {
            adicionar_aresta(k33, i, j);
        }
    }
    imprimir_planaridade("K3,3 (nao-planar)", k33);
    liberar_grafo_lista(k33);

    /* Subdivisao de K3,3: mesmo K3,3, mas cada aresta (0,3) é
     * "esticada" com um vertice extra no meio (vertice 6), de grau 2.
     * O grafo continua nao-planar mesmo satisfazendo m <= 3n-6. */
    GrafoLista *subdiv = criar_grafo_lista(7);
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) {
            if (i == 0 && j == 3) continue; /* essa aresta sera subdividida */
            adicionar_aresta(subdiv, i, j);
        }
    }
    adicionar_aresta(subdiv, 0, 6);
    adicionar_aresta(subdiv, 6, 3);
    imprimir_planaridade("Subdivisao de K3,3 (nao-planar)", subdiv);
    liberar_grafo_lista(subdiv);

    return 0;
}