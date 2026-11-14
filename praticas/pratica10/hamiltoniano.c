#include <stdio.h>
#include "hamiltoniano.h"

/* ==================================================================== */
/*  CONSTRUCAO E UTILIDADES                                              */
/* ==================================================================== */

void criar_grafo(Grafo *g, int n) {
    int i, j;

    if (n < 0) n = 0;
    if (n > MAX_VERTICES) n = MAX_VERTICES;

    g->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
}

void inserir_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return;                 /* ignora aresta invalida e laco */

    g->adj[u][v] = 1;
    g->adj[v][u] = 1;           /* grafo nao-dirigido: matriz simetrica */
}

void imprimir_grafo(const Grafo *g) {
    int i, j;

    printf("  Grafo com %d vertices:\n", g->n);
    for (i = 0; i < g->n; i++) {
        printf("    %2d ->", i);
        for (j = 0; j < g->n; j++)
            if (g->adj[i][j])
                printf(" %d", j);
        printf("\n");
    }
}

/* ==================================================================== */
/*  BACKTRACKING                                                         */
/* ==================================================================== */

/*
 * Monta o caminho vertice a vertice.
 * caminho[0..pos-1] ja esta preenchido; precisamos escolher caminho[pos].
 * Se nenhuma escolha der certo, desfaz a marcacao (backtrack) e volta.
 */
static int bt(const Grafo *g, int *caminho, int *visitado, int pos,
              int exigir_ciclo, long long *nos) {
    int atual, u;

    (*nos)++;

    if (pos == g->n) {                      /* todos os vertices ja entraram */
        if (!exigir_ciclo)
            return 1;                       /* caminho hamiltoniano completo */
        /* para o ciclo ainda falta a aresta de volta ao vertice inicial */
        return g->adj[caminho[g->n - 1]][caminho[0]];
    }

    atual = caminho[pos - 1];

    for (u = 0; u < g->n; u++) {
        if (!visitado[u] && g->adj[atual][u]) {
            visitado[u] = 1;
            caminho[pos] = u;

            if (bt(g, caminho, visitado, pos + 1, exigir_ciclo, nos))
                return 1;

            visitado[u] = 0;                /* <-- backtrack: desfaz a escolha */
        }
    }

    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho,
                                      int exigir_ciclo, long long *nos_visitados) {
    int visitado[MAX_VERTICES];
    int inicio, i;
    long long nos = 0;

    if (nos_visitados != NULL) *nos_visitados = 0;

    if (g->n <= 0) return 0;
    if (exigir_ciclo && g->n < 3) return 0;   /* ciclo precisa de ao menos 3 vertices */

    if (g->n == 1 && !exigir_ciclo) {         /* unico vertice ja e um caminho */
        caminho[0] = 0;
        if (nos_visitados != NULL) *nos_visitados = 1;
        return 1;
    }

    /*
     * Para o CICLO basta comecar no vertice 0: todo ciclo hamiltoniano
     * passa por todos os vertices, entao passa por 0.
     * Para o CAMINHO e preciso testar cada vertice como ponto de partida.
     */
    for (inicio = 0; inicio < g->n; inicio++) {
        for (i = 0; i < g->n; i++) visitado[i] = 0;

        visitado[inicio] = 1;
        caminho[0] = inicio;

        if (bt(g, caminho, visitado, 1, exigir_ciclo, &nos)) {
            if (nos_visitados != NULL) *nos_visitados = nos;
            return 1;
        }

        if (exigir_ciclo) break;
    }

    if (nos_visitados != NULL) *nos_visitados = nos;
    return 0;
}

/* ==================================================================== */
/*  VERIFICACAO E SAIDA                                                  */
/* ==================================================================== */

int eh_caminho_hamiltoniano(const Grafo *g, const int *caminho, int exigir_ciclo) {
    int visto[MAX_VERTICES];
    int i;

    if (g->n <= 0) return 0;

    for (i = 0; i < g->n; i++) visto[i] = 0;

    /* 1) cada vertice aparece exatamente uma vez */
    for (i = 0; i < g->n; i++) {
        int v = caminho[i];
        if (v < 0 || v >= g->n) return 0;
        if (visto[v]) return 0;
        visto[v] = 1;
    }

    /* 2) existe aresta entre vertices consecutivos */
    for (i = 0; i < g->n - 1; i++)
        if (!g->adj[caminho[i]][caminho[i + 1]])
            return 0;

    /* 3) no ciclo, existe tambem a aresta de volta */
    if (exigir_ciclo && !g->adj[caminho[g->n - 1]][caminho[0]])
        return 0;

    return 1;
}

void imprimir_caminho(const char *rotulo, const int *caminho, int n, int exigir_ciclo) {
    int i;

    printf("%s", rotulo);
    for (i = 0; i < n; i++)
        printf(" %d%s", caminho[i], (i < n - 1) ? " ->" : "");
    if (exigir_ciclo)
        printf(" -> %d", caminho[0]);
    printf("\n");
}