#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

/* ---------- Construção / destruição ---------- */

GrafoLista *criar_grafo(int num_vertices) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    if (g == NULL) {
        fprintf(stderr, "Erro ao alocar grafo.\n");
        exit(1);
    }

    g->num_vertices = num_vertices;
    g->adj = malloc(num_vertices * sizeof(Node *));
    if (g->adj == NULL) {
        fprintf(stderr, "Erro ao alocar lista de adjacencia.\n");
        exit(1);
    }

    for (int i = 0; i < num_vertices; i++) {
        g->adj[i] = NULL;
    }

    return g;
}

void adicionar_aresta(GrafoLista *g, int origem, int destino) {
    Node *novo = malloc(sizeof(Node));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar aresta.\n");
        exit(1);
    }

    novo->dest = destino;
    novo->next = g->adj[origem];
    g->adj[origem] = novo;
}

void liberar_grafo(GrafoLista *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->num_vertices; i++) {
        Node *atual = g->adj[i];
        while (atual != NULL) {
            Node *tmp = atual;
            atual = atual->next;
            free(tmp);
        }
    }

    free(g->adj);
    free(g);
}

/* ---------- Detecção de ciclo (usada por eh_dag e pela DFS) ---------- */

/* Cores: 0 = branco (nao visitado), 1 = cinza (em processamento na
 * pilha de recursao), 2 = preto (finalizado). Uma aresta para um
 * vertice cinza é uma aresta de retorno => ciclo. */
static int dfs_tem_ciclo(GrafoLista *g, int v, int *cor) {
    cor[v] = 1;

    for (Node *n = g->adj[v]; n != NULL; n = n->next) {
        int u = n->dest;
        if (cor[u] == 1) {
            return 1; /* aresta de retorno: ciclo encontrado */
        }
        if (cor[u] == 0 && dfs_tem_ciclo(g, u, cor)) {
            return 1;
        }
    }

    cor[v] = 2;
    return 0;
}

int eh_dag(GrafoLista *g) {
    int n = g->num_vertices;
    int *cor = calloc(n, sizeof(int));
    if (cor == NULL) {
        fprintf(stderr, "Erro ao alocar memoria em eh_dag.\n");
        exit(1);
    }

    int tem_ciclo = 0;
    for (int v = 0; v < n && !tem_ciclo; v++) {
        if (cor[v] == 0) {
            tem_ciclo = dfs_tem_ciclo(g, v, cor);
        }
    }

    free(cor);
    return !tem_ciclo;
}

/* ---------- Algoritmo de Kahn (BFS com grau de entrada) ---------- */

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;

    int *grau_entrada = calloc(n, sizeof(int));
    int *fila = malloc(n * sizeof(int));
    int *resultado = malloc(n * sizeof(int));
    if (grau_entrada == NULL || fila == NULL || resultado == NULL) {
        fprintf(stderr, "Erro ao alocar memoria em ordenacao_topologica_kahn.\n");
        exit(1);
    }

    /* Calcula o grau de entrada de cada vertice */
    for (int v = 0; v < n; v++) {
        for (Node *node = g->adj[v]; node != NULL; node = node->next) {
            grau_entrada[node->dest]++;
        }
    }

    /* Fila com todos os vertices de grau de entrada 0 */
    int inicio = 0, fim = 0;
    for (int v = 0; v < n; v++) {
        if (grau_entrada[v] == 0) {
            fila[fim++] = v;
        }
    }

    int idx = 0;
    while (inicio < fim) {
        int u = fila[inicio++];
        resultado[idx++] = u;

        for (Node *node = g->adj[u]; node != NULL; node = node->next) {
            int v = node->dest;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
        }
    }

    free(grau_entrada);
    free(fila);

    if (idx != n) {
        /* Nem todos os vertices foram processados: ha ciclo */
        free(resultado);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = n;
    return resultado;
}

/* ---------- Ordenação topológica por DFS ---------- */

static void dfs_topologica(GrafoLista *g, int v, int *visitado, int *pilha, int *topo) {
    visitado[v] = 1;

    for (Node *node = g->adj[v]; node != NULL; node = node->next) {
        if (!visitado[node->dest]) {
            dfs_topologica(g, node->dest, visitado, pilha, topo);
        }
    }

    /* Empilha o vertice na saida da DFS (ele so sai depois de todos
     * os seus sucessores) */
    pilha[(*topo)++] = v;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;

    /* Se houver ciclo, ordenacao topologica nao existe */
    if (!eh_dag(g)) {
        *tamanho = 0;
        return NULL;
    }

    int *visitado = calloc(n, sizeof(int));
    int *pilha = malloc(n * sizeof(int));
    int *resultado = malloc(n * sizeof(int));
    if (visitado == NULL || pilha == NULL || resultado == NULL) {
        fprintf(stderr, "Erro ao alocar memoria em ordenacao_topologica_dfs.\n");
        exit(1);
    }

    int topo = 0;
    for (int v = 0; v < n; v++) {
        if (!visitado[v]) {
            dfs_topologica(g, v, visitado, pilha, &topo);
        }
    }

    /* A pilha sai na ordem inversa da topologica: inverte ao copiar */
    for (int i = 0; i < n; i++) {
        resultado[i] = pilha[n - 1 - i];
    }

    free(visitado);
    free(pilha);

    *tamanho = n;
    return resultado;
}