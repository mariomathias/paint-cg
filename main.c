#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "ponto.h"
#include "reta.h"
#include "poligono.h"

#define width 600
#define height 500

#define DENTRO 0
#define ACIMA 1
#define ABAIXO 2
#define ESQUERDA 4
#define DIREITA 8

// define init func
// GLfloat t=0;

//------ESTRUTURA------
int aux = 0;
int posAux_x;
int posAux_y;
int subMenuDesenhar;
int subMenuSelecionar;
int tolerance = 10;

//------VARIAVEIS------
// 1.listas
Elemento_p **lista_p = NULL;
Elemento_r **lista_r = NULL;
Elemento_pol **lista_pol = NULL;
// 2.auxiliares
Elemento_p **lista_auxiliar_p = NULL;     // Lista dos pontos selecionados
Elemento_r **lista_auxiliar_r = NULL;     // Lista das retas selecionadas
Elemento_pol **lista_auxiliar_pol = NULL; // Lista dos poligonos selecionados
Elemento_p **lista_vertices = NULL;
Ponto pontos_reta[2];
int cont_pontos_pol = 0;
int cont_pontos_reta = 0;
// 3.quantidades
int quantidade_pontos = 0;
int quantidade_retas = 0;
int quantidade_poligonos = 0;
// 4.controle
boolean drawing_pol = FALSE;

//------FUNCOES------
void addPonto(Elemento_p **lista, Ponto p)
{
    insercao_p(lista, p);
    quantidade_pontos++;
}

void desenharPoligono(Elemento_pol **lista)
{
    Elemento_pol *aux_pol = *lista;
    Elemento_p *aux_ponto = NULL;
    glColor3f(1.0, 1.0, 1.0);
    while (aux_pol != NULL)
    {
        aux_ponto = *(aux_pol->poligono.vertices);
        glBegin(GL_POLYGON);
        while (aux_ponto != NULL)
        {
            glVertex2i(aux_ponto->ponto.x, aux_ponto->ponto.y);
            aux_ponto = aux_ponto->prox;
        }
        glEnd();
        aux_pol = aux_pol->prox;
    }
    aux_pol = *lista_auxiliar_pol;
    glColor3f(0, 1.0, 0);
    while (aux_pol)
    {
        aux_ponto = *(aux_pol->poligono.vertices);
        glBegin(GL_POLYGON);
        while (aux_ponto != NULL)
        {
            glVertex2i(aux_ponto->ponto.x, aux_ponto->ponto.y);
            aux_ponto = aux_ponto->prox;
        }
        glEnd();
        aux_pol = aux_pol->prox;
    }
}

int finaliza_pol(Elemento_pol **lista_poligonos)
{
    int tamanho_pol = get_tamanho_lista_p(lista_vertices);
    if (tamanho_pol < 3)
    {
        delecao_lista_p(lista_vertices);
        return 0;
    }
    else
    {
        Ponto vertice = get_point_list(lista_vertices, 0);
        Ponto ultimo_vertice = cria_ponto(vertice.x, vertice.y);
        printf("bb\n");
        insercao_p(lista_vertices, ultimo_vertice);
        Poligono p = {lista_vertices, tamanho_pol + 1, 0};
        insercao_pol(lista_poligonos, p);
        quantidade_poligonos++;
        desenharPoligono(lista_pol);
        lista_vertices = NULL;
        return 1;
    }
}

void desenharPonto(Elemento_p **lista)
{
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    Elemento_p *aux = *lista;
    while (aux != NULL)
    {
        glVertex2i(aux->ponto.x, aux->ponto.y);
        print_ponto(aux->ponto);
        aux = aux->prox;
    }
    aux = *lista_auxiliar_p;
    glColor3f(0, 1.0, 0);
    while (aux != NULL)
    {
        glVertex2i(aux->ponto.x, aux->ponto.y);
        print_ponto(aux->ponto);
        aux = aux->prox;
    }
    glEnd();
}

void addReta(Elemento_r **lista, Reta r)
{
    insercao_r(lista, r);
    quantidade_retas++;
}

