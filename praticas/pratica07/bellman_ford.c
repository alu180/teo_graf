#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

GrafoListaPonderado *criar_grafo(int n) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = calloc(n, sizeof(Aresta *)); /* todas as listas começam vazias */
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    Aresta *a = malloc(sizeof(Aresta));
    a->destino = v;
    a->peso = peso;
    a->prox = g->adj[u]; /* insere no início da lista de u */
    g->adj[u] = a;
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int u = 0; u < g->n; u++) {
        Aresta *a = g->adj[u];
        while (a != NULL) {
            Aresta *tmp = a;
            a = a->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->n;

    /* 1) Inicialização: todos "infinitos", só a origem vale 0 */
    for (int i = 0; i < n; i++) {
        dist[i] = INFINITO;
        pred[i] = SEM_PRED;
    }
    dist[origem] = 0;

    /* 2) n - 1 iterações relaxando todas as arestas */
    for (int i = 1; i <= n - 1; i++) {
        for (int u = 0; u < n; u++) {
            if (dist[u] == INFINITO) continue; /* evita infinito + peso (overflow) */
            for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
                int v = a->destino;
                if (dist[u] + a->peso < dist[v]) {
                    dist[v] = dist[u] + a->peso;
                    pred[v] = u;
                }
            }
        }
    }

    /* 3) n-ésima iteração: se algo ainda melhorar, há ciclo negativo */
    for (int u = 0; u < n; u++) {
        if (dist[u] == INFINITO) continue;
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            if (dist[u] + a->peso < dist[a->destino]) {
                return 0; /* ciclo negativo */
            }
        }
    }

    return 1; /* sucesso */
}

/* Retorna o peso da aresta u -> v (ou 0 se não existir) */
static int peso_aresta(GrafoListaPonderado *g, int u, int v) {
    for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
        if (a->destino == v) return a->peso;
    }
    return 0;
}

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred) {
    int n = g->n;
    int x = -1;

    /* 1) Acha um vértice que ainda relaxa (o mesmo teste da n-ésima iteração) */
    for (int u = 0; u < n && x == -1; u++) {
        if (dist[u] == INFINITO) continue;
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (dist[u] + a->peso < dist[v]) {
                pred[v] = u;
                x = v;
                break;
            }
        }
    }

    if (x == -1) {
        printf("Nenhum ciclo negativo encontrado.\n");
        return;
    }

    /* 2) Retrocede n vezes pelo pred[] para garantir que caiu dentro do ciclo */
    for (int i = 0; i < n; i++) {
        if (pred[x] == SEM_PRED) {
            printf("Erro ao reconstruir o ciclo.\n");
            return;
        }
        x = pred[x];
    }

    /* 3) Percorre o ciclo a partir de x até voltar em x */
    int *ciclo = malloc((n + 1) * sizeof(int));
    int tam = 0;
    int v = x;
    do {
        ciclo[tam++] = v;
        v = pred[v];
    } while (v != x && tam <= n);

    /* ciclo[] está "de trás pra frente" (pred aponta para quem vem antes),
       então imprimimos invertido: x -> ciclo[tam-1] -> ... -> ciclo[1] -> x */
    int custo = 0;
    int anterior = x;
    printf("Ciclo negativo: %d", x);
    for (int i = tam - 1; i >= 1; i--) {
        printf(" -> %d", ciclo[i]);
        custo += peso_aresta(g, anterior, ciclo[i]);
        anterior = ciclo[i];
    }
    printf(" -> %d", x);
    custo += peso_aresta(g, anterior, x);
    printf("  (custo total = %d)\n", custo);

    free(ciclo);
}

void imprimir_caminho(int *pred, int destino) {
    if (pred[destino] != SEM_PRED) {
        imprimir_caminho(pred, pred[destino]);
        printf(" -> ");
    }
    printf("%d", destino);
}