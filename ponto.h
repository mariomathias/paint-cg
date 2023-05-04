#ifndef PONTO_H
#define PONTO_H
#include <stdlib.h>
typedef struct
{
    int x;
    int y;
    int selecionado;
} Ponto;

typedef struct elemento_p
{
    Ponto ponto;
    struct elemento_p *prox;
} Elemento_p;

Elemento_p **criar_lista_p();
int insercao_p(Elemento_p **lista, Ponto ponto);
int insercao_p_pointer(Elemento_p **lista, Elemento_p *pointer);
int compara_ponto(Ponto p, Ponto q);
Elemento_p *remover_p(Elemento_p **lista, Ponto ponto);
Ponto cria_ponto(int x, int y);
void print_ponto(Ponto p);
Ponto get_point_list(Elemento_p **lista, int n);
void delecao_lista_p(Elemento_p **lista);
int get_tamanho_lista_p(Elemento_p **lista);
#endif
