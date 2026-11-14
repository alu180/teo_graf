#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

/* ==================================================================== */
/*  TESTES COM GRAFOS CONHECIDOS                                         */
/* ==================================================================== */

static void testar(const char *nome, const Grafo *g, int esperado) {
    int bt[MAX_VERTICES];
    int gl[MAX_VERTICES];
    long long nos;
    int tam_bt, tam_gl;

    printf("--------------------------------------------------------\n");
    printf("%s\n", nome);
    imprimir_grafo(g);

    tam_bt = cobertura_minima_bt(g, bt, &nos);
    tam_gl = cobertura_gulosa(g, gl);

    printf("  Backtracking : tamanho = %d  (nos explorados = %lld)\n", tam_bt, nos);
    imprimir_conjunto("    cobertura =", bt, g->n);
    printf("    eh_cobertura? %s\n", eh_cobertura(g, bt) ? "SIM" : "NAO");

    printf("  Guloso       : tamanho = %d\n", tam_gl);
    imprimir_conjunto("    cobertura =", gl, g->n);
    printf("    eh_cobertura? %s\n", eh_cobertura(g, gl) ? "SIM" : "NAO");

    printf("  Esperado (otimo) = %d  -> %s\n", esperado,
           (tam_bt == esperado) ? "OK" : "FALHOU");
    if (tam_gl > tam_bt)
        printf("  >> o guloso ficou %d vertice(s) acima do otimo\n", tam_gl - tam_bt);
    printf("\n");
}

static void bateria_de_testes(void) {
    Grafo g;

    printf("========================================================\n");
    printf(" PARTE 1 - TESTES COM GRAFOS CONHECIDOS\n");
    printf("========================================================\n");

    /* Quadrado 0-1-3-2-0: otimo = 2 ({0,3} ou {1,2}) */
    criar_grafo(&g, 4);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 0, 2);
    inserir_aresta(&g, 1, 3);
    inserir_aresta(&g, 2, 3);
    testar("TESTE 1 - Quadrado (ciclo de 4 vertices)", &g, 2);

    /* Caminho 0-1-2-3-4: otimo = 2 ({1,3}) */
    criar_grafo(&g, 5);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 1, 2);
    inserir_aresta(&g, 2, 3);
    inserir_aresta(&g, 3, 4);
    testar("TESTE 2 - Caminho com 5 vertices", &g, 2);

    /* Estrela: o centro 0 sozinho cobre tudo -> otimo = 1 */
    criar_grafo(&g, 5);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 0, 2);
    inserir_aresta(&g, 0, 3);
    inserir_aresta(&g, 0, 4);
    testar("TESTE 3 - Estrela (centro no vertice 0)", &g, 1);

    /* Triangulo: qualquer 1 vertice deixa 1 aresta solta -> otimo = 2 */
    criar_grafo(&g, 3);
    inserir_aresta(&g, 0, 1);
    inserir_aresta(&g, 1, 2);
    inserir_aresta(&g, 2, 0);
    testar("TESTE 4 - Triangulo", &g, 2);

    /* Grafo completo K5: precisa de n-1 = 4 vertices */
    criar_grafo(&g, 5);
    {
        int i, j;
        for (i = 0; i < 5; i++)
            for (j = i + 1; j < 5; j++)
                inserir_aresta(&g, i, j);
    }
    testar("TESTE 5 - Grafo completo K5", &g, 4);

    /* Grafo sem arestas: cobertura vazia */
    criar_grafo(&g, 4);
    testar("TESTE 6 - Grafo sem arestas", &g, 0);

    /* Bipartido: os vertices 0..3 de um lado, 4..6 do outro.
       O vertice 4 tem grau 4; os vertices 5 e 6 tem grau 2 cada.
       Cobertura minima = {4,5,6}, com 3 vertices. */
    criar_grafo(&g, 7);
    inserir_aresta(&g, 4, 0); inserir_aresta(&g, 4, 1);
    inserir_aresta(&g, 4, 2); inserir_aresta(&g, 4, 3);
    inserir_aresta(&g, 5, 0); inserir_aresta(&g, 5, 1);
    inserir_aresta(&g, 6, 2); inserir_aresta(&g, 6, 3);
    testar("TESTE 7 - Bipartido com graus desiguais", &g, 3);
}

/* ==================================================================== */
/*  EXPERIMENTO: TEMPO DA FORCA BRUTA x HEURISTICA                       */
/* ==================================================================== */

#define REPETICOES 10   /* grafos aleatorios por valor de n */

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    int k;

    printf("========================================================\n");
    printf(" PARTE 2 - EXPERIMENTO (%d grafos aleatorios por n, p=0.5)\n", REPETICOES);
    printf("========================================================\n");
    printf("  n | BT(tam) | BT(tempo s) | BT(nos) | GL(tam) | GL(tempo s) | GL otimo\n");
    printf("----+---------+-------------+---------+---------+-------------+---------\n");

    srand(42);   /* semente fixa: o experimento e reproduzivel */

    for (k = 0; k < qtd; k++) {
        int n = tamanhos[k];
        int r;
        double t_bt = 0.0, t_gl = 0.0;
        long long nos_bt = 0;
        int soma_bt = 0, soma_gl = 0, acertos = 0;

        for (r = 0; r < REPETICOES; r++) {
            Grafo g;
            int bt[MAX_VERTICES], gl[MAX_VERTICES];
            long long nos;
            clock_t ini, fim;
            int tam_bt, tam_gl;

            gerar_grafo_aleatorio(&g, n, 0.5);

            /* forca bruta com backtracking e poda */
            ini = clock();
            tam_bt = cobertura_minima_bt(&g, bt, &nos);
            fim = clock();
            t_bt += (double)(fim - ini) / CLOCKS_PER_SEC;
            nos_bt += nos;

            /* heuristica gulosa */
            ini = clock();
            tam_gl = cobertura_gulosa(&g, gl);
            fim = clock();
            t_gl += (double)(fim - ini) / CLOCKS_PER_SEC;

            soma_bt += tam_bt;
            soma_gl += tam_gl;
            if (tam_gl == tam_bt) acertos++;
        }

        printf("%3d | %7.1f | %11.6f | %7lld | %7.1f | %11.6f | %2d/%d\n",
               n,
               (double)soma_bt / REPETICOES, t_bt / REPETICOES,
               nos_bt / REPETICOES,
               (double)soma_gl / REPETICOES, t_gl / REPETICOES,
               acertos, REPETICOES);
    }

    printf("\nLegenda:\n");
    printf("  BT = forca bruta com backtracking e poda por limite\n");
    printf("  GL = heuristica gulosa (maior grau primeiro)\n");
    printf("  tam      = tamanho medio da cobertura devolvida\n");
    printf("  nos      = media de nos da arvore de busca explorados pelo BT\n");
    printf("  GL otimo = em quantos dos %d grafos o guloso achou o otimo\n", REPETICOES);
}

int main(void) {
    bateria_de_testes();
    experimento();
    return 0;
}