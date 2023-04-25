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

void desenharLinha() {
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
    if (button==GLUT_LEFT_BUTTON && state == GLUT_UP) {
        printf("%i,%i\n", pos_x, pos_y);
        addPonto(pos_x,height-pos_y);
        desenharPonto();
    }
    glutPostRedisplay();
}

//------MENU------
void menu(int opcao) {
    switch(opcao) {
        case 1:
            printf("oi");
            break;
        case 2:
            //criar reta
            break;
        case 3:
            //poligono
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

    int menu = glutCreateMenu(menu);
    glutAddMenuEntry("PONTO",0);
    glutAddMenuEntry("RETA",1);
    glutAddMenuEntry("POLIGONO",2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
