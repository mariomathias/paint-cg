#ifndef RETA_H
#define RETA_H
#include <stdlib.h>
#include "ponto.h"
typedef struct
{
    Ponto inicio;
    Ponto fim;
    int selecionado;
} Reta;

typedef struct elemento_r
{
    Reta reta;
    struct elemento_r *prox;
} Elemento_r;
Reta cria_reta(Ponto inicio, Ponto fim);
int compara_reta(Reta p, Reta q);
Elemento_r *remover_r(Elemento_r **lista, Reta reta);
int insercao_r_pointer(Elemento_r **lista, Elemento_r *pointer);
Elemento_r **criar_lista_r();
int insercao_r(Elemento_r **lista, Reta reta);
void delecao_lista_r(Elemento_r **lista);

#endif