void desenharReta(Elemento_r **retas)
{
    glLineWidth(2.0);
    glColor3f(1.0, 1.0, 1.0);
    Elemento_r *aux = *retas;
    glBegin(GL_LINES);
    while (aux != NULL)
    {
        glVertex2f(aux->reta.inicio.x, aux->reta.inicio.y);
        glVertex2f(aux->reta.fim.x, aux->reta.fim.y);
        aux = aux->prox;
    }
    aux = *lista_auxiliar_r;
    glColor3f(0, 1.0, 0);
    while (aux != NULL)
    {
        glVertex2f(aux->reta.inicio.x, aux->reta.inicio.y);
        glVertex2f(aux->reta.fim.x, aux->reta.fim.y);
        aux = aux->prox;
    }
    glEnd();

}

//------FUNCOES AUXILIARES------
int pegarPonto(int px, int py, int mx, int my, int tolerance)
{
    // printf("%i, %i/%i, %i\n", px + tolerance, px - tolerance, py + tolerance, py - tolerance);
    // printf("%i, %i\n", mx, my);
    if (mx <= px + tolerance && mx >= px - tolerance)
    {
        if (my <= (height - py) + tolerance && my >= (height - py) - tolerance)
        {
            return 1;
        }
    }
    return 0;
}
int verificarPonto(Elemento_p **lista_p, Elemento_p **lista_auxiliar_p, int mx, int my)
{
    int foiSelecionado = 0;
    Elemento_p *aux = *lista_p;
    Elemento_p *removido = NULL;
    while (aux != NULL)
    {
        foiSelecionado = pegarPonto(aux->ponto.x, aux->ponto.y, mx, my, tolerance);
        printf("foiSelecionado: %i\n", foiSelecionado);
        if (foiSelecionado)
        {
            printf("ponto a ser removido: ");
            print_ponto(aux->ponto);
            removido = remover_p(lista_p, aux->ponto);
            printf("removido ponto: (%d,%d)\n", removido->ponto.x, removido->ponto.y);
            insercao_p_pointer(lista_auxiliar_p, removido);
        }
        aux = aux->prox;
    }
    return 0;
}

void deselecionaPonto(Elemento_p **lista_p)
{
    Elemento_p *aux = *lista_auxiliar_p;
    Elemento_p *removido = NULL;
    while (aux != NULL)
    {
        removido = remover_p(lista_auxiliar_p, (*lista_auxiliar_p)->ponto);
        aux = aux->prox;
        insercao_p_pointer(lista_p, removido);
    }
}

int identificarCodigo(Ponto p, int mx, int my)
{
    int codigo = DENTRO;
    printf("Mouse (%i,%i)\n", mx, my);
    // 0001
    if ((height - p.y) > (height-my) + tolerance)
        codigo += ACIMA;
    // 0010
    if ((height - p.y) < (height-my) - tolerance)
        codigo += ABAIXO;
    // 0100
    if (p.x > mx + tolerance)
        codigo += ESQUERDA;
    // 1000
    if (p.x < mx - tolerance)
        codigo += DIREITA;

    return codigo;
}
int pegarReta(Ponto inicioP, Ponto fimP, int mx, int my)
{
    int xmax = mx + tolerance;
    int xmin = mx - tolerance;
    int ymax = my + tolerance;
    int ymin = my - tolerance;
    int x1, y1, x2, y2;

    Ponto auxPonto;
    auxPonto.x = inicioP.x;
    auxPonto.y = inicioP.y;
    printf("auxPonto.x: %i\n", auxPonto.x);
    // printf("auxPonto.y: %i\n", auxPonto.y);

    while (1)
    {
        int codeInicioP = identificarCodigo(auxPonto, mx, my);
        int codeFimP = identificarCodigo(fimP, mx, my);
        printf("fimP: (%i,%i)\n", fimP.x, height-fimP.y);
        printf("codeInicioP: %i\n", codeInicioP);
        printf("codeFimP: %i\n", codeFimP);
        printf("AND: %i\n", codeInicioP & 1);
        printf("AND: %i\n", codeInicioP & 4);

        if (codeInicioP == 0 && codeFimP == 0)
        { // Os 2 pontos na tolerancia
            printf("Os 2 pontos estao na tolerancia\n");
            return 1;
            break;
        }
        else if (codeInicioP == 0 || codeFimP == 0)
        { // Um dos pontos na tolerancia
            printf("Um dos pontos na tolerancia\n");
            return 1;
            break;
        }
        else if ((codeInicioP & codeFimP) != 0)
        { // Totalmente fora da tolerancia
            printf("Totalmente fora da tolerancia\n");
            return 0;
            break;
        }
        else
        { // Caso nao trivial, mas so uma delas tao certas, ops
            x1 = auxPonto.x;
            y1 = auxPonto.y;
            x2 = fimP.x;
            y2 = fimP.y;

            if ((codeInicioP & 1) == ACIMA)
            {
                printf("Ponto inicial acima\n");
                auxPonto.x = x1 + (ymax - x1) * ((x2 - x1) / (y2 - y1));
                auxPonto.y = ymax;
                printf("|| auxPontox: %i ||\n", auxPonto.x);
                printf("|| auxPontoy: %i ||\n", auxPonto.y);
                break;
            }
            else if ((codeInicioP & 2) == ABAIXO)
            {
                printf("Ponto inicial abaixo\n");
                auxPonto.x = x1 + (ymin - y1) * ((x2 - x1) / (y2 - y1));
                auxPonto.y = ymin;
                printf("|| auxPontox: %i ||\n", auxPonto.x);
                printf("|| auxPontoy: %i ||\n", auxPonto.y);
            }
            else if ((codeInicioP & 4) == ESQUERDA)
            {
                printf("Ponto inicial a esquerda\n");
                auxPonto.y = y1 + (xmax - x1) * ((y2 - y1) / (x2 - y1));
                auxPonto.x = xmax;
                printf("|| auxPontox: %i ||\n", auxPonto.x);
                printf("|| auxPontoy: %i ||\n", auxPonto.y);
            }
            else if ((codeInicioP & 8) == DIREITA)
            {
                printf("Ponto inicial a direita\n");
                auxPonto.y = y1 + (xmin - x1) * ((y2 - y1) / (x2 - x1));
                auxPonto.x = xmin;
                printf("|| auxPontox: %i ||\n", auxPonto.x);
                printf("|| auxPontoy: %i ||\n", auxPonto.y);
            }
        }
    }
    return 0;
}

