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
    Reta reta = {inicio, fim, 0};
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
