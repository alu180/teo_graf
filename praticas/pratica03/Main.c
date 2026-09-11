#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

static void imprimir_ordenacao(const char *nome, int *ordem, int tamanho) {
    if (ordem == NULL) {
        printf("%s: IMPOSSIVEL (o grafo possui um ciclo)\n", nome);
        return;
    }

    printf("%s: ", nome);
    for (int i = 0; i < tamanho; i++) {
        printf("%d", ordem[i]);
        if (i < tamanho - 1) printf(" -> ");
    }
    printf("\n");
}

int main(void) {
    int n, m;

    printf("Numero de vertices: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Numero de vertices invalido.\n");
        return 1;
    }

    printf("Numero de arestas: ");
    if (scanf("%d", &m) != 1 || m < 0) {
        fprintf(stderr, "Numero de arestas invalido.\n");
        return 1;
    }

    GrafoLista *g = criar_grafo(n);

    printf("Digite as %d arestas no formato 'origem destino' (vertices de 0 a %d):\n", m, n - 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 0 || u >= n || v < 0 || v >= n) {
            fprintf(stderr, "Aresta invalida.\n");
            liberar_grafo(g);
            return 1;
        }
        adicionar_aresta(g, u, v);
    }

    printf("\n");
    if (eh_dag(g)) {
        printf("O grafo EH um DAG (aciclico).\n\n");
    } else {
        printf("O grafo NAO eh um DAG (possui ciclo).\n\n");
    }

    int tamanho_kahn;
    int *ordem_kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);
    imprimir_ordenacao("Kahn (BFS)", ordem_kahn, tamanho_kahn);
    free(ordem_kahn);

    int tamanho_dfs;
    int *ordem_dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);
    imprimir_ordenacao("DFS       ", ordem_dfs, tamanho_dfs);
    free(ordem_dfs);

    liberar_grafo(g);
    return 0;
}