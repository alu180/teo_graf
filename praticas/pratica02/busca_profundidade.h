#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h" 

/* ===================== Pilha (LIFO) para dfs iterativa ===================== */

typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

Pilha *criar_pilha(int capacidade);
int pilha_vazia(Pilha *p);
void empilhar(Pilha *p, int valor);
int desempilhar(Pilha *p);
void liberar_pilha(Pilha *p);

/* ===================== dfs ===================== */


void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *entrada, int *saida, int *tempo);

/* dfs iterativa equivalente, usando a Pilha em vez de recursao. */
void dfs_iterativa(GrafoLista *g, int origem, int *visitado);

/* ===================== Aplicacoes de busca po bfs ===================== */

int eh_bipartido(GrafoLista *g);      
int contar_componentes(GrafoLista *g); 
int tem_ciclo(GrafoLista *g);          

#endif