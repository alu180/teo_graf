#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tps.h"

/* ==================================================================== */
/*  CONSTRUCAO E UTILIDADES                                              */
/* ==================================================================== */

void gerar_tsp_aleatorio(GrafoPonderado *gp, int n, int custo_max) {
    int i, j, c;

    if (n < 0) n = 0;
    if (n > MAX_VERTICES) n = MAX_VERTICES;
    if (custo_max < 1) custo_max = 1;

    gp->n = n;
    for (i = 0; i < n; i++) {
        gp->custo[i][i] = 0;
        for (j = i + 1; j < n; j++) {
            c = 1 + rand() % custo_max;
            gp->custo[i][j] = c;
            gp->custo[j][i] = c;        /* instancia simetrica */
        }
    }
}

int custo_rota(const GrafoPonderado *gp, const int *rota) {
    int i, total = 0;

    if (gp->n <= 1) return 0;

    for (i = 0; i < gp->n - 1; i++)
        total += gp->custo[rota[i]][rota[i + 1]];

    total += gp->custo[rota[gp->n - 1]][rota[0]];   /* volta a origem */
    return total;
}

void imprimir_matriz_custos(const GrafoPonderado *gp) {
    int i, j;

    printf("  Matriz de custos (%d cidades):\n", gp->n);
    printf("       ");
    for (j = 0; j < gp->n; j++) printf("%4d", j);
    printf("\n");
    for (i = 0; i < gp->n; i++) {
        printf("    %2d ", i);
        for (j = 0; j < gp->n; j++) printf("%4d", gp->custo[i][j]);
        printf("\n");
    }
}

void imprimir_rota(const char *rotulo, const int *rota, int n, int custo) {
    int i;

    printf("%s", rotulo);
    for (i = 0; i < n; i++)
        printf(" %d ->", rota[i]);
    printf(" %d   (custo = %d)\n", rota[0], custo);
}

/* ==================================================================== */
/*  REDUCAO: CICLO HAMILTONIANO -> TSP                                   */
/* ==================================================================== */

void grafo_para_tsp(const Grafo *g, GrafoPonderado *gp) {
    int i, j;

    gp->n = g->n;
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            if (i == j)
                gp->custo[i][j] = 0;
            else if (g->adj[i][j])
                gp->custo[i][j] = 1;    /* aresta existe  */
            else
                gp->custo[i][j] = 2;    /* aresta ausente */
        }
    }
}

/* ==================================================================== */
/*  TSP - FORCA BRUTA                                                    */
/* ==================================================================== */

/*
 * Gera as permutacoes dos vertices 1..n-1 (o vertice 0 fica fixo na
 * posicao 0). O custo e acumulado durante a recursao, entao cada
 * permutacao completa ja chega com quase todo o valor somado.
 */
static void permutar(const GrafoPonderado *gp, int *atual, int *visitado, int pos,
                     int custo_parcial, int *melhor_rota, int *melhor_custo,
                     long long *permutacoes) {
    int u, i, total;

    if (pos == gp->n) {
        total = custo_parcial + gp->custo[atual[gp->n - 1]][atual[0]];
        (*permutacoes)++;

        if (total < *melhor_custo) {
            *melhor_custo = total;
            for (i = 0; i < gp->n; i++)
                melhor_rota[i] = atual[i];
        }
        return;
    }

    for (u = 1; u < gp->n; u++) {           /* comeca em 1: a origem ja esta fixa */
        if (!visitado[u]) {
            visitado[u] = 1;
            atual[pos] = u;

            permutar(gp, atual, visitado, pos + 1,
                     custo_parcial + gp->custo[atual[pos - 1]][u],
                     melhor_rota, melhor_custo, permutacoes);

            visitado[u] = 0;
        }
    }
}

