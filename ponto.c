#include "ponto.h"
Elemento_p **criar_lista_p()
{
    Elemento_p **lista;
    lista = (Elemento_p **)malloc(sizeof(Elemento_p *));
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
