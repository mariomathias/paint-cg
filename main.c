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
        printf("addrs: %p\n", aux_pol);
        glEnd();
        aux_pol = aux_pol->prox;
    }
}

int finaliza_pol(Elemento_pol **lista_poligonos)
{
    int tamanho_pol = get_tamanho_lista_p(lista_vertices);
    if (tamanho_pol < 3)
    {
        delecao_lista(lista_vertices);
        return 0;
    }
    else
    {
        Ponto vertice = get_point_list(lista_vertices, 0);
        Ponto ultimo_vertice = cria_ponto(vertice.x, vertice.y);
        printf("bb\n");
        insercao_p(lista_vertices, ultimo_vertice);
        Poligono p = {lista_vertices, tamanho_pol + 1};
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
    glEnd();
}

//------FUNCOES AUXILIARES------
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
}

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
        printf("fimP: (%i,%i)\n", fimP.x, height - fimP.y);
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

int identificarCodigo(Ponto p, int mx, int my)
{
    int codigo = DENTRO;
    printf("Mouse (%i,%i)\n", mx, my);
    // 0001
    if ((height - p.y) > my + tolerance)
        codigo += ACIMA;
    // 0010
    if ((height - p.y) < my - tolerance)
        codigo += ABAIXO;
    // 0100
    if (p.x > mx + tolerance)
        codigo += ESQUERDA;
    // 1000
    if (p.x < mx - tolerance)
        codigo += DIREITA;

    return codigo;
}

int verificarReta(Elemento_r **lista_r, int mx, int my)
{
    int foiSelecionado = 0;
    Elemento_r *aux = *lista_r;

    while (aux != NULL)
    {
        foiSelecionado = pegarReta(aux->reta.inicio, aux->reta.fim, mx, my);
        printf("foiSelecionado: %i\n", foiSelecionado);
        aux = aux->prox;
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
        verificarReta(lista_r, x, y);
    }
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
        // aumenta
        if (aux == 3)
        {
            printf("n�o pode escalar ponto!\n");
        }
        break;
    case 45:
        // diminui
        if (aux == 3)
        {
            printf("n�o pode escalar ponto!\n");
        }
    }
}

//------MENU------
void menuOpcoes(int opcao)
{
    switch (opcao)
    {
    case 0:
        printf("Escolheu Ponto\n");
        aux = 0;
        deselecionaPonto(lista_p);
        break;
    case 1:
        printf("Escolheu Reta\n");
        aux = 1;
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
        printf("Escolheu Selecionar Pol�gono\n");
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
    // coloquei as mesmas opcoes so para nao deixar em branco
    // mas ao criar as funcinalidade de selecionar
    // encaixar aqui por favor
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
