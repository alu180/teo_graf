#include <stdlib.h>
#include "busca_profundidade.h"

/* ===================== Pilha (lifo) - array ===================== */

Pilha *criar_pilha(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    p->dados = malloc(capacidade * sizeof(int));
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

int pilha_vazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, int valor) {
    if (p->topo + 1 == p->capacidade) return; 
    p->dados[++p->topo] = valor;
}

int desempilhar(Pilha *p) {
    if (pilha_vazia(p)) return -1;
    return p->dados[p->topo--];
}

void liberar_pilha(Pilha *p) {
    free(p->dados);
    free(p);
}

/* ===================== dfs recursiva (com entrada/saida) ===================== */

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *entrada, int *saida, int *tempo) {
    visitado[u] = 1;
    entrada[u] = (*tempo)++;

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado, entrada, saida, tempo);
        }
        atual = atual->prox;
    }

    saida[u] = (*tempo)++;
}

/* ===================== dfs iterativa (com Pilha) ===================== */

void dfs_iterativa(GrafoLista *g, int origem, int *visitado) {
    Pilha *p = criar_pilha(g->n);
    empilhar(p, origem);

    while (!pilha_vazia(p)) {
        int u = desempilhar(p);
        if (visitado[u]) continue;
        visitado[u] = 1;

        No *atual = g->adj[u];
        while (atual != NULL) {
            if (!visitado[atual->destino]) {
                empilhar(p, atual->destino);
            }
            atual = atual->prox;
        }
    }

    liberar_pilha(p);
}

/* ===================== Componentes conexos (via dfs) ===================== */

int contar_componentes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = calloc(g->n, sizeof(int));
    int *saida = calloc(g->n, sizeof(int));
    int tempo = 0;
    int componentes = 0;

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            dfs_recursiva(g, i, visitado, entrada, saida, &tempo);
            componentes++;
        }
    }

    free(visitado);
    free(entrada);
    free(saida);
    return componentes;
}

/* ===================== Bipartição (2-coloracao via dfs) ===================== */

int eh_bipartido(GrafoLista *g) {
    int *cor = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) cor[i] = -1; 

    for (int inicio = 0; inicio < g->n; inicio++) {
        if (cor[inicio] != -1) continue;

        cor[inicio] = 0;
        Fila *f = criar_fila(g->n);
        enfileirar(f, inicio);

        while (!fila_vazia(f)) {
            int u = desenfileirar(f);
            No *atual = g->adj[u];
            while (atual != NULL) {
                int v = atual->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    enfileirar(f, v);
                } else if (cor[v] == cor[u]) {
                    liberar_fila(f);
                    free(cor);
                    return 0; 
                }
                atual = atual->prox;
            }
        }

        liberar_fila(f);
    }

    free(cor);
    return 1;
}

/* ===================== Deteccao de ciclo (grafo nao-direcionado, por dfs) ===================== */

static int dfs_ciclo(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;
    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) {
            if (dfs_ciclo(g, v, u, visitado)) return 1;
        } else if (v != pai) {
            return 1; 
        }
        atual = atual->prox;
    }
    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            if (dfs_ciclo(g, i, -1, visitado)) {
                free(visitado);
                return 1;
            }
        }
    }

    free(visitado);
    return 0;
}