#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->n = n;

    // um vetor de n "cabecas" de lista, uma para cada vertice
    g->adj = malloc(n * sizeof(No *));
    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL; // lista vazia = sem vizinhos ainda
    }
    return g;
}

// funcao auxiliar: insere 'destino' no comeco da lista apontada por 'cabeca'
static void inserir_na_lista(No **cabeca, int destino) {
    No *novo = malloc(sizeof(No));
    novo->destino = destino;
    novo->prox = *cabeca;
    *cabeca = novo;
}

// funcao auxiliar: remove o no com valor 'destino' da lista, se existir
static void remover_da_lista(No **cabeca, int destino) {
    No *atual = *cabeca;
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == destino) {
            if (anterior == NULL) {
                *cabeca = atual->prox; // era o primeiro da lista
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    inserir_na_lista(&g->adj[u], v);
    inserir_na_lista(&g->adj[v], u); // nao-direcionado: aresta nos dois sentidos
}

void remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    remover_da_lista(&g->adj[u], v);
    remover_da_lista(&g->adj[v], u);
}

int grau_lista(GrafoLista *g, int v) {
    int grau = 0;
    No *atual = g->adj[v];
    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    No *atual = g->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }
    return 0;
}

void imprimir_grafo_lista(GrafoLista *g) {
    for (int i = 0; i < g->n; i++) {
        printf("%d: ", i);
        No *atual = g->adj[i];
        while (atual != NULL) {
            printf("-> %d ", atual->destino);
            atual = atual->prox;
        }
        printf("\n");
    }
}

void liberar_grafo_lista(GrafoLista *g) {
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}