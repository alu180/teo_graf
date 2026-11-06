#ifndef COBERTURA_H
#define COBERTURA_H

#define MAX_VERTICES 64

/*
 * Grafo nao-dirigido representado por matriz de adjacencia.
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
int  total_arestas(const Grafo *g);
void gerar_grafo_aleatorio(Grafo *g, int n, double prob);
void imprimir_grafo(const Grafo *g);

/* ---------- funcoes exigidas pela pratica ---------- */

/* Verifica se o conjunto marcado em selecionado[] cobre TODAS as arestas.
   selecionado[i] == 1  =>  vertice i faz parte da cobertura.
   Retorna 1 se e cobertura valida, 0 caso contrario. */
int eh_cobertura(const Grafo *g, const int *selecionado);

/* Forca bruta com backtracking e poda por limite.
   Escreve a melhor cobertura encontrada em melhor[] e devolve o tamanho dela.
   Se nos_visitados != NULL, recebe quantos nos da arvore foram explorados. */
int cobertura_minima_bt(const Grafo *g, int *melhor, long long *nos_visitados);

/* Heuristica gulosa: escolhe sempre o vertice de maior grau restante.
   Escreve o resultado em selecionado[] e devolve o tamanho da cobertura. */
int cobertura_gulosa(const Grafo *g, int *selecionado);

/* ---------- saida ---------- */
void imprimir_conjunto(const char *rotulo, const int *selecionado, int n);

#endif /* COBERTURA_H */