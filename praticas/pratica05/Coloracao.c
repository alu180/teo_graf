#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

/* ============ Construção / destruição do grafo ============ */

GrafoLista *criar_grafo(int num_vertices) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(No *)); /* todas as listas começam vazias (NULL) */
    return g;
}

static No *criar_no(int vertice) {
    No *n = malloc(sizeof(No));
    n->vertice = vertice;
    n->prox = NULL;
    return n;
}

void adicionar_aresta(GrafoLista *g, int origem, int destino) {
    /* grafo não-direcionado: a aresta aparece nas duas listas */
    No *n1 = criar_no(destino);
    n1->prox = g->adj[origem];
    g->adj[origem] = n1;

    No *n2 = criar_no(origem);
    n2->prox = g->adj[destino];
    g->adj[destino] = n2;
}

void liberar_grafo(GrafoLista *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int grau(const GrafoLista *g, int v) {
    int contador = 0;
    for (No *atual = g->adj[v]; atual != NULL; atual = atual->prox) {
        contador++;
    }
    return contador;
}

/* ============ Coloração gulosa (base para as duas heurísticas) ============ */

/* Colore os vértices seguindo a ordem dada em 'ordem' (vetor de índices de
 * vértices). O resultado (cores[vertice]) é sempre indexado pelo vértice
 * original, não pela posição na ordem. */
static int *colorir_na_ordem(GrafoLista *g, const int *ordem, int *num_cores) {
    int n = g->num_vertices;
    int *cores = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        cores[i] = -1; /* -1 = ainda sem cor */
    }

    int *usada = malloc(n * sizeof(int)); /* usada[c] = 1 se algum vizinho já colorido usa a cor c */
    int maior_cor = -1;

    for (int i = 0; i < n; i++) {
        int v = ordem[i];

        for (int c = 0; c < n; c++) usada[c] = 0;
        for (No *viz = g->adj[v]; viz != NULL; viz = viz->prox) {
            if (cores[viz->vertice] != -1) {
                usada[cores[viz->vertice]] = 1;
            }
        }

        int c = 0;
        while (usada[c]) c++; /* menor cor livre */
        cores[v] = c;
        if (c > maior_cor) maior_cor = c;
    }

    free(usada);
    *num_cores = maior_cor + 1;
    return cores;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    int *ordem = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) ordem[i] = i; /* ordenação arbitrária: 0, 1, 2, ... */

    int *cores = colorir_na_ordem(g, ordem, num_cores);
    free(ordem);
    return cores;
}

/* ============ Heurística de Welsh-Powell ============ */

typedef struct {
    int vertice;
    int grau;
} VerticeGrau;

static int compara_grau_desc(const void *a, const void *b) {
    const VerticeGrau *va = a;
    const VerticeGrau *vb = b;
    return vb->grau - va->grau; /* ordem decrescente de grau */
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    VerticeGrau *vg = malloc(n * sizeof(VerticeGrau));
    for (int i = 0; i < n; i++) {
        vg[i].vertice = i;
        vg[i].grau = grau(g, i);
    }
    qsort(vg, n, sizeof(VerticeGrau), compara_grau_desc);

    int *ordem = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) ordem[i] = vg[i].vertice;
    free(vg);

    int *cores = colorir_na_ordem(g, ordem, num_cores);
    free(ordem);
    return cores;
}

/* ============ Verificação de bipartição (2-coloração via BFS) ============ */

int eh_bipartido(GrafoLista *g) {
    int n = g->num_vertices;
    int *cor = malloc(n * sizeof(int));
    int *fila = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) cor[i] = -1; /* -1 = não visitado */

    int bipartido = 1;

    for (int inicio = 0; inicio < n && bipartido; inicio++) {
        if (cor[inicio] != -1) continue; /* já visitado em outra componente */

        int ini = 0, fim = 0;
        cor[inicio] = 0;
        fila[fim++] = inicio;

        while (ini < fim && bipartido) {
            int v = fila[ini++];
            for (No *viz = g->adj[v]; viz != NULL; viz = viz->prox) {
                int w = viz->vertice;
                if (cor[w] == -1) {
                    cor[w] = 1 - cor[v]; /* alterna entre 0 e 1 */
                    fila[fim++] = w;
                } else if (cor[w] == cor[v]) {
                    bipartido = 0; /* aresta liga dois vértices da mesma cor */
                    break;
                }
            }
        }
    }

    free(cor);
    free(fila);
    return bipartido;
}

/* ============ Utilitário ============ */

void imprimir_cores(const int *cores, int num_vertices) {
    for (int v = 0; v < num_vertices; v++) {
        printf("  Vertice %d -> cor %d\n", v, cores[v]);
    }
}