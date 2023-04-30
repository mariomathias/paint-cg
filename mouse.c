#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.c"
#include <gl/glut.h>

#define width 600
#define height 500

int aux = 0;
int posAux_x;
int posAux_y;

void mouse(int button, int state, int x, int y) {
    int pos_x = x;
    int pos_y = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 1){
        printf("%i,%i\n", pos_x, pos_y);
        addPonto(pos_x,height-pos_y);
        desenharPonto();
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && aux == 2) {
        if (quantidade_retas == 0) {
            printf("%i,%i\n", pos_x, pos_y);
            posAux_x = pos_x;
            posAux_y = pos_y;
            printf("POSAUX_X= %i, POSAUX_Y = %i\n", posAux_x, posAux_y);
            addReta(posAux_x,height-posAux_y,pos_x,height-pos_y);
            desenharReta();
        } else {
            printf("%i,%i\n", pos_x, pos_y);
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
