#ifndef RETA_H
#define RETA_H
#include <stdlib.h>
#include "ponto.h"
typedef struct
{
    Ponto inicio;
    Ponto fim;
} Reta;

typedef struct elemento_r
{
    Reta reta;
    Elemento_r *prox;
} Elemento_r;

Elemento_r **criar_lista_r();
int insercao_r(Elemento_r **lista, Reta reta);

#endif