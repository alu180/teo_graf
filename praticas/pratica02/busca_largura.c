#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"

/* ===================== Grafo (lista de adjacencia) ===================== */

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = malloc(n * sizeof(No *));
    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

static void inserir_na_lista(No **cabeca, int destino) {
    No *novo = malloc(sizeof(No));
    novo->destino = destino;
    novo->prox = *cabeca;
    *cabeca = novo;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    inserir_na_lista(&g->adj[u], v);
    inserir_na_lista(&g->adj[v], u); /* grafo nao-direcionado */
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

/* ===================== Fila - array circular ===================== */

Fila *criar_fila(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}

void enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) return; 
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int desenfileirar(Fila *f) {
    if (fila_vazia(f)) return -1;
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

/* ===================== bfs ===================== */

void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila *f = criar_fila(g->n);
    dist[origem] = 0;
    enfileirar(f, origem);

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(f, v);
            }
            atual = atual->prox;
        }
    }

    liberar_fila(f);
}