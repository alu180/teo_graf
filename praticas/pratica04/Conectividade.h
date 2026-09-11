#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#include <stdbool.h>

/* Nó de uma lista de adjacência */
typedef struct NoAdj {
    int destino;
    struct NoAdj *prox;
} NoAdj;

/* Grafo não-dirigido representado por lista de adjacência */
typedef struct {
    int numVertices;
    NoAdj **adj; /* vetor de listas, tamanho numVertices */
} GrafoLista;

/* Representa uma aresta (u, v) */
typedef struct {
    int u;
    int v;
} Aresta;

/* ---------- Construção / utilitários ---------- */

GrafoLista *criar_grafo_lista(int numVertices);
void adicionar_aresta(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);
int contar_arestas(const GrafoLista *g);
int grau_vertice(const GrafoLista *g, int v);

/* ---------- Pontos de articulação (algoritmo de Tarjan) ----------
 * dfs_articulacoes: busca em profundidade que preenche descoberta[] e
 * low[] e marca em eh_articulacao[] os vértices de corte.
 *   - raiz da DFS é articulação se tiver 2+ filhos na árvore de busca;
 *   - vértice interno u é articulação se algum filho w satisfizer
 *     low[w] >= descoberta[u] (nenhum atalho de volta passa "por cima"
 *     de u).
 */
void dfs_articulacoes(GrafoLista *g, int u, int pai,
                       bool visitado[], int descoberta[], int low[],
                       bool eh_articulacao[], int *tempo);

/* Executa dfs_articulacoes a partir de todos os componentes do grafo
 * e devolve, em articulacoes[], os vértices de corte encontrados.
 * Retorna a quantidade de articulações. */
int encontrar_articulacoes(GrafoLista *g, int articulacoes[]);

/* ---------- Pontes / arestas de corte ----------
 * (u, w) é ponte quando low[w] > descoberta[u] (condição ESTRITA:
 * diferente da articulação, aqui não vale "igual", pois se
 * low[w] == descoberta[u] ainda existe um caminho alternativo de volta
 * até u, então a aresta não é de corte).
 */
int detectar_pontes(GrafoLista *g, Aresta pontes[]);

#endif