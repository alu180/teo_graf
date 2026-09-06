#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

int main(void) {
    /* ---------- Grafo A: caminho 0-1-2-3-4 (sem ciclo) ---------- */
    printf("=== Grafo A: 0-1-2-3-4 ===\n");
    GrafoLista *a = criar_grafo_lista(5);
    inserir_aresta_lista(a, 0, 1);
    inserir_aresta_lista(a, 1, 2);
    inserir_aresta_lista(a, 2, 3);
    inserir_aresta_lista(a, 3, 4);
    imprimir_grafo_lista(a);

    int dist[5], pred[5];
    bfs(a, 0, dist, pred);
    printf("BFS a partir do vertice 0:\n");
    for (int i = 0; i < 5; i++) {
        printf("  vertice %d -> dist = %d, pred = %d\n", i, dist[i], pred[i]);
    }

    int visitado[5] = {0}, entrada[5], saida[5], tempo = 0;
    dfs_recursiva(a, 0, visitado, entrada, saida, &tempo);
    printf("DFS recursiva a partir do vertice 0 (tempos de entrada/saida):\n");
    for (int i = 0; i < 5; i++) {
        printf("  vertice %d -> entrada = %d, saida = %d\n", i, entrada[i], saida[i]);
    }

    int visitado_iter[5] = {0};
    dfs_iterativa(a, 0, visitado_iter);
    printf("DFS iterativa a partir do vertice 0, visitados: ");
    for (int i = 0; i < 5; i++) printf("%d ", visitado_iter[i]);
    printf("\n");

    printf("Tem ciclo? %s\n", tem_ciclo(a) ? "Sim" : "Nao");
    printf("E bipartido? %s\n", eh_bipartido(a) ? "Sim" : "Nao");
    printf("Numero de componentes conexos: %d\n", contar_componentes(a));
    liberar_grafo_lista(a);

    /* ---------- Grafo B: triangulo 0-1-2-0 (ciclo impar, nao bipartido) ---------- */
    printf("\n=== Grafo B: triangulo 0-1-2-0 ===\n");
    GrafoLista *b = criar_grafo_lista(3);
    inserir_aresta_lista(b, 0, 1);
    inserir_aresta_lista(b, 1, 2);
    inserir_aresta_lista(b, 2, 0);
    imprimir_grafo_lista(b);

    printf("Tem ciclo? %s\n", tem_ciclo(b) ? "Sim" : "Nao");
    printf("E bipartido? %s\n", eh_bipartido(b) ? "Sim" : "Nao");
    printf("Numero de componentes conexos: %d\n", contar_componentes(b));
    liberar_grafo_lista(b);

    /* ---------- Grafo C: ciclo par 0-1-2-3-0 (tem ciclo, mas e bipartido) ---------- */
    printf("\n=== Grafo C: ciclo 0-1-2-3-0 ===\n");
    GrafoLista *c = criar_grafo_lista(4);
    inserir_aresta_lista(c, 0, 1);
    inserir_aresta_lista(c, 1, 2);
    inserir_aresta_lista(c, 2, 3);
    inserir_aresta_lista(c, 3, 0);
    imprimir_grafo_lista(c);

    printf("Tem ciclo? %s\n", tem_ciclo(c) ? "Sim" : "Nao");
    printf("E bipartido? %s\n", eh_bipartido(c) ? "Sim" : "Nao");
    printf("Numero de componentes conexos: %d\n", contar_componentes(c));
    liberar_grafo_lista(c);

    /* ---------- Grafo D: desconexo, com 4 componentes ---------- */
    printf("\n=== Grafo D: desconexo (0-1, 2-3, 4 e 5 isolados) ===\n");
    GrafoLista *d = criar_grafo_lista(6);
    inserir_aresta_lista(d, 0, 1);
    inserir_aresta_lista(d, 2, 3);
    imprimir_grafo_lista(d);

    printf("Tem ciclo? %s\n", tem_ciclo(d) ? "Sim" : "Nao");
    printf("E bipartido? %s\n", eh_bipartido(d) ? "Sim" : "Nao");
    printf("Numero de componentes conexos: %d\n", contar_componentes(d));
    liberar_grafo_lista(d);

    return 0;
}