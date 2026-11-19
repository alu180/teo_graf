#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

/* ============================================================
 * Union-Find
 * ============================================================ */

DSU *dsu_criar(int n) {
    DSU *d = (DSU *)malloc(sizeof(DSU));
    if (!d) return NULL;
    d->n = n;
    d->pai = (int *)malloc(n * sizeof(int));
    d->rank = (int *)calloc(n, sizeof(int));
    if (!d->pai || !d->rank) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }
    for (int i = 0; i < n; i++) d->pai[i] = i; /* cada um e seu proprio chefe */
    return d;
}

void dsu_liberar(DSU *d) {
    if (!d) return;
    free(d->pai);
    free(d->rank);
    free(d);
}

/* find com compressao de caminho */
int find(DSU *d, int x) {
    if (d->pai[x] != x)
        d->pai[x] = find(d, d->pai[x]); /* aponta direto para a raiz */
    return d->pai[x];
}

/* uniao por rank */
int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0; /* mesmo conjunto: uniria um ciclo */

    if (d->rank[rx] < d->rank[ry]) {
        d->pai[rx] = ry;
    } else if (d->rank[rx] > d->rank[ry]) {
        d->pai[ry] = rx;
    } else {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }
    return 1;
}

/* ============================================================
 * Kruskal
 * ============================================================ */

static int cmp_aresta(const void *a, const void *b) {
    const Aresta *x = (const Aresta *)a;
    const Aresta *y = (const Aresta *)b;
    if (x->peso < y->peso) return -1;
    if (x->peso > y->peso) return 1;
    return 0;
}

int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd) {
    int total = 0;
    *qtd = 0;

    /* Ordena uma copia para nao alterar o vetor do chamador: O(E log E) */
    Aresta *ord = (Aresta *)malloc(m * sizeof(Aresta));
    if (!ord) return 0;
    for (int i = 0; i < m; i++) ord[i] = arestas[i];
    qsort(ord, m, sizeof(Aresta), cmp_aresta);

    DSU *d = dsu_criar(n);
    if (!d) {
        free(ord);
        return 0;
    }

    for (int i = 0; i < m && *qtd < n - 1; i++) {
        if (unir(d, ord[i].u, ord[i].v)) { /* so aceita se nao formar ciclo */
            agm[(*qtd)++] = ord[i];
            total += ord[i].peso;
        }
    }

    dsu_liberar(d);
    free(ord);
    return total;
}

/* ============================================================
 * Min-Heap (usado pelo Prim)
 *
 * Guarda pares (vertice, chave). pos[v] diz em que indice do heap
 * o vertice v esta (-1 se nao esta no heap), o que permite
 * diminuir a chave de um vertice em O(log V).
 * ============================================================ */

typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int *pos;
    int tamanho;
} MinHeap;

static MinHeap *heap_criar(int capacidade) {
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->dados = (NoHeap *)malloc(capacidade * sizeof(NoHeap));
    h->pos = (int *)malloc(capacidade * sizeof(int));
    h->tamanho = 0;
    if (!h->dados || !h->pos) {
        free(h->dados);
        free(h->pos);
        free(h);
        return NULL;
    }
    for (int i = 0; i < capacidade; i++) h->pos[i] = -1;
    return h;
}

static void heap_liberar(MinHeap *h) {
    if (!h) return;
    free(h->dados);
    free(h->pos);
    free(h);
}

static void heap_trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->pos[h->dados[i].vertice] = i;
    h->pos[h->dados[j].vertice] = j;
}

static void heap_subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].chave <= h->dados[i].chave) break;
        heap_trocar(h, pai, i);
        i = pai;
    }
}

static void heap_descer(MinHeap *h, int i) {
    for (;;) {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave) menor = esq;
        if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave) menor = dir;
        if (menor == i) break;
        heap_trocar(h, i, menor);
        i = menor;
    }
}

static void heap_inserir(MinHeap *h, int vertice, int chave) {
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].chave = chave;
    h->pos[vertice] = i;
    heap_subir(h, i);
}

static NoHeap heap_extrair_min(MinHeap *h) {
    NoHeap min = h->dados[0];
    h->tamanho--;
    h->pos[min.vertice] = -1;
    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->pos[h->dados[0].vertice] = 0;
        heap_descer(h, 0);
    }
    return min;
}

static void heap_diminuir_chave(MinHeap *h, int vertice, int nova_chave) {
    int i = h->pos[vertice];
    h->dados[i].chave = nova_chave;
    heap_subir(h, i);
}

/* ============================================================
 * Prim
 * ============================================================ */

#define INF 1000000000

/* Lista de adjacencia auxiliar (so existe dentro do prim) */
typedef struct Viz {
    int dest;
    int peso;
    struct Viz *prox;
} Viz;

static void adj_inserir(Viz **adj, int u, int v, int peso) {
    Viz *novo = (Viz *)malloc(sizeof(Viz));
    novo->dest = v;
    novo->peso = peso;
    novo->prox = adj[u];
    adj[u] = novo;
}

int prim(int n, Aresta *arestas, int m, int inicio, Aresta *agm, int *qtd) {
    int total = 0;
    *qtd = 0;

    /* 1) Monta a lista de adjacencia (grafo nao direcionado) */
    Viz **adj = (Viz **)calloc(n, sizeof(Viz *));
    for (int i = 0; i < m; i++) {
        adj_inserir(adj, arestas[i].u, arestas[i].v, arestas[i].peso);
        adj_inserir(adj, arestas[i].v, arestas[i].u, arestas[i].peso);
    }

    /* 2) chave[v] = menor peso conhecido para ligar v a arvore;
     *    pai[v]   = vertice da arvore que oferece essa ligacao */
    int *chave = (int *)malloc(n * sizeof(int));
    int *pai = (int *)malloc(n * sizeof(int));
    int *na_arvore = (int *)calloc(n, sizeof(int));
    MinHeap *h = heap_criar(n);

    for (int v = 0; v < n; v++) {
        chave[v] = INF;
        pai[v] = -1;
    }
    chave[inicio] = 0;
    for (int v = 0; v < n; v++) heap_inserir(h, v, chave[v]);

    /* 3) Repete: extrai o vertice mais barato e atualiza os vizinhos */
    while (h->tamanho > 0) {
        NoHeap atual = heap_extrair_min(h);
        int u = atual.vertice;
        if (atual.chave == INF) break; /* resto inalcancavel (grafo desconexo) */

        na_arvore[u] = 1;
        if (pai[u] != -1) {
            Aresta e = { pai[u], u, chave[u] };
            agm[(*qtd)++] = e;
            total += chave[u];
        }

        for (Viz *p = adj[u]; p; p = p->prox) {
            int v = p->dest;
            if (!na_arvore[v] && p->peso < chave[v]) {
                chave[v] = p->peso;
                pai[v] = u;
                heap_diminuir_chave(h, v, p->peso); /* atualizacao de chave */
            }
        }
    }

    /* 4) Limpeza */
    for (int i = 0; i < n; i++) {
        Viz *p = adj[i];
        while (p) {
            Viz *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(adj);
    free(chave);
    free(pai);
    free(na_arvore);
    heap_liberar(h);
    return total;
}