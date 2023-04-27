#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "ponto.h"
#include "reta.h"
#define MAX_PONTOS_POLIGONO 100
#define MAX_POLIGONOS 100

#define width 600
#define height 500

// define init func
// GLfloat t=0;

//------ESTRUTURA------

typedef struct
{
    Ponto pontos[MAX_PONTOS_POLIGONO];
} Poligono;

int aux = 0;
int posAux_x;
int posAux_y;

//------VARIAVEIS------
Elemento_p **lista_p = NULL;
Elemento_r **lista_r = NULL;
Ponto pontos_reta[2];
int cont_pontos_reta = 0;
int quantidade_pontos = 0;
int quantidade_retas = 0;
int quantidade_poligonos = 0;
Poligono poligonos[MAX_POLIGONOS];

//------FUNCOES------
void addPonto(Elemento_p **lista, Ponto p)
{
    insercao_p(lista, p);
    quantidade_pontos++;
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
    glEnd();
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
    printf("%d,%d\n", pos_x, pos_y);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 1)
    {
        Ponto p = cria_ponto(pos_x, height - pos_y);
        printf("%d,%d", p.x, p.y);
        addPonto(lista_p, p);
        desenharPonto(lista_p);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 2)
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

        desenharReta(lista_r);
    }
    glutPostRedisplay();
}

//------MENU------
void menuOpcoes(int opcao)
{
    switch (opcao)
    {
    case 0:
        printf("Escolheu Ponto\n");
        aux = 1;
        break;
    case 1:
        printf("Escolheu Reta\n");
        aux = 2;
        break;
    case 2:
        aux = 3;
        printf("Escolheu Poligono\n");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glFlush();
}

//------MAIN-------
int main(int argc, char **argv)
{
    lista_p = criar_lista_p();
    lista_r = criar_lista_r();
    (*lista_p) = NULL;
    (*lista_r) = NULL;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(width, height);
    glutCreateWindow("PAINT");
    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutCreateMenu(menuOpcoes);
    glutAddMenuEntry("PONTO", 0);
    glutAddMenuEntry("RETA", 1);
    glutAddMenuEntry("POLIGONO", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
