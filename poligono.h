#ifndef POLIGONO_H
#define POLIGONO_H
#include <stdlib.h>
#include "ponto.h"

typedef struct
{
    Elemento_p **vertices;
    int qtd_vertices;
    int selecionado;
} Poligono;

typedef struct elemento_pol
{
    Poligono poligono;
    struct elemento_pol *prox;
} Elemento_pol;

Poligono cria_poligono(Elemento_p **vertices);
Elemento_pol **criar_lista_pol();
int insercao_pol(Elemento_pol **lista, Poligono poligono);
void add_vertice_pol(Poligono *p, Ponto vertice);
Ponto get_vertice_pol(Poligono p, int k);
void deleta_pol(Poligono *p);
int compara_pol(Poligono p, Poligono q);
Elemento_pol *remover_pol(Elemento_pol **lista, Poligono poli);
int insercao_pol_pointer(Elemento_pol **lista, Elemento_pol *pointer);
void delecao_lista_pol(Elemento_pol **lista);

#endif
