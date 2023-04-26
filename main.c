#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>

#define MAX_PONTOS 100
#define MAX_RETAS 100
#define MAX_PONTOS_POLIGONO 100
#define MAX_POLIGONOS 100

#define width 600
#define height 500

//define init func
//GLfloat t=0;

//------ESTRUTURA------
typedef struct {
    float x;
    float y;
}Ponto;

typedef struct {
    Ponto inicio;
    Ponto fim;
}Reta;

typedef struct {
    Ponto pontos[MAX_PONTOS_POLIGONO]
}Poligono;

int aux = 0;
int posAux_x;
int posAux_y;

//------VARIAVEIS------
int quantidade_pontos = 0;
Ponto pontos[MAX_PONTOS];

int quantidade_retas = 0;
Reta retas[MAX_RETAS];

int quantidade_poligonos = 0;
Poligono poligonos[MAX_POLIGONOS];


//------FUNCOES------
void addPonto(float x, float y) {
    pontos[quantidade_pontos].x = x;
    pontos[quantidade_pontos].y = y;

    quantidade_pontos++;
}

void desenharPonto() {
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i=0;i<quantidade_pontos;i++) {
        glColor3f(1.0,1.0,1.0);
        glVertex2f(pontos[i].x,pontos[i].y);
    }
    glEnd();
}

void addReta(float x1, float y1, float x2, float y2) {
    retas[quantidade_retas].inicio.x = x1;
    retas[quantidade_retas].inicio.y = y1;

    retas[quantidade_retas].fim.x = x2;
    retas[quantidade_retas].fim.y = y2;

    quantidade_retas++;
}

void desenharReta() {
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (int i = 0; i < quantidade_pontos; i++){
        glColor3f(1, 1, 1);
        glVertex2f(retas[i].inicio.x, retas[i].inicio.y);
        glVertex2f(retas[i].fim.x, retas[i].fim.y);
    }
    glEnd();
}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,width,0.0,height);
}

//------MOUSE------
void mouse(int button, int state, int x, int y) {
    int pos_x = x;
    int pos_y = y;

    printf("%i,%i\n", pos_x, pos_y);

    if (button==GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 1){
        addPonto(pos_x,height-pos_y);
        desenharPonto();
    } else if (button==GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 2) {
        if (quantidade_retas == 0) {
            posAux_x = pos_x;
            posAux_y = pos_y;
            printf("POSAUX_X= %i, POSAUX_Y = %i\n", posAux_x, posAux_y);
            addReta(posAux_x,height-posAux_y,pos_x,height-pos_y);
            desenharReta();
        } else {
            addReta(posAux_x,height-posAux_y,pos_x,height-pos_y);
            printf("POSAUX_X= %i, POSAUX_Y = %i\n", posAux_x, posAux_y);
            posAux_x = pos_x;
            posAux_y = pos_y;
            desenharReta();
            //problema relacionado ao tamanho do vetor dos pontos
        }
    }
    glutPostRedisplay();
}

//------MENU------
void menuOpcoes(int opcao) {
    switch(opcao) {
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

void display(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glFlush();
}

//------MAIN-------
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(width,height);
    glutCreateWindow("PAINT");
    init();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    int menu = glutCreateMenu(menuOpcoes);
    glutAddMenuEntry("PONTO", 0);
    glutAddMenuEntry("RETA", 1);
    glutAddMenuEntry("POLIGONO", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
