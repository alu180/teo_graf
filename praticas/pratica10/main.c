#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

/* ==================================================================== */
/*  PARTE 1 - CAMINHO E CICLO HAMILTONIANO                               */
/* ==================================================================== */

static void testar_hamiltoniano(const char *nome, const Grafo *g,
                                int esperado_caminho, int esperado_ciclo) {
    int caminho[MAX_VERTICES];
    long long nos;
    int achou;

    printf("--------------------------------------------------------\n");
    printf("%s\n", nome);
    imprimir_grafo(g);

    /* caminho hamiltoniano */
    achou = encontrar_caminho_hamiltoniano_bt(g, caminho, 0, &nos);
    printf("  Caminho hamiltoniano : %s  (nos explorados = %lld)\n",
           achou ? "SIM" : "NAO", nos);
    if (achou) {
        imprimir_caminho("    caminho =", caminho, g->n, 0);
        printf("    valido? %s\n", eh_caminho_hamiltoniano(g, caminho, 0) ? "SIM" : "NAO");
    }
    printf("    esperado = %s -> %s\n", esperado_caminho ? "SIM" : "NAO",
           (achou == esperado_caminho) ? "OK" : "FALHOU");

    /* ciclo hamiltoniano */
    achou = encontrar_caminho_hamiltoniano_bt(g, caminho, 1, &nos);
    printf("  Ciclo hamiltoniano   : %s  (nos explorados = %lld)\n",
           achou ? "SIM" : "NAO", nos);
    if (achou) {
        imprimir_caminho("    ciclo   =", caminho, g->n, 1);
        printf("    valido? %s\n", eh_caminho_hamiltoniano(g, caminho, 1) ? "SIM" : "NAO");
    }
    printf("    esperado = %s -> %s\n", esperado_ciclo ? "SIM" : "NAO",
           (achou == esperado_ciclo) ? "OK" : "FALHOU");
    printf("\n");
}

static void parte1_hamiltoniano(void) {
    Grafo g;
    int i, j;

    printf("========================================================\n");
    printf(" PARTE 1 - CAMINHO E CICLO HAMILTONIANO (backtracking)\n");
    printf("========================================================\n");

    /* Quadrado 0-1-3-2-0: tem caminho e tem ciclo */
    criar_grafo(&g, 4);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 1, 3);
    inserir_aresta(&g, 3, 2);
    inserir_aresta(&g, 2, 0);
    testar_hamiltoniano("TESTE 1 - Quadrado (ciclo de 4 vertices)", &g, 1, 1);

    /* Caminho 0-1-2-3: tem caminho, mas falta a aresta de volta 3->0 */
    criar_grafo(&g, 4);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 1, 2);
    inserir_aresta(&g, 2, 3);
    testar_hamiltoniano("TESTE 2 - Caminho simples com 4 vertices", &g, 1, 0);

    /* Estrela: sair de uma folha obriga a voltar pelo centro -> nao tem nenhum dos dois */
    criar_grafo(&g, 5);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 0, 2);
    inserir_aresta(&g, 0, 3);
    inserir_aresta(&g, 0, 4);
    testar_hamiltoniano("TESTE 3 - Estrela (centro no vertice 0)", &g, 0, 0);

    /* Grafo completo K5: qualquer ordem serve */
    criar_grafo(&g, 5);
    for (i = 0; i < 5; i++)
        for (j = i + 1; j < 5; j++)
            inserir_aresta(&g, i, j);
    testar_hamiltoniano("TESTE 4 - Grafo completo K5", &g, 1, 1);

    /* Desconexo: duas arestas separadas, impossivel ligar as partes */
    criar_grafo(&g, 4);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 2, 3);
    testar_hamiltoniano("TESTE 5 - Grafo desconexo", &g, 0, 0);
}

/* ==================================================================== */
/*  PARTE 2 - REDUCAO CICLO HAMILTONIANO -> TSP                          */
/* ==================================================================== */

static void testar_reducao(const char *nome, const Grafo *g) {
    GrafoPonderado gp;
    int rota[MAX_VERTICES];
    int custo, tem_ciclo;
    int caminho[MAX_VERTICES];

    printf("--------------------------------------------------------\n");
    printf("%s\n", nome);

    grafo_para_tsp(g, &gp);
    imprimir_matriz_custos(&gp);

    custo = tsp_forca_bruta(&gp, rota, NULL);
    imprimir_rota("  Melhor rota do TSP:", rota, gp.n, custo);

    tem_ciclo = encontrar_caminho_hamiltoniano_bt(g, caminho, 1, NULL);

    printf("  custo = %d, n = %d  ->  %s\n", custo, gp.n,
           (custo == gp.n) ? "o grafo TEM ciclo hamiltoniano"
                           : "o grafo NAO tem ciclo hamiltoniano");
    printf("  conferindo com o backtracking: %s -> %s\n",
           tem_ciclo ? "TEM" : "NAO TEM",
           ((custo == gp.n) == (tem_ciclo != 0)) ? "OK" : "FALHOU");
    printf("\n");
}

