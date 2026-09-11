#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

/* ---------- Construção / utilitários ---------- */

GrafoLista *criar_grafo_lista(int numVertices) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->numVertices = numVertices;
    g->adj = calloc(numVertices, sizeof(NoAdj *));
    return g;
}

void adicionar_aresta(GrafoLista *g, int u, int v) {
    NoAdj *no1 = malloc(sizeof(NoAdj));
    no1->destino = v;
    no1->prox = g->adj[u];
    g->adj[u] = no1;

    NoAdj *no2 = malloc(sizeof(NoAdj));
    no2->destino = u;
    no2->prox = g->adj[v];
    g->adj[v] = no2;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (!g) return;
    for (int i = 0; i < g->numVertices; i++) {
        NoAdj *atual = g->adj[i];
        while (atual) {
            NoAdj *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int contar_arestas(const GrafoLista *g) {
    int soma = 0;
    for (int i = 0; i < g->numVertices; i++) {
        for (NoAdj *atual = g->adj[i]; atual != NULL; atual = atual->prox) {
            soma++;
        }
    }
    return soma / 2; /* cada aresta aparece duas vezes na lista */
}

int grau_vertice(const GrafoLista *g, int v) {
    int grau = 0;
    for (NoAdj *atual = g->adj[v]; atual != NULL; atual = atual->prox) {
        grau++;
    }
    return grau;
}

/* ---------- Pontos de articulação (Tarjan) ---------- */

void dfs_articulacoes(GrafoLista *g, int u, int pai,
                       bool visitado[], int descoberta[], int low[],
                       bool eh_articulacao[], int *tempo) {
    int filhos = 0;
    visitado[u] = true;
    descoberta[u] = low[u] = (*tempo)++;

    for (NoAdj *aresta = g->adj[u]; aresta != NULL; aresta = aresta->prox) {
        int w = aresta->destino;

        if (!visitado[w]) {
            filhos++;
            dfs_articulacoes(g, w, u, visitado, descoberta, low, eh_articulacao, tempo);

            low[u] = (low[u] < low[w]) ? low[u] : low[w];

            /* raiz da árvore de busca: articulação se tiver 2+ filhos */
            if (pai == -1 && filhos > 1) {
                eh_articulacao[u] = true;
            }
            /* vértice interno: articulação se nenhum atalho do filho
             * "pula por cima" de u */
            if (pai != -1 && low[w] >= descoberta[u]) {
                eh_articulacao[u] = true;
            }
        } else if (w != pai) {
            /* aresta de retorno (back edge) */
            low[u] = (low[u] < descoberta[w]) ? low[u] : descoberta[w];
        }
    }
}

int encontrar_articulacoes(GrafoLista *g, int articulacoes[]) {
    int n = g->numVertices;
    bool *visitado = calloc(n, sizeof(bool));
    int *descoberta = malloc(n * sizeof(int));
    int *low = malloc(n * sizeof(int));
    bool *eh_articulacao = calloc(n, sizeof(bool));
    int tempo = 0;

    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            dfs_articulacoes(g, i, -1, visitado, descoberta, low, eh_articulacao, &tempo);
        }
    }

    int total = 0;
    for (int i = 0; i < n; i++) {
        if (eh_articulacao[i]) {
            articulacoes[total++] = i;
        }
    }

    free(visitado);
    free(descoberta);
    free(low);
    free(eh_articulacao);
    return total;
}

/* ---------- Pontes / arestas de corte ---------- */

static void dfs_pontes(GrafoLista *g, int u, int pai,
                        bool visitado[], int descoberta[], int low[],
                        Aresta pontes[], int *numPontes, int *tempo) {
    visitado[u] = true;
    descoberta[u] = low[u] = (*tempo)++;

    for (NoAdj *aresta = g->adj[u]; aresta != NULL; aresta = aresta->prox) {
        int w = aresta->destino;

        if (!visitado[w]) {
            dfs_pontes(g, w, u, visitado, descoberta, low, pontes, numPontes, tempo);

            low[u] = (low[u] < low[w]) ? low[u] : low[w];

            /* condição ESTRITA (diferente da articulação): a aresta
             * (u, w) só é ponte se não existir NENHUM caminho de volta
             * de w (ou de seus descendentes) até u ou um ancestral. */
            if (low[w] > descoberta[u]) {
                pontes[*numPontes].u = u;
                pontes[*numPontes].v = w;
                (*numPontes)++;
            }
        } else if (w != pai) {
            low[u] = (low[u] < descoberta[w]) ? low[u] : descoberta[w];
        }
    }
}

int detectar_pontes(GrafoLista *g, Aresta pontes[]) {
    int n = g->numVertices;
    bool *visitado = calloc(n, sizeof(bool));
    int *descoberta = malloc(n * sizeof(int));
    int *low = malloc(n * sizeof(int));
    int numPontes = 0;
    int tempo = 0;

    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            dfs_pontes(g, i, -1, visitado, descoberta, low, pontes, &numPontes, &tempo);
        }
    }

    free(visitado);
    free(descoberta);
    free(low);
    return numPontes;
}