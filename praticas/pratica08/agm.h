#ifndef AGM_H
#define AGM_H

/* Union-Find (Disjoint Set Union) */
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

/* Aresta de um grafo nao direcionado ponderado */
typedef struct {
    int u, v, peso;
} Aresta;

/* ---------- Union-Find ---------- */
DSU *dsu_criar(int n);
void dsu_liberar(DSU *d);

/* Retorna o representante (raiz) do conjunto de x, com compressao de caminho */
int find(DSU *d, int x);

/* Une os conjuntos de x e y por rank.
 * Retorna 1 se uniu, 0 se x e y ja estavam no mesmo conjunto. */
int unir(DSU *d, int x, int y);

/* ---------- Arvore Geradora Minima ---------- */

/* Kruskal: O(E log E).
 * n = numero de vertices (0..n-1), arestas = vetor com m arestas.
 * Escreve as arestas escolhidas em agm (capacidade minima n-1) e a
 * quantidade em *qtd. Retorna o peso total. */
int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd);

/* Prim com Min-Heap: O((V+E) log V).
 * Mesma convencao do kruskal, mais o vertice inicial. */
int prim(int n, Aresta *arestas, int m, int inicio, Aresta *agm, int *qtd);

#endif