int verificarReta(Elemento_r **lista_r, Elemento_r **lista_auxiliar_r, int mx, int my)
{
    int foiSelecionado = 0;
    Elemento_r *aux = *lista_r;
    Elemento_r *removido = NULL;
    while (aux != NULL)
    {
        foiSelecionado = pegarReta(aux->reta.inicio, aux->reta.fim, mx, my);
        if (foiSelecionado)
        {
            removido = remover_r(lista_r, aux->reta);
            printf("removido reta com inicio: (%d,%d)\n", removido->reta.inicio.x, removido->reta.inicio.y);
            insercao_r_pointer(lista_auxiliar_r, removido);
        }
        aux = aux->prox;
    }
    return 0;
}

void deselecionaReta(Elemento_r **lista_r)
{
    Elemento_r *aux = *lista_auxiliar_r;
    Elemento_r *removido = NULL;
    while (aux != NULL)
    {
        removido = remover_r(lista_auxiliar_r, (*lista_auxiliar_r)->reta);
        aux = aux->prox;
        insercao_r_pointer(lista_r, removido);
    }
}

int pegarPoligono(Poligono pol, int x, int y)
{
    int intersec = 0, xi;
    //Elemento_pol *aux_pol = *lista_pol;
    //printf("%p\n", aux_pol);
    Elemento_p *vertice = NULL;
    printf("%p\n", vertice);
    //Elemento_p *primeiro_vertice = NULL;
    //primeiro_vertice = *(aux_pol->poligono.vertices);
    //printf("%p\n", primeiro_vertice);
    //printf("primeiro_vertice: %i, %i\n", primeiro_vertice->ponto.x, primeiro_vertice->ponto.y);
    //imprimir_lista(vertice);
    Ponto p1, p2;
    printf("MOUSE (%i,%i)\n",x,y);
    printf("vertice addr %p\n", vertice);
    vertice = *(pol.vertices);
    printf("vertice addr %p\n", vertice);
    while (vertice != NULL) {
        p1 = vertice->ponto;
        p2 = vertice->prox->ponto;
        printf("(p1.x > x) && (p2.x > x) = %i\n", (p1.x > x) && (p2.x > x));
        printf("(p1.y > y) && (p2.y < y) = %i\n", (p1.y > y) && (p2.y < y));
        printf("(p1.y < y) && (p2.y > y) = %i\n", (p1.y < y) && (p2.y > y));
        printf("((p1.y > y) && (p2.y < y)) || ((p1.y < y) && (p2.y > y)) = %i\n", ((p1.y > y) && (p2.y < y)) || ((p1.y < y) && (p2.y > y)));
        printf("(((p1.x > x) && (p2.x > x)) && (((p1.y > y) && (p2.y < y)) || ((p1.y < y) && (p2.y > y)))) = %i\n", (((p1.x > x) && (p2.x > x)) && (((p1.y > y) && (p2.y < y)) || ((p1.y < y) && (p2.y > y)))));
        printf("(%i, %i), (%i, %i)\n", p1.x, p1.y, p2.x, p2.y);
        if(((p1.y > y) && (p2.y > y)) || ((p1.y < y) && (p2.y < y)) || ((p1.x < x) && (p2.x < x)) || (p1.y == p2.y)) {
            printf("Continue\n");
        }
        else if (((p1.x > x) && (p2.x > x)) && (((p1.y > y) && (p2.y < y)) || ((p1.y < y) && (p2.y > y)))) {
            intersec += 1;
        }
        else {
            xi = p1.x + ((y - p1.y)*(p2.x - p1.x))/(p2.y - p1.y);
            printf("xi: %i\n", xi);
            if (xi > x) intersec += 1;
        }
        printf("intersec = %i\n", intersec);
        printf("vertice: %i, %i\n", vertice->ponto.x, vertice->ponto.y);
        vertice = vertice->prox;
        printf("PROX: %p\n", vertice->prox);
        if (vertice->prox == NULL) break;
            }
        if (intersec%2 != 0) return 1;
            printf("----------------------------------------------------------\n");
            printf("foiSelecionado = 0!!!!!!!!!!!!!!\n");
        //aux_pol = aux_pol->prox;
    return 0;
}

