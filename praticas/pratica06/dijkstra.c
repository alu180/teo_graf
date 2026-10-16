#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"


GrafoListaPonderado *criar_grafo(int num_vertices) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) {
        fprintf(stderr, "Erro: falha ao alocar o grafo.\n");
        exit(1);
    }
    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(Aresta *)); /* todas as listas comecam vazias (NULL) */
    if (g->adj == NULL) {
        fprintf(stderr, "Erro: falha ao alocar as listas de adjacencia.\n");
        exit(1);
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso) {
    if (origem < 0 || origem >= g->num_vertices || destino < 0 || destino >= g->num_vertices) {
        fprintf(stderr, "Aviso: aresta %d -> %d ignorada (vertice invalido).\n", origem, destino);
        return;
    }
    if (peso < 0) {
        fprintf(stderr, "Aviso: aresta %d -> %d ignorada (Dijkstra nao aceita peso negativo).\n",
                origem, destino);
        return;
    }
    Aresta *nova = malloc(sizeof(Aresta));
    if (nova == NULL) {
        fprintf(stderr, "Erro: falha ao alocar aresta.\n");
        exit(1);
    }
    nova->destino = destino;
    nova->peso = peso;
    nova->prox = g->adj[origem]; /* insere no inicio da lista */
    g->adj[origem] = nova;
}

void imprimir_grafo(GrafoListaPonderado *g) {
    for (int u = 0; u < g->num_vertices; u++) {
        printf("  %d:", u);
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            printf(" -> %d (peso %d)", a->destino, a->peso);
        }
        printf("\n");
    }
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (g == NULL) return;
    for (int u = 0; u < g->num_vertices; u++) {
        Aresta *a = g->adj[u];
        while (a != NULL) {
            Aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(g->adj);
    free(g);
}

/* =====================================================================
 *                              MIN-HEAP
 * ===================================================================== */

/* Indices dentro do vetor do heap:
 *   pai de i            = (i - 1) / 2
 *   filho esquerdo de i = 2*i + 1
 *   filho direito de i  = 2*i + 2
 */
#define PAI(i)      (((i) - 1) / 2)
#define ESQUERDO(i) (2 * (i) + 1)
#define DIREITO(i)  (2 * (i) + 2)

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) {
        fprintf(stderr, "Erro: falha ao alocar o heap.\n");
        exit(1);
    }
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    if (h->dados == NULL || h->posicao == NULL) {
        fprintf(stderr, "Erro: falha ao alocar os vetores do heap.\n");
        exit(1);
    }
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int v = 0; v < capacidade; v++) {
        h->posicao[v] = -1; /* nenhum vertice esta no heap ainda */
    }
    return h;
}

void liberar_min_heap(MinHeap *h) {
    if (h == NULL) return;
    free(h->dados);
    free(h->posicao);
    free(h);
}

int heap_vazio(MinHeap *h) {
    return h->tamanho == 0;
}

int esta_no_heap(MinHeap *h, int vertice) {
    return h->posicao[vertice] != -1;
}

/* Troca dois nos do heap de lugar E atualiza o mapa posicao[] */
static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;

    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

/* "Sobe" o no da posicao i enquanto ele for menor que o pai */
static void subir(MinHeap *h, int i) {
    while (i > 0 && h->dados[i].distancia < h->dados[PAI(i)].distancia) {
        trocar(h, i, PAI(i));
        i = PAI(i);
    }
}

/* "Desce" o no da posicao i enquanto algum filho for menor que ele */
static void descer(MinHeap *h, int i) {
    while (1) {
        int menor = i;
        int esq = ESQUERDO(i);
        int dir = DIREITO(i);

        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
            menor = dir;

        if (menor == i) break; /* ja esta no lugar certo */

        trocar(h, i, menor);
        i = menor;
    }
}

/* insert: O(log V) */
void inserir(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho >= h->capacidade) {
        fprintf(stderr, "Erro: heap cheio.\n");
        return;
    }
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
}

/* extract_min: O(log V) */
NoHeap extrair_minimo(MinHeap *h) {
    NoHeap minimo = {-1, INFINITO};
    if (heap_vazio(h)) return minimo;

    minimo = h->dados[0];

    /* coloca o ultimo elemento na raiz e reorganiza */
    int ultimo = h->tamanho - 1;
    trocar(h, 0, ultimo);
    h->tamanho--;
    h->posicao[minimo.vertice] = -1; /* saiu do heap */

    if (!heap_vazio(h)) descer(h, 0);

    return minimo;
}

/* decrease_key: O(log V) gracas ao mapa posicao[] (achar o vertice e O(1)) */
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];
    if (i == -1) return;                                 /* nao esta no heap */
    if (nova_distancia >= h->dados[i].distancia) return; /* nao diminuiu */

    h->dados[i].distancia = nova_distancia;
    subir(h, i);
}



void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;
    MinHeap *h = criar_min_heap(n);

    /* 1) Inicializacao: infinito sem predecessor */
    for (int v = 0; v < n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;

    /* 2) Coloca todos os vertices com sua distancia atual */
    for (int v = 0; v < n; v++) {
        inserir(h, v, dist[v]);
    }

    /* 3) Laco principal */
    while (!heap_vazio(h)) {
        NoHeap atual = extrair_minimo(h);
        int u = atual.vertice;

        /* Se o menor e infinito o resto e inalcancavel */
        if (dist[u] == INFINITO) break;

        /* 4) Relaxamento das arestas que saem de u */
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            int v = a->destino;
            int peso = a->peso;

            if (esta_no_heap(h, v) && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pred[v] = u;
                diminuir_chave(h, v, dist[v]);
            }
        }
    }

    liberar_min_heap(h);
}

/* Imprime o caminho de forma recursiva: primeiro o "antes", depois o vertice */
static void imprimir_caminho_rec(int origem, int destino, int *pred) {
    if (destino == origem) {
        printf("%d", origem);
        return;
    }
    imprimir_caminho_rec(origem, pred[destino], pred);
    printf(" -> %d", destino);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino != origem && pred[destino] == -1) {
        printf("Nao existe caminho de %d ate %d.\n", origem, destino);
        return;
    }
    imprimir_caminho_rec(origem, destino, pred);
    printf("\n");
}