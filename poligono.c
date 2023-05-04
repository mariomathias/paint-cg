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
    Poligono p = {vertices, 0, 0};
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
    delecao_lista_p(p->vertices);
    printf("aa1\n");
    p->vertices = NULL;
    p->qtd_vertices = 0;
    free(p);
}

void delecao_lista_pol(Elemento_pol **lista)
{
    if (lista != NULL)
    {
        Elemento_pol *aux = *lista;
        while (*lista != NULL)
        {
            aux = *lista;
            *lista = (*lista)->prox;
            free(aux);
        }
    }
    *lista = NULL;
}

int compara_pol(Poligono p, Poligono q)
{

    int a = get_tamanho_lista_p(p.vertices);
    int b = get_tamanho_lista_p(q.vertices);
    if (a != b)
    {
        return 0;
    }
    int cont = 0;
    Elemento_p *aux1 = (*p.vertices);
    Elemento_p *aux2 = (*q.vertices);
    while (aux1 && aux2)
    {
        if (compara_ponto(aux1->ponto, aux2->ponto))
        {
            cont++;
        }
        aux1 = aux1->prox;
        aux2 = aux2->prox;
    }
    if (cont == a)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Elemento_pol *remover_pol(Elemento_pol **lista, Poligono poli)
{
    Elemento_pol *aux, *removido = NULL;
    if (*lista)
    {
        if (compara_pol((*lista)->poligono, poli))
        {
            removido = *lista;
            *lista = (*lista)->prox;
        }
        else
        {
            aux = *lista;
            while (aux->prox != NULL && compara_pol(aux->prox->poligono, poli) == 0)
            {
                aux = aux->prox;
            }
            if (aux->prox)
            {
                removido = aux->prox;
                aux->prox = removido->prox;
            }
        }
    }
    return removido;
}

int insercao_pol_pointer(Elemento_pol **lista, Elemento_pol *pointer)
{
    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        pointer->prox = NULL;
        if (*lista == NULL)
        {
            *lista = pointer;
            return 1;
        }
        Elemento_pol *aux = *lista;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = pointer;
        return 1;
    }
}