int verificarPoligono(Elemento_pol **lista_pol, int mx, int my)
{
    int foiSelecionado = 0;
    Elemento_pol *aux = *lista_pol;
    Elemento_pol *removido = NULL;

    while (aux != NULL)
    {
        foiSelecionado = pegarPoligono(aux->poligono, mx, my);
        if (foiSelecionado)
        {
            printf("ponto a ser removido: ");
            removido = remover_pol(lista_pol, aux->poligono);
            printf("removido poligono com ponto inicial: (%d,%d)\n", get_vertice_pol(aux->poligono, 0).x, get_vertice_pol(aux->poligono, 0).y);
            insercao_pol_pointer(lista_auxiliar_pol, removido);
        }
        printf("foiSelecionado: %i\n", foiSelecionado);
        aux = aux->prox;
    }
    return 0;
}

void deselecionaPoligono()
{
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_pol *removido = NULL;
    while (aux != NULL)
    {
        removido = remover_pol(lista_auxiliar_pol, aux->poligono);
        aux = aux->prox;
        insercao_pol_pointer(lista_pol, removido);
    }
}

void transladarPontoDireita(Elemento_p **lista_auxiliar_p)
{
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL)
    {
        aux->ponto.x += 5;
        printf("transladando para direita %i \n", aux->ponto.x);
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarRetaDireta(Elemento_r **lista_auxiliar_r)
{
    Elemento_r *aux = *lista_auxiliar_r;
    while (aux != NULL)
    {
        aux->reta.inicio.x += 2;
        aux->reta.fim.x += 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPolDireita(Elemento_pol **lista_auxiliar_pol)
{
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    while (aux != NULL)
    {
        vertice = *(aux->poligono.vertices);
        while (vertice != NULL)
        {
            vertice->ponto.x += 2;
            vertice = vertice->prox;
        }
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPontoEsquerda(Elemento_p **lista_auxiliar_p)
{
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL)
    {
        printf("transladando para esquerda %i \n", aux->ponto.x);
        aux->ponto.x -= 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarRetaEsquerda(Elemento_r **lista_auxiliar_r)
{
    Elemento_r *aux = *lista_auxiliar_r;
    while (aux != NULL)
    {
        aux->reta.inicio.x -= 2;
        aux->reta.fim.x -= 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPolEsquerda(Elemento_pol **lista_auxiliar_pol)
{
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    while (aux != NULL)
    {
        vertice = *(aux->poligono.vertices);
        while (vertice != NULL)
        {
            vertice->ponto.x -= 2;
            vertice = vertice->prox;
        }
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPontoCima(Elemento_p **lista_auxiliar_p)
{
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL)
    {
        printf("transladando para cima %i \n", aux->ponto.y);
        aux->ponto.y += 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarRetaCima(Elemento_r **lista_auxiliar_r)
{
    Elemento_r *aux = *lista_auxiliar_r;
    while (aux != NULL)
    {
        aux->reta.inicio.y += 2;
        aux->reta.fim.y += 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPolCima(Elemento_pol **lista_auxiliar_pol)
{
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    while (aux != NULL)
    {
        vertice = *(aux->poligono.vertices);
        while (vertice != NULL)
        {
            vertice->ponto.y += 2;
            vertice = vertice->prox;
        }
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPontoBaixo(Elemento_p **lista_auxiliar_p)
{
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL)
    {
        printf("transladando para baixo %i \n", aux->ponto.y);
        aux->ponto.y -= 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarRetaBaixo(Elemento_r **lista_auxiliar_r)
{
    Elemento_r *aux = *lista_auxiliar_r;
    while (aux != NULL)
    {
        aux->reta.inicio.y -= 2;
        aux->reta.fim.y -= 2;
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void transladarPolBaixo(Elemento_pol **lista_auxiliar_pol)
{
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    while (aux != NULL)
    {
        vertice = *(aux->poligono.vertices);
        while (vertice != NULL)
        {
            vertice->ponto.y -= 2;
            vertice = vertice->prox;
        }
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void aumentarReta(Elemento_r **lista_auxiliar_r){
    Elemento_r *aux = *lista_auxiliar_r;
    int centrox = 0;
    int centroy = 0;
    while (aux != NULL){
        centrox = (aux->reta.inicio.x + aux->reta.fim.x) / 2;
        centroy = (aux->reta.inicio.y + aux->reta.fim.y) / 2;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glScalef(2.0, 2.0, 1.0);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void aumentarPol(Elemento_pol **lista_auxiliar_pol){
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    int centrox = 0;
    int centroy = 0;
    int somax = 0;
    int somay = 0;
    int n = 0;
    while (aux != NULL){
        n = aux->poligono.qtd_vertices;
        vertice = *(aux->poligono.vertices);
        somax = 0;
        somay = 0;
        while (vertice != NULL){
            somax += vertice->ponto.x;
            somay += vertice->ponto.y;
        }
        centrox = somax / n;
        centroy = somay / n;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glScalef(2.0,2.0,1.0);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void diminuirReta(Elemento_r **lista_auxiliar_r){
    Elemento_r *aux = *lista_auxiliar_r;
    int centrox = 0;
    int centroy = 0;
    while (aux != NULL){
        centrox = (aux->reta.inicio.x + aux->reta.fim.x) / 2;
        centroy = (aux->reta.inicio.y + aux->reta.fim.y) / 2;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glScalef(0.5, 0.5, 1.0);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void diminuirPol(Elemento_pol **lista_auxiliar_pol){
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    int centrox = 0;
    int centroy = 0;
    int somax = 0;
    int somay = 0;
    int n = 0;
    while (aux != NULL){
        n = aux->poligono.qtd_vertices;
        vertice = *(aux->poligono.vertices);
        somax = 0;
        somay = 0;
        while (vertice != NULL){
            somax += vertice->ponto.x;
            somay += vertice->ponto.y;
        }
        centrox = somax / n;
        centroy = somay / n;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glScalef(0.5,0.5,1.0);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarPontoPositivo(Elemento_p **lista_auxiliar_p){
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL){
        glPushMatrix();
        glRotatef(10, 0,0,1);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarRetaPositivo(Elemento_r **lista_auxiliar_r){
    Elemento_r *aux = *lista_auxiliar_r;
    int centrox = 0;
    int centroy = 0;
    while (aux != NULL){
        centrox = (aux->reta.inicio.x + aux->reta.fim.x) / 2;
        centroy = (aux->reta.inicio.y + aux->reta.fim.y) / 2;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glRotatef(10,0,0,1);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarPolPositivo(Elemento_pol **lista_auxiliar_pol){
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    int centrox = 0;
    int centroy = 0;
    int somax = 0;
    int somay = 0;
    int n = 0;
    while (aux != NULL){
        n = aux->poligono.qtd_vertices;
        vertice = *(aux->poligono.vertices);
        somax = 0;
        somay = 0;
        while (vertice != NULL){
            somax += vertice->ponto.x;
            somay += vertice->ponto.y;
        }
        centrox = somax / n;
        centroy = somay / n;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glRotatef(10,0,0,1);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarPontoNegativo(Elemento_p **lista_auxiliar_p){
    Elemento_p *aux = *lista_auxiliar_p;
    while (aux != NULL){
        glPushMatrix();
        glRotatef(-10, 0,0,1);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarRetaNegativo(Elemento_r **lista_auxiliar_r){
    Elemento_r *aux = *lista_auxiliar_r;
    int centrox = 0;
    int centroy = 0;
    while (aux != NULL){
        centrox = (aux->reta.inicio.x + aux->reta.fim.x) / 2;
        centroy = (aux->reta.inicio.y + aux->reta.fim.y) / 2;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glRotatef(-10,0,0,1);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void rotacionarPolNegativo(Elemento_pol **lista_auxiliar_pol){
    Elemento_pol *aux = *lista_auxiliar_pol;
    Elemento_p *vertice = NULL;
    int centrox = 0;
    int centroy = 0;
    int somax = 0;
    int somay = 0;
    int n = 0;
    while (aux != NULL){
        n = aux->poligono.qtd_vertices;
        vertice = *(aux->poligono.vertices);
        somax = 0;
        somay = 0;
        while (vertice != NULL){
            somax += vertice->ponto.x;
            somay += vertice->ponto.y;
        }
        centrox = somax / n;
        centroy = somay / n;
        glPushMatrix();
        glTranslatef(centrox, centroy, 0);
        glRotatef(-10,0,0,1);
        glTranslatef(-centrox, -centroy, 0);
        glPopMatrix();
        aux = aux->prox;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, width, 0.0, height);
}

//------MOUSE------
void mouse(int button, int state, int x, int y)
{
    int pos_x = x;
    int pos_y = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 0)
    {
        Ponto p = cria_ponto(pos_x, height - pos_y);
        printf("%d,%d\n", p.x, p.y);
        addPonto(lista_p, p);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 1)
    {
        posAux_x = pos_x;
        posAux_y = pos_y;
        if (cont_pontos_reta == 0)
        {
            pontos_reta[cont_pontos_reta] = cria_ponto(posAux_x, height - posAux_y);
            cont_pontos_reta++;
        }
        else if (cont_pontos_reta == 1)
        {
            pontos_reta[cont_pontos_reta] = cria_ponto(pos_x, height - pos_y);
            print_ponto(pontos_reta[0]);
            print_ponto(pontos_reta[1]);
            Reta reta = cria_reta(pontos_reta[0], pontos_reta[1]);
            addReta(lista_r, reta);
            cont_pontos_reta = 0;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 2)
    {
        if (!drawing_pol)
        {
            drawing_pol = TRUE;
            lista_vertices = criar_lista_p();
        }
        addPonto(lista_vertices, cria_ponto(x, height - y));
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 3)
    {
        verificarPonto(lista_p, lista_auxiliar_p, pos_x, pos_y);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 4)
    {
        printf("x: %i, y: %i\n", x, y);
        verificarReta(lista_r, lista_auxiliar_r, x, height-y);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 5)
    {
        verificarPoligono(lista_pol, x, height - y);
    }
}

void deletarObjeto()
{
    if (aux == 3) delecao_lista_p(lista_auxiliar_p);
    else if (aux == 4) delecao_lista_r(lista_auxiliar_r);
    else if (aux == 5) delecao_lista_pol(lista_auxiliar_pol);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        if (aux == 2)
        {
            finaliza_pol(lista_pol);
            drawing_pol = FALSE;
            desenharPoligono(lista_pol);
            glFlush();
        }
        else
        {
            exit(0);
        }
        break;

    case 100:
        //move para direita no 'd'
        if (aux == 3)
        {
            transladarPontoDireita(lista_auxiliar_p);
        }
        if (aux == 4)
        {
            transladarRetaDireta(lista_auxiliar_r);
        }
        if (aux == 5)
        {
            transladarPolDireita(lista_auxiliar_pol);
        }
        break;
    case 97:
        //move para esquerda no 'a'
        if (aux == 3)
        {
            transladarPontoEsquerda(lista_auxiliar_p);
        }
        if (aux == 4)
        {
            transladarRetaEsquerda(lista_auxiliar_r);
        }
        if (aux == 5)
        {
            transladarPolEsquerda(lista_auxiliar_pol);
        }
        break;
    case 115:
        //move para baixo no 's'
        if (aux == 3)
        {
            transladarPontoBaixo(lista_auxiliar_p);
        }
        if (aux == 4)
        {
            transladarRetaBaixo(lista_auxiliar_r);
        }
        if (aux == 5)
        {
            transladarPolBaixo(lista_auxiliar_pol);
        }
        break;
    case 119:
        //move para cima no 'w'
        if (aux == 3)
        {
            transladarPontoCima(lista_auxiliar_p);
        }
        if (aux == 4)
        {
            transladarRetaCima(lista_auxiliar_r);
        }
        if (aux == 5)
        {
            transladarPolCima(lista_auxiliar_pol);
        }
        break;
    case 43:
        //aumenta clicando no +
        if (aux == 3){
            printf("não pode escalar ponto!\n");
        }
        if (aux == 4){
            aumentarReta(lista_auxiliar_r);
        }
        if (aux == 5){
            aumentarPol(lista_auxiliar_pol);
        }
        break;
    case 45:
        //diminui clicando no -
        if (aux ==3){
            printf("não pode escalar ponto!\n");
        }
        if (aux == 4){
            diminuirReta(lista_auxiliar_r);
        }
        if (aux == 5){
            diminuirPol(lista_auxiliar_pol);
        }
        break;
    case 52:
        //rotaciona no sentido horário no 4
        printf("rotacionando\n");
        if (aux == 3){
            rotacionarPontoPositivo(lista_auxiliar_p);
        }
        if (aux == 4){
            rotacionarRetaPositivo(lista_auxiliar_r);
        }
        if (aux == 5){
            rotacionarPolPositivo(lista_auxiliar_pol);
        }
        break;
    case 54:
        //rotaciona no sentido antihorário no 6
        printf("rotacionando\n");
        if (aux == 3){
            rotacionarPontoNegativo(lista_auxiliar_p);
        }
        if (aux == 4){
            rotacionarRetaNegativo(lista_auxiliar_r);
        }
        if (aux == 5){
            rotacionarPolNegativo(lista_auxiliar_pol);
        }
        break;
    case 101:
        printf("deletando\n");
        deletarObjeto();
    default:
        break;
    }
}

void deselecionaObjetos()
{
    deselecionaPonto(lista_p);
    deselecionaReta(lista_r);
    deselecionaPoligono();
}

//------MENU------
void menuOpcoes(int opcao)
{
    switch (opcao)
    {
    case 0:
        printf("Escolheu Ponto\n");
        aux = 0;
        deselecionaObjetos();
        break;
    case 1:
        printf("Escolheu Reta\n");
        aux = 1;
        deselecionaObjetos();
        break;
    case 2:
        aux = 2;
        printf("Escolheu Poligono\n");
        break;
    case 3:
        aux = 3;
        printf("Escolheu Selecionar Objeto\n");
        break;
    case 4:
        aux = 4;
        printf("Escolheu Selecionar Reta\n");
        break;
    case 5:
        aux = 5;
        printf("Escolheu Selecionar Poligono\n");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    desenharPonto(lista_p);
    desenharReta(lista_r);
    desenharPoligono(lista_pol);
    glLoadIdentity();
    glFlush();
}

//------MAIN-------
int main(int argc, char **argv)
{
    lista_p = criar_lista_p();
    lista_auxiliar_p = criar_lista_p();
    lista_auxiliar_r = criar_lista_r();
    lista_auxiliar_pol = criar_lista_pol();
    lista_r = criar_lista_r();
    lista_pol = criar_lista_pol();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(width, height);
    glutCreateWindow("PAINT");
    init();
    glClear(GL_COLOR_BUFFER_BIT);

    subMenuDesenhar = glutCreateMenu(menuOpcoes);
    glutAddMenuEntry("PONTO", 0);
    glutAddMenuEntry("RETA", 1);
    glutAddMenuEntry("POLIGONO", 2);

    subMenuSelecionar = glutCreateMenu(menuOpcoes);
    glutAddMenuEntry("PONTO", 3);
    glutAddMenuEntry("RETA", 4);
    glutAddMenuEntry("POLIGONO", 5);

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutCreateMenu(menuOpcoes);
    glutDisplayFunc(display);
    glutAddSubMenu("DESENHAR", subMenuDesenhar);
    glutAddSubMenu("SELECIONAR", subMenuSelecionar);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
