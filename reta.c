#include <stdlib.h>
#include "reta.h"
#include "ponto.h"
Elemento_r **criar_lista_r()
{
    Elemento_r **lista;
    lista = (Elemento_r **)malloc(sizeof(Elemento_r *));
    *lista = NULL;
    return lista;
}

Reta cria_reta(Ponto inicio, Ponto fim)
{
    Reta reta = {inicio, fim};
    return reta;
}
int insercao_r(Elemento_r **lista, Reta reta)
{
    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        Elemento_r *novo_elem = (Elemento_r *)malloc(sizeof(Elemento_r));
        novo_elem->reta = reta;
        novo_elem->prox = NULL;
        if (*lista == NULL)
        {
            *lista = novo_elem;
            return 1;
        }
        Elemento_r *aux = *lista;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo_elem;
        return 1;
    }
}

void delecao_lista_r(Elemento_r **lista)
{
    if (lista != NULL)
    {
        Elemento_r *aux = *lista;
        while (*lista != NULL)
        {
            aux = *lista;
            *lista = (*lista)->prox;
            free(aux);
        }
    }
    *lista = NULL;
}

int compara_reta(Reta p, Reta q)
{
    int a = (compara_ponto(p.inicio, q.inicio) && compara_ponto(p.fim, q.fim)) ? 1 : 0;
    return a;
}

Elemento_r *remover_r(Elemento_r **lista, Reta reta)
{
    Elemento_r *aux, *removido = NULL;
    if (*lista)
    {
        if (compara_reta((*lista)->reta, reta))
        {
            removido = *lista;
            *lista = (*lista)->prox;
        }
        else
        {
            aux = *lista;
            while (aux->prox != NULL && compara_reta(aux->prox->reta, reta) == 0)
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
int insercao_r_pointer(Elemento_r **lista, Elemento_r *pointer)
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
        Elemento_r *aux = *lista;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = pointer;
        return 1;
    }
}
