#include <stdlib.h>
#include <stdio.h>
#include "poligono.h"
#include "ponto.h"
Elemento_pol **criar_lista_pol()
{
    Elemento_pol **lista;
    lista = (Elemento_pol **)malloc(sizeof(Elemento_pol *));
    *lista = NULL;
    return lista;
}

Poligono cria_poligono(Elemento_p **vertices)
{
    Poligono p = {vertices, 0};
    return p;
}
int insercao_pol(Elemento_pol **lista, Poligono p)
{
    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        Elemento_pol *novo_elem = (Elemento_pol *)malloc(sizeof(Elemento_pol));
        novo_elem->poligono = p;
        novo_elem->prox = NULL;
        if (*lista == NULL)
        {
            *lista = novo_elem;
            return 1;
        }
        Elemento_pol *aux = *lista;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo_elem;
        return 1;
    }
}

void add_vertice_pol(Poligono *p, Ponto vertice)
{
    insercao_p(p->vertices, vertice);
    p->qtd_vertices++;
}

Ponto get_vertice_pol(Poligono p, int k)
{
    if (k < 0 || k >= p.qtd_vertices)
    {
        Ponto vz = {-1, -1};
        return vz;
    }
    else
    {
        return get_point_list(p.vertices, k);
    }
}
void deleta_pol(Poligono *p)
{
    delecao_lista(p->vertices);
    printf("aa1\n");
    p->vertices = NULL;
    p->qtd_vertices = 0;
    free(p);
}
