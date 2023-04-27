#ifndef PONTO_H
#define PONTO_H
#include <stdlib.h>
typedef struct
{
    float x;
    float y;
} Ponto;

typedef struct elemento_p
{
    Ponto ponto;
    Elemento_p *prox;
} Elemento_p;

Elemento_p **criar_lista_p();
int insercao_p(Elemento_p **lista, Ponto ponto);

#endif
