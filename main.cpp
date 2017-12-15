//
// Created by jack on 17-12-2.
//

#include <iostream>
#include "ObjLoader.h"

#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4


string filePath;
ObjLoader *objModel;
bool isClicked_Rotate = false;
bool isClicked_Translate = false;
int Rotate_temp_X = 0;
int Rotate_temp_Y = 0;
int Translate_temp_X = 0;
int Translate_temp_Y = 0;
int Rotate_change_x = 0, Rotate_change_y = 0, Translate_change_x = 0, Translate_change_y = 0;
float Scale_coefficient = 1;


int LastRotateX = 0;
int LastRotateY = 0;

//安置光源
void setLightRes() {
    GLfloat lightPosition[] = {100.0f, 100.0f, 100.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHTING); //启用光源
    glEnable(GL_LIGHT0);   //使用指定灯光
}

//初始化
void init() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("ObjLoader");
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLightRes();
    glEnable(GL_DEPTH_TEST);
}

void display() {
    objModel->changeTranslate(Translate_change_x, Translate_change_y, 0);
    objModel->changeScale(Scale_coefficient);
    objModel->changeRotate(0.0f, Rotate_change_x, Rotate_change_y);
    objModel->setLastRotateXY(LastRotateX, LastRotateY, isClicked_Rotate);
    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    setLightRes();
    gluLookAt(-5, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    objModel->Draw();//绘制obj模型
//    if (isClicked_Rotate)
//        glPopMatrix();
//    if (isClicked_Translate)
//        glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble) width / (GLdouble) height, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void moseMove(int button, int state, int x, int y) {
//    cout << "state:" << state << endl;
//    cout << "button:" << button << endl;
    if (button == GLUT_LEFT_BUTTON) {
        switch (state) {
            case GLUT_DOWN:
                isClicked_Rotate = !isClicked_Rotate;
                Rotate_temp_X = x;
                Rotate_temp_Y = y;
                break;
            case GLUT_UP:
                isClicked_Rotate = !isClicked_Rotate;
                LastRotateX = x - Rotate_temp_X;
                LastRotateY = y - Rotate_temp_Y;
                break;
            default:
                break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        switch (state) {
            case GLUT_DOWN:
                isClicked_Translate = !isClicked_Translate;
                Translate_temp_X = x;
                Translate_temp_Y = y;
                break;
            case GLUT_UP:
                isClicked_Translate = !isClicked_Translate;
                Translate_temp_Y = Translate_temp_X = 0;
                break;
            default:
                break;
        }
    }
    switch (button) {
        case GLUT_WHEEL_DOWN:
            Scale_coefficient += 0.1f;
            break;
        case GLUT_WHEEL_UP:
            Scale_coefficient -= 0.1f;
            if (Scale_coefficient <= 1.0f)
                Scale_coefficient = 1.0f;
            break;
        default:
            break;
    }

}


void mouseMotion(int x, int y) {
//    objModel->changeRotate(0, Rotate_change_x, Rotate_change_y);
    if (isClicked_Rotate) {
        Rotate_change_x = x - Rotate_temp_X;
        Rotate_change_y = y - Rotate_temp_Y;
    }
    if (isClicked_Translate) {
        Translate_change_x = (x - Translate_temp_X) / 10;
        Translate_change_y = (y - Translate_temp_Y) / 10;
    }
}


void keyboardFunction(unsigned char key, int x, int y) {

    switch (key) {
        case 'r':
            cout << "key:" << key << endl;
            Scale_coefficient = 1.0f;
            Translate_change_y = 0;
            Translate_change_x = 0;
            Rotate_change_y = 0;
            Rotate_change_x = 0;
            break;
        case 'q':
            cout << "program finished!" << endl;
            exit(0);
        default:
            break;

    }

}

void myIdle() {
    glutPostRedisplay();
}


int main(int argc, char **argv) {
    filePath = "bunny.obj";
    objModel = new ObjLoader(filePath);
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(moseMove);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboardFunction);
    glutIdleFunc(myIdle);
    glutMainLoop();
    return 0;
}