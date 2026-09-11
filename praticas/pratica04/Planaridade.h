#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include <stdbool.h>
#include "conectividade.h"

/* Tamanho máximo de grafo aceito pelo teste de subdivisão por força
 * bruta  */
#define PLANARIDADE_MAX_N 10

bool eh_planar_euler(GrafoLista *g);

/* Testa, por força bruta (válido apenas para n <= PLANARIDADE_MAX_N),
 * se o grafo contém uma SUBDIVISÃO de K5 ou de K3,3. */
bool testar_subdivisao_k5_k33(GrafoLista *g);


bool eh_planar(GrafoLista *g);

#endif