int tsp_forca_bruta(const GrafoPonderado *gp, int *rota, long long *permutacoes) {
    int visitado[MAX_VERTICES];
    int atual[MAX_VERTICES];
    int melhor_custo = INT_MAX;
    long long perms = 0;
    int i;

    if (permutacoes != NULL) *permutacoes = 0;

    if (gp->n <= 0) return 0;
    if (gp->n == 1) { rota[0] = 0; return 0; }

    for (i = 0; i < gp->n; i++) visitado[i] = 0;

    visitado[0] = 1;        /* ORIGEM FIXA */
    atual[0] = 0;
    for (i = 0; i < gp->n; i++) rota[i] = i;

    permutar(gp, atual, visitado, 1, 0, rota, &melhor_custo, &perms);

    if (permutacoes != NULL) *permutacoes = perms;
    return melhor_custo;
}

/* ==================================================================== */
/*  TSP - HEURISTICA DO VIZINHO MAIS PROXIMO                             */
/* ==================================================================== */

int tsp_vizinho_mais_proximo(const GrafoPonderado *gp, int *rota) {
    int visitado[MAX_VERTICES];
    int i, pos, u, atual, melhor_u, melhor_c;

    if (gp->n <= 0) return 0;

    for (i = 0; i < gp->n; i++) visitado[i] = 0;

    atual = 0;              /* ORIGEM FIXA */
    rota[0] = 0;
    visitado[0] = 1;

    for (pos = 1; pos < gp->n; pos++) {
        melhor_u = -1;
        melhor_c = INT_MAX;

        for (u = 0; u < gp->n; u++) {
            if (!visitado[u] && gp->custo[atual][u] < melhor_c) {
                melhor_c = gp->custo[atual][u];
                melhor_u = u;
            }
        }

        rota[pos] = melhor_u;
        visitado[melhor_u] = 1;
        atual = melhor_u;
    }

    return custo_rota(gp, rota);
}

/* ==================================================================== */
/*  TSP - HEURISTICA DE INSERCAO DO MAIS PROXIMO                         */
/* ==================================================================== */

int tsp_insercao(const GrafoPonderado *gp, int *rota) {
    int noCiclo[MAX_VERTICES];
    int i, j, tam, u, v;
    int melhor_v, melhor_d, melhor_pos, melhor_delta;
    int a, b, d, delta;

    if (gp->n <= 0) return 0;
    if (gp->n == 1) { rota[0] = 0; return 0; }

    for (i = 0; i < gp->n; i++) noCiclo[i] = 0;

    /* ciclo inicial: a origem e a cidade mais proxima dela */
    rota[0] = 0;
    noCiclo[0] = 1;

    melhor_v = -1;
    melhor_d = INT_MAX;
    for (u = 1; u < gp->n; u++) {
        if (gp->custo[0][u] < melhor_d) {
            melhor_d = gp->custo[0][u];
            melhor_v = u;
        }
    }
    rota[1] = melhor_v;
    noCiclo[melhor_v] = 1;
    tam = 2;

    while (tam < gp->n) {
        /* 1) cidade de fora mais proxima de ALGUM vertice do ciclo */
        melhor_v = -1;
        melhor_d = INT_MAX;
        for (v = 0; v < gp->n; v++) {
            if (noCiclo[v]) continue;

            d = INT_MAX;
            for (i = 0; i < tam; i++)
                if (gp->custo[rota[i]][v] < d)
                    d = gp->custo[rota[i]][v];

            if (d < melhor_d) {
                melhor_d = d;
                melhor_v = v;
            }
        }

        /* 2) posicao de insercao que menos aumenta o custo:
              delta = custo(a,v) + custo(v,b) - custo(a,b)          */
        melhor_pos = 0;
        melhor_delta = INT_MAX;
        for (i = 0; i < tam; i++) {
            a = rota[i];
            b = rota[(i + 1) % tam];
            delta = gp->custo[a][melhor_v] + gp->custo[melhor_v][b] - gp->custo[a][b];

            if (delta < melhor_delta) {
                melhor_delta = delta;
                melhor_pos = i;
            }
        }

        /* 3) insere melhor_v logo depois da posicao melhor_pos */
        for (j = tam; j > melhor_pos + 1; j--)
            rota[j] = rota[j - 1];
        rota[melhor_pos + 1] = melhor_v;
        noCiclo[melhor_v] = 1;
        tam++;
    }

    return custo_rota(gp, rota);
}