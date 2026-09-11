#ifndef COLORACAO_H
#define COLORACAO_H

/*
 * Representação do grafo por lista de adjacência (não-direcionado).
 * Cada vértice é um índice de 0 a num_vertices - 1.
 */

typedef struct No {
    int vertice;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    No **adj; /* adj[i] = ponteiro para a lista de vizinhos do vértice i */
} GrafoLista;

/* ---- Construção / destruição do grafo ---- */
GrafoLista *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista *g, int origem, int destino);
void liberar_grafo(GrafoLista *g);
int grau(const GrafoLista *g, int v);

/* ---- Funções de coloração pedidas na prática ---- */

/* Colore os vértices na ordem 0, 1, 2, ..., n-1 (ordenação arbitrária).
 * Retorna um vetor alocado dinamicamente onde cores[v] é a cor do vértice v.
 * *num_cores recebe a quantidade de cores usadas. */
int *coloracao_gulosa(GrafoLista *g, int *num_cores);

/* Igual à gulosa, mas antes ordena os vértices por grau decrescente
 * (heurística de Welsh-Powell). */
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);

/* Retorna 1 se o grafo é bipartido (número cromático <= 2) e 0 caso
 * contrário. Usa busca em largura (BFS) tentando colorir com 2 cores. */
int eh_bipartido(GrafoLista *g);

/* ---- Utilitário ---- */
void imprimir_cores(const int *cores, int num_vertices);

#endif