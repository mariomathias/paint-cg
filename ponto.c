#include <stdio.h>
#include <stdlib.h>
#include "ponto.h"
Elemento_p **criar_lista_p()
{
    Elemento_p **lista;
    lista = (Elemento_p **)malloc(sizeof(Elemento_p *));
    *lista = NULL;
    return lista;
}

int insercao_p(Elemento_p **lista, Ponto ponto)
{
    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        Elemento_p *novo_elem = (Elemento_p *)malloc(sizeof(Elemento_p));
        novo_elem->ponto = ponto;
        novo_elem->prox = NULL;
        if (*lista == NULL)
        {
            *lista = novo_elem;
            return 1;
        }
        Elemento_p *aux = *lista;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo_elem;
        return 1;
    }
}

Ponto cria_ponto(int x, int y)
{
    Ponto ponto = {x, y, 0};
    return ponto;
}

void print_ponto(Ponto p)
{
    printf("p:%d,%d\n", p.x, p.y);
}

// n: numero de elementos a percorrer, 0 para o primeiro elemento, assim por diante
Ponto get_point_list(Elemento_p **lista, int n)
{
    Ponto vz = {-1, -1};
    if (lista != NULL && *(lista) != NULL)
    {
        int cont = 0;
        Elemento_p *aux = *lista;
        while (*lista != NULL)
        {
            if (cont == n)
            {
                return aux->ponto;
            }
            aux = aux->prox;
        }
        return vz;
    }
    return vz;
}
void delecao_lista(Elemento_p **lista)
{
    if (lista != NULL)
    {
        Elemento_p *aux = *lista;
        while (*lista != NULL)
        {
            aux = *lista;
            *lista = (*lista)->prox;
            free(aux);
        }
    }
}
int get_tamanho_lista_p(Elemento_p **lista)
{
    if (lista == NULL || *(lista) == NULL)
    {
        return 0;
    }
    else
    {
        int cont = 0;
        Elemento_p *aux = *lista;
        while (aux != NULL)
        {
            aux = aux->prox;
            cont++;
        }
        return cont;
    }
}
