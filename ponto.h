#ifndef PONTO_H
#define PONTO_H
#include <stdlib.h>
typedef struct
{
    int x;
    int y;
} Ponto;

typedef struct elemento_p
{
    Ponto ponto;
    struct elemento_p *prox;
} Elemento_p;

Elemento_p **criar_lista_p();
int insercao_p(Elemento_p **lista, Ponto ponto);
Ponto cria_ponto(int x, int y);
void print_ponto(Ponto p);
#endif
