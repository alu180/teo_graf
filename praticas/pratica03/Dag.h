#ifndef DAG_H
#define DAG_H

/* Nó de uma lista de adjacência (aresta direcionada origem -> dest) */
typedef struct Node {
    int dest;
    struct Node *next;
} Node;

/* Grafo direcionado representado por lista de adjacência */
typedef struct {
    int num_vertices;
    Node **adj; /* adj[v] = lista de vizinhos de v */
} GrafoLista;

/* Construção / destruição do grafo */
GrafoLista *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista *g, int origem, int destino);
void liberar_grafo(GrafoLista *g);


/* Algoritmo de Kahn (BFS com grau de entrada).
 * Retorna um array malloc com a ordenação topológica e escreve o
 * tamanho em *tamanho. Se o grafo tiver ciclo, retorna NULL e
 * *tamanho = 0. */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);

/* Variante por DFS (empilha na saída da DFS).
 * Mesma convenção de retorno que a versão de Kahn. */
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);

/* Verifica se o grafo é acíclico (DAG). Retorna 1 se for DAG, 0 caso
 * contrário. */
int eh_dag(GrafoLista *g);

#endif /* DAG_H */