#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/*
 * Instancia do TSP (Caixeiro Viajante): grafo COMPLETO e PONDERADO,
 * guardado como matriz de custos.
 * custo[i][j] = preco de ir da cidade i para a cidade j.
 * A matriz e simetrica e custo[i][i] == 0.
 *
 * Em todas as funcoes a ORIGEM e fixa no vertice 0 e a rota devolvida
 * guarda apenas a ordem de visita (o retorno a origem fica implicito).
 */
typedef struct {
    int n;                                    /* quantidade de cidades */
    int custo[MAX_VERTICES][MAX_VERTICES];    /* matriz de custos      */
} GrafoPonderado;

/* ---------- construcao e utilidades ---------- */

/* Instancia aleatoria: custos simetricos sorteados em [1, custo_max]. */
void gerar_tsp_aleatorio(GrafoPonderado *gp, int n, int custo_max);

/* Custo total do ciclo rota[0..n-1] somando a volta ate rota[0]. */
int custo_rota(const GrafoPonderado *gp, const int *rota);

void imprimir_matriz_custos(const GrafoPonderado *gp);
void imprimir_rota(const char *rotulo, const int *rota, int n, int custo);

/* ---------- funcoes exigidas pela pratica ---------- */

/*
 * REDUCAO Ciclo Hamiltoniano -> TSP.
 * Converte um grafo nao ponderado em instancia do TSP:
 *   peso 1 para as arestas que existem, 2 para as que nao existem.
 * Consequencia: o TSP devolve custo exatamente n se, e somente se,
 * o grafo original possui ciclo hamiltoniano.
 */
void grafo_para_tsp(const Grafo *g, GrafoPonderado *gp);

/*
 * Forca bruta: fixa a origem no vertice 0 e gera as (n-1)! permutacoes
 * dos demais vertices, guardando a de menor custo.
 * Devolve o custo otimo e escreve a melhor ordem em rota[0..n-1].
 * Se permutacoes != NULL, recebe quantas permutacoes foram avaliadas.
 */
int tsp_forca_bruta(const GrafoPonderado *gp, int *rota, long long *permutacoes);

/*
 * Heuristica construtiva do vizinho mais proximo: partindo da origem,
 * vai sempre para a cidade ainda nao visitada de menor custo.
 * Devolve o custo do ciclo obtido.
 */
int tsp_vizinho_mais_proximo(const GrafoPonderado *gp, int *rota);

/*
 * Heuristica de insercao do mais proximo: comeca com um ciclo de duas
 * cidades e, a cada passo, escolhe a cidade de fora mais proxima do ciclo
 * e a insere na posicao que menos aumenta o custo total.
 * Devolve o custo do ciclo obtido.
 */
int tsp_insercao(const GrafoPonderado *gp, int *rota);

#endif /* TPS_H */