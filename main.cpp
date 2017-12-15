//
// Created by jack on 17-12-2.
//

#include <iostream>
#include "ObjLoader.h"
#include "arcball.h"

#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4


string filePath;
ObjLoader *objModel;


ArcBallT arcBall(640.0f, 480.0f);
ArcBallT *arcBallT = &arcBall;
float Translate_temp_X;
float Translate_temp_Y;
float Translate_change_x = 0, Translate_change_y = 0;
float Scale_coefficient = 1;

bool isRightButtonClicked = false;


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
    glutInitWindowSize(640, 480);
    glutCreateWindow("ObjLoader");
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLightRes();
    glEnable(GL_DEPTH_TEST);
}

void display() {
//    objModel->changeTranslate(Translate_change_x, Translate_change_y, 0);
//    objModel->changeScale(Scale_coefficient);


    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    setLightRes();
    gluLookAt(0, 0, 10, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(Translate_change_x, Translate_change_y, 0);
    cout << "Translate_change_x" << Translate_change_x << endl;
    glScalef(Scale_coefficient, Scale_coefficient, Scale_coefficient);
    glMultMatrixf(arcBallT->Transform.M);
    objModel->Draw();//绘制obj模型
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void reshape(int width, int height) {
    arcBallT->setBounds(width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble) width / (GLdouble) height, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void mouseMotion(int x, int y) {
    arcBallT->MousePt.s.X = x;
    arcBallT->MousePt.s.Y = y;
    arcBallT->upstate();
    if (isRightButtonClicked) {
        Translate_change_x += -(Translate_temp_X - x) * 0.01f;
        Translate_change_y += (Translate_temp_Y - y) * 0.01f;
        Translate_temp_X = x;
        Translate_temp_Y = y;
    }
    glutPostRedisplay();
}

void moseMove(int button, int state, int x, int y) {
//    cout << "state:" << state << endl;
//    cout << "button:" << button << endl;
    if (button == GLUT_LEFT_BUTTON) {
        switch (state) {
            case GLUT_DOWN:
//                isClicked_Rotate = !isClicked_Rotate;
                arcBallT->isClicked = true;
                mouseMotion(x, y);
                break;
            case GLUT_UP:
                arcBallT->isClicked = false;
                break;
            default:
                break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        switch (state) {
            case GLUT_DOWN:
//                isClicked_Translate = !isClicked_Translate;
                arcBallT->isRClicked = true;
                mouseMotion(x, y);
                Translate_temp_X = x;
                Translate_temp_Y = y;
                isRightButtonClicked = true;
                break;
            case GLUT_UP:
                arcBallT->isRClicked = false;
                isRightButtonClicked = false;
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
            if (Scale_coefficient <= 1.f)
                Scale_coefficient = 1.f;
            break;
        default:
            break;
    }
    arcBallT->upstate();
    glutPostRedisplay();

}


void keyboardFunction(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            cout << "key:" << key << endl;
            Scale_coefficient = 1.0f;
            Translate_change_y = 0;
            Translate_change_x = 0;
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
    //arcBallT = new ArcBall_t(500, 500);
    glutInit(&argc, argv);
    cout << "end" << endl;

    cout << "1" << endl;
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