#include <stdbool.h>
#include <string.h>
#include "planaridade.h"

/* ---------- Fórmula de Euler ---------- */

bool eh_planar_euler(GrafoLista *g) {
    int n = g->numVertices;
    int m = contar_arestas(g);

    if (n < 3) {
        return true; /* grafos triviais (0, 1 ou 2 vértices) são sempre planares */
    }

    return m <= 3 * n - 6;
}

/* ---------- Redução do grafo (supressão de vértices de grau 2) ---------- */

static void construir_matriz(GrafoLista *g, bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    int n = g->numVertices;
    memset(m, 0, sizeof(bool) * PLANARIDADE_MAX_N * PLANARIDADE_MAX_N);
    for (int u = 0; u < n; u++) {
        for (NoAdj *aresta = g->adj[u]; aresta != NULL; aresta = aresta->prox) {
            m[u][aresta->destino] = true;
        }
    }
}

static int grau_matriz(bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N], int n, bool ativo[PLANARIDADE_MAX_N], int v) {
    if (!ativo[v]) return -1;
    int grau = 0;
    for (int j = 0; j < n; j++) {
        if (ativo[j] && m[v][j]) grau++;
    }
    return grau;
}

/* Uma passada: colapsa todo vértice ativo de grau exatamente 2,
 * conectando diretamente seus dois vizinhos. Retorna true se algo
 * mudou (para repetir até estabilizar). */
static bool suprimir_grau2(bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N], int n, bool ativo[PLANARIDADE_MAX_N]) {
    bool alterou = false;

    for (int v = 0; v < n; v++) {
        if (!ativo[v]) continue;

        int vizinhos[3]; /* tamanho 3: precisamos detectar grau > 2, não só armazenar 2 */
        int grau = 0;
        for (int j = 0; j < n && grau < 3; j++) {
            if (ativo[j] && m[v][j]) vizinhos[grau++] = j;
        }
        if (grau != 2) continue;

        int a = vizinhos[0], b = vizinhos[1];
        if (a != b) {
            /* colapsa a cadeia a - v - b em uma única aresta a - b */
            m[a][b] = m[b][a] = true;
        }
        m[v][a] = m[a][v] = false;
        m[v][b] = m[b][v] = false;
        ativo[v] = false;
        alterou = true;
    }

    return alterou;
}

static void reduzir_grafo(bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N], int n, bool ativo[PLANARIDADE_MAX_N]) {
    for (int i = 0; i < n; i++) ativo[i] = true;
    (void)grau_matriz; /* utilitário disponível para depuração, evita warning de não uso */
    while (suprimir_grau2(m, n, ativo)) {
        /* repete até não haver mais vértices de grau 2 para colapsar */
    }
}

/* ---------- Busca de K5 (clique de 5) no grafo reduzido ---------- */

static bool eh_clique(int *verts, int k, bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            if (!m[verts[i]][verts[j]]) return false;
        }
    }
    return true;
}

static bool escolher_k5(int *vs, int cnt, int *atual, int inicio, int prof,
                         bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    if (prof == 5) {
        return eh_clique(atual, 5, m);
    }
    for (int i = inicio; i < cnt; i++) {
        atual[prof] = vs[i];
        if (escolher_k5(vs, cnt, atual, i + 1, prof + 1, m)) return true;
    }
    return false;
}

static bool existe_k5(bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N], bool ativo[PLANARIDADE_MAX_N], int n) {
    int vs[PLANARIDADE_MAX_N], cnt = 0;
    for (int i = 0; i < n; i++) if (ativo[i]) vs[cnt++] = i;
    if (cnt < 5) return false;

    int atual[5];
    return escolher_k5(vs, cnt, atual, 0, 0, m);
}

/* ---------- Busca de K3,3 (bipartido completo 3+3) no grafo reduzido ---------- */

static bool eh_biclique_completo(int *A, int *B, bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!m[A[i]][B[j]]) return false;
        }
    }
    return true;
}

/* Dado um conjunto de 6 vértices, tenta todas as formas de dividi-lo
 * em dois grupos de 3 e verifica se algum particionamento forma um
 * K3,3 completo. */
static bool alguma_particao_k33(int *seis, bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    /* Fixa seis[0] no grupo A; escolhe os outros 2 membros de A entre
     * os 5 restantes (C(5,2) = 10 combinações), o resto vai para B. */
    int restantes[5];
    int idx = 0;
    for (int i = 1; i < 6; i++) restantes[idx++] = seis[i];

    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            int A[3] = { seis[0], restantes[i], restantes[j] };
            int B[3];
            int bidx = 0;
            for (int k = 0; k < 5; k++) {
                if (k != i && k != j) B[bidx++] = restantes[k];
            }
            if (eh_biclique_completo(A, B, m)) return true;
        }
    }
    return false;
}

static bool escolher_seis(int *vs, int cnt, int *atual, int inicio, int prof,
                           bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N]) {
    if (prof == 6) {
        return alguma_particao_k33(atual, m);
    }
    for (int i = inicio; i < cnt; i++) {
        atual[prof] = vs[i];
        if (escolher_seis(vs, cnt, atual, i + 1, prof + 1, m)) return true;
    }
    return false;
}

static bool existe_k33(bool m[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N], bool ativo[PLANARIDADE_MAX_N], int n) {
    int vs[PLANARIDADE_MAX_N], cnt = 0;
    for (int i = 0; i < n; i++) if (ativo[i]) vs[cnt++] = i;
    if (cnt < 6) return false;

    int atual[6];
    return escolher_seis(vs, cnt, atual, 0, 0, m);
}

/* ---------- Funções públicas ---------- */

bool testar_subdivisao_k5_k33(GrafoLista *g) {
    int n = g->numVertices;
    if (n > PLANARIDADE_MAX_N) return false; /* fora do escopo da força bruta */

    bool matriz[PLANARIDADE_MAX_N][PLANARIDADE_MAX_N];
    bool ativo[PLANARIDADE_MAX_N];

    construir_matriz(g, matriz);
    reduzir_grafo(matriz, n, ativo);

    if (existe_k5(matriz, ativo, n)) return true;
    if (existe_k33(matriz, ativo, n)) return true;
    return false;
}

bool eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g)) {
        return false; /* violou condição necessária: com certeza não é planar */
    }

    if (g->numVertices <= PLANARIDADE_MAX_N) {
        if (testar_subdivisao_k5_k33(g)) {
            return false; /* contém subdivisão de K5 ou K3,3: não é planar */
        }
    }

    return true;
}