#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

/* ===================== Grafo (lista de adjacencia) ===================== */

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;     /* numero de vertices */
    No **adj;  /* vetor de listas de adjacencia, uma por vertice */
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);
void inserir_aresta_lista(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);
void imprimir_grafo_lista(GrafoLista *g);

/* ===================== Fila para bfs ===================== */

typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

Fila *criar_fila(int capacidade);
int fila_vazia(Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);
void liberar_fila(Fila *f);


void bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif