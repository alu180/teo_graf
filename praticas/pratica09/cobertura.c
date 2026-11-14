#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

/* ==================================================================== */
/*  CONSTRUCAO E UTILIDADES DO GRAFO                                     */
/* ==================================================================== */

void criar_grafo(Grafo *g, int n) {
    int i, j;
    if (n > MAX_VERTICES) n = MAX_VERTICES;
    g->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
}

void inserir_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;   /* grafo nao-dirigido: liga nos dois sentidos */
}

int total_arestas(const Grafo *g) {
    int i, j, m = 0;
    for (i = 0; i < g->n; i++)
        for (j = i + 1; j < g->n; j++)
            if (g->adj[i][j]) m++;
    return m;
}

void gerar_grafo_aleatorio(Grafo *g, int n, double prob) {
    int i, j;
    criar_grafo(g, n);
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if ((double)rand() / RAND_MAX < prob)
                inserir_aresta(g, i, j);
}

void imprimir_grafo(const Grafo *g) {
    int i, j;
    printf("Grafo com %d vertices e %d arestas:\n", g->n, total_arestas(g));
    for (i = 0; i < g->n; i++)
        for (j = i + 1; j < g->n; j++)
            if (g->adj[i][j])
                printf("  (%d, %d)\n", i, j);
}

void imprimir_conjunto(const char *rotulo, const int *selecionado, int n) {
    int i, primeiro = 1;
    printf("%s { ", rotulo);
    for (i = 0; i < n; i++) {
        if (selecionado[i]) {
            if (!primeiro) printf(", ");
            printf("%d", i);
            primeiro = 0;
        }
    }
    if (primeiro) printf("vazio ");
    printf(" }\n");
}

/* ==================================================================== */
/*  1) VERIFICACAO DE COBERTURA                                          */
/* ==================================================================== */
/*
 * Uma cobertura de vertices e valida quando NAO existe nenhuma aresta
 * com as duas pontas fora do conjunto. Basta achar uma aresta assim
 * para o conjunto ser invalido.
 */
int eh_cobertura(const Grafo *g, const int *selecionado) {
    int i, j;
    for (i = 0; i < g->n; i++)
        for (j = i + 1; j < g->n; j++)
            if (g->adj[i][j] && !selecionado[i] && !selecionado[j])
                return 0;   /* aresta (i,j) descoberta */
    return 1;
}

/* ==================================================================== */
/*  2) HEURISTICA GULOSA - MAIOR GRAU PRIMEIRO                           */
/* ==================================================================== */
/*
 * Mantem uma copia da matriz com as arestas AINDA NAO cobertas.
 * A cada rodada escolhe o vertice de maior grau nessa matriz, coloca ele
 * na cobertura e apaga todas as arestas dele. Repete ate nao sobrar aresta.
 *
 * O grau e recalculado a cada rodada: usar os graus originais daria um
 * resultado pior.
 */
int cobertura_gulosa(const Grafo *g, int *selecionado) {
    static int restante[MAX_VERTICES][MAX_VERTICES];
    int i, j, v, u;
    int tam = 0;

    for (i = 0; i < g->n; i++) {
        selecionado[i] = 0;
        for (j = 0; j < g->n; j++)
            restante[i][j] = g->adj[i][j];
    }

    while (1) {
        int melhor_v = -1;
        int melhor_grau = 0;

        /* procura o vertice de maior grau entre as arestas nao cobertas */
        for (v = 0; v < g->n; v++) {
            int grau = 0;
            for (u = 0; u < g->n; u++)
                grau += restante[v][u];
            if (grau > melhor_grau) {
                melhor_grau = grau;
                melhor_v = v;
            }
        }

        if (melhor_v == -1) break;  /* nao sobrou aresta: terminou */

        selecionado[melhor_v] = 1;
        tam++;

        /* apaga todas as arestas incidentes ao vertice escolhido */
        for (u = 0; u < g->n; u++) {
            restante[melhor_v][u] = 0;
            restante[u][melhor_v] = 0;
        }
    }

    return tam;
}

/* ==================================================================== */
/*  3) FORCA BRUTA COM BACKTRACKING E PODA                               */
/* ==================================================================== */

typedef struct {
    const Grafo *g;
    int atual[MAX_VERTICES];    /* decisao parcial em construcao */
    int melhor[MAX_VERTICES];   /* melhor cobertura ja encontrada */
    int melhor_tam;
    long long nos;              /* contador de nos explorados */
} EstadoBT;

static void backtrack(EstadoBT *e, int v, int tam_atual) {
    e->nos++;

    /* PODA POR LIMITE: o caminho atual ja empatou ou perdeu da melhor
       solucao conhecida, entao nenhuma folha abaixo dele pode melhorar. */
    if (tam_atual >= e->melhor_tam)
        return;

    /* todos os vertices foram decididos: chegou numa folha */
    if (v == e->g->n) {
        if (eh_cobertura(e->g, e->atual)) {
            e->melhor_tam = tam_atual;
            memcpy(e->melhor, e->atual, sizeof(int) * e->g->n);
        }
        return;
    }

    /* ramo 1: coloca v na cobertura */
    e->atual[v] = 1;
    backtrack(e, v + 1, tam_atual + 1);

    /* ramo 2: deixa v de fora */
    e->atual[v] = 0;
    backtrack(e, v + 1, tam_atual);

    e->atual[v] = 0;   /* desfaz a escolha antes de voltar */
}

int cobertura_minima_bt(const Grafo *g, int *melhor, long long *nos_visitados) {
    static EstadoBT e;
    int i;

    e.g = g;
    e.nos = 0;
    e.melhor_tam = g->n + 1;    /* pior caso: todos os vertices */
    for (i = 0; i < g->n; i++) {
        e.atual[i] = 0;
        e.melhor[i] = 1;        /* se nada melhorar, pegar todos e valido */
    }

    backtrack(&e, 0, 0);

    for (i = 0; i < g->n; i++)
        melhor[i] = e.melhor[i];

    if (nos_visitados) *nos_visitados = e.nos;

    return e.melhor_tam;
}