static void parte2_reducao(void) {
    Grafo g;

    printf("========================================================\n");
    printf(" PARTE 2 - REDUCAO: peso 1 se a aresta existe, 2 se nao\n");
    printf("========================================================\n");
    printf(" Se o TSP devolver custo igual a n, todas as arestas usadas\n");
    printf(" custaram 1, ou seja, todas existem no grafo original: ha\n");
    printf(" ciclo hamiltoniano. Custo maior que n significa que nao ha.\n\n");

    /* Quadrado: tem ciclo -> custo esperado = 4 */
    criar_grafo(&g, 4);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 1, 3);
    inserir_aresta(&g, 3, 2);
    inserir_aresta(&g, 2, 0);
    testar_reducao("CASO A - Quadrado", &g);

    /* Estrela: nao tem ciclo -> custo esperado > 5 */
    criar_grafo(&g, 5);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 0, 2);
    inserir_aresta(&g, 0, 3);
    inserir_aresta(&g, 0, 4);
    testar_reducao("CASO B - Estrela", &g);
}

/* ==================================================================== */
/*  PARTE 3 - EXPERIMENTO: TEMPO E CUSTO PARA n = 5, 8, 10, 12           */
/* ==================================================================== */

static void parte3_experimento(void) {
    int tamanhos[] = {5, 8, 10, 12};
    int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    int k;

    printf("========================================================\n");
    printf(" PARTE 3 - EXPERIMENTO TSP (instancias aleatorias, custos 1..100)\n");
    printf("========================================================\n");
    printf("  n |  permutacoes | FB custo | FB tempo(s) | VMP custo | VMP tempo(s) | VMP erro | INS custo | INS tempo(s) | INS erro\n");
    printf("----+--------------+----------+-------------+-----------+--------------+----------+-----------+--------------+---------\n");

    srand(42);      /* semente fixa: o experimento e reproduzivel */

    for (k = 0; k < qtd; k++) {
        GrafoPonderado gp;
        int rota_fb[MAX_VERTICES], rota_vmp[MAX_VERTICES], rota_ins[MAX_VERTICES];
        int n = tamanhos[k];
        int custo_fb, custo_vmp, custo_ins;
        double t_fb, t_vmp, t_ins;
        double erro_vmp, erro_ins;
        long long perms;
        clock_t ini, fim;

        gerar_tsp_aleatorio(&gp, n, 100);

        ini = clock();
        custo_fb = tsp_forca_bruta(&gp, rota_fb, &perms);
        fim = clock();
        t_fb = (double)(fim - ini) / CLOCKS_PER_SEC;

        ini = clock();
        custo_vmp = tsp_vizinho_mais_proximo(&gp, rota_vmp);
        fim = clock();
        t_vmp = (double)(fim - ini) / CLOCKS_PER_SEC;

        ini = clock();
        custo_ins = tsp_insercao(&gp, rota_ins);
        fim = clock();
        t_ins = (double)(fim - ini) / CLOCKS_PER_SEC;

        erro_vmp = 100.0 * (custo_vmp - custo_fb) / custo_fb;
        erro_ins = 100.0 * (custo_ins - custo_fb) / custo_fb;

        printf("%3d | %12lld | %8d | %11.6f | %9d | %12.6f | %7.2f%% | %9d | %12.6f | %7.2f%%\n",
               n, perms,
               custo_fb, t_fb,
               custo_vmp, t_vmp, erro_vmp,
               custo_ins, t_ins, erro_ins);
    }

    printf("\nLegenda:\n");
    printf("  FB  = forca bruta  (testa as (n-1)! permutacoes, sempre acha o otimo)\n");
    printf("  VMP = vizinho mais proximo (heuristica gulosa)\n");
    printf("  INS = insercao do mais proximo (heuristica construtiva)\n");
    printf("  erro = quanto a heuristica ficou acima do custo otimo, em %%\n");
}

/* ==================================================================== */

int main(void) {
    parte1_hamiltoniano();
    parte2_reducao();
    parte3_experimento();
    return 0;
}