#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main(void) {
    printf("=== Grafo com Matriz de Adjacencia ===\n");
    GrafoMatriz *gm = criar_grafo_matriz(5);
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 2);

    printf("Matriz de adjacencia:\n");
    imprimir_grafo_matriz(gm);

    printf("Grau do vertice 0: %d\n", grau_matriz(gm, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 1) ? "Sim" : "Nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 3) ? "Sim" : "Nao");

    remover_aresta_matriz(gm, 0, 1);
    printf("Apos remover aresta 0-1, grau do vertice 0: %d\n", grau_matriz(gm, 0));

    liberar_grafo_matriz(gm);

    printf("\n=== Grafo com Lista de Adjacencia ===\n");
    GrafoLista *gl = criar_grafo_lista(5);
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 2);

    printf("Lista de adjacencia:\n");
    imprimir_grafo_lista(gl);

    printf("Grau do vertice 0: %d\n", grau_lista(gl, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim" : "Nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 3) ? "Sim" : "Nao");

    remover_aresta_lista(gl, 0, 1);
    printf("Apos remover aresta 0-1, grau do vertice 0: %d\n", grau_lista(gl, 0));

    liberar_grafo_lista(gl);

    return 0;
}