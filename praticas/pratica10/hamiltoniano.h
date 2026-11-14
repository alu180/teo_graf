#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#define MAX_VERTICES 20

/*
 * Grafo nao-dirigido e NAO ponderado, representado por matriz de adjacencia.
 * adj[i][j] == 1  <=>  existe a aresta (i, j).
 * A matriz e simetrica: adj[i][j] == adj[j][i].
 */
typedef struct {
    int n;                                  /* quantidade de vertices */
    int adj[MAX_VERTICES][MAX_VERTICES];    /* matriz de adjacencia   */
} Grafo;

/* ---------- construcao e utilidades do grafo ---------- */
void criar_grafo(Grafo *g, int n);
void inserir_aresta(Grafo *g, int u, int v);
void imprimir_grafo(const Grafo *g);

/* ---------- funcao exigida pela pratica ---------- */

/*
 * Forca bruta com backtracking (permutacoes podadas pelas arestas).
 *
 * exigir_ciclo == 0  -> procura um CAMINHO hamiltoniano
 *                       (visita todos os vertices uma unica vez).
 * exigir_ciclo == 1  -> procura um CICLO hamiltoniano
 *                       (caminho + aresta de volta do ultimo ao primeiro).
 *
 * Retorna 1 se encontrou e escreve a sequencia de vertices em caminho[0..n-1];
 * retorna 0 caso contrario.
 * Se nos_visitados != NULL, recebe quantos nos da arvore foram explorados.
 */
int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho,
                                      int exigir_ciclo, long long *nos_visitados);

/* ---------- verificacao e saida ---------- */

/* Confere se caminho[0..n-1] e realmente hamiltoniano no grafo g. */
int eh_caminho_hamiltoniano(const Grafo *g, const int *caminho, int exigir_ciclo);

void imprimir_caminho(const char *rotulo, const int *caminho, int n, int exigir_ciclo);

#endif /* HAMILTONIANO_H */