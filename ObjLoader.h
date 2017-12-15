//
// Created by jack on 17-12-2.
//

#ifndef BUNNY_VTMOEDEL_H
#define BUNNY_VTMOEDEL_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <GL/glut.h>

using namespace std;

class ObjLoader {
public:
    ObjLoader() = default;

    explicit ObjLoader(const string &filename) {
        string line;
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "没有发现OBJ文件" << endl;
            exit(2);
        }

        while (!f.eof()) {
            getline(f, line);//拿到obj文件中一行，作为一个字符串
            vector<string> parameters;
            string tailMark = " ", ans;

            line = line.append(tailMark);
            for (char ch : line) {
                if (ch != ' ') {
                    ans += ch;
                } else {
                    parameters.push_back(ans); //取出字符串中的元素，以空格切分
                    ans.clear();
                }
            }
            if (parameters.size() != 4) {
                cout << "the size is not correct" << endl;
                exit(3);
            } else {
                if (parameters[0] == "v") {   //如果是顶点的话
                    vector<GLfloat> Point;
                    for (int i = 1; i < 4; i++) {   //从1开始，将顶点的xyz三个坐标放入顶点vector
                        auto xyz = (float) strtod(parameters[i].c_str(), nullptr);
                        Point.push_back(xyz);
                    }
                    vSets.push_back(Point);
                } else if (parameters[0] == "f") {   //如果是面的话，存放三个顶点的索引
                    vector<GLint> vIndexSets;
                    for (int i = 1; i < 4; i++) {
                        string x = parameters[i];
                        ans.clear();
                        for (char ch : x) {   //跳过‘/’
                            if (ch != '/') {
                                ans += ch;
                            } else {
                                break;
                            }
                        }
                        auto index = (int) strtod(ans.c_str(), nullptr);
                        index--;
                        vIndexSets.push_back(index);
                    }
                    fSets.push_back(vIndexSets);
                }
            }
        }
        f.close();

    }

    ~ObjLoader() = default;

    void Draw() {


        glTranslatef(TranslateX, TranslateY, TranslateZ);
        glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(RotateY, 0.0f, 1.0f, 0.0f);
        glRotatef(RotateZ, 0.0f, 0.0f, 1.0f);
        if (isLeftButtonClicked) {
            cout << "RotateX:" << RotateX << endl;
            cout << "RotateY:" << RotateY << endl;
            cout << "RotateZ:" << RotateZ << endl;
        }
        glScalef(ScaleXYZ, ScaleXYZ, ScaleXYZ);

        glBegin(GL_TRIANGLES);//开始绘制
        for (auto &fSet : fSets) {
            GLfloat VN[3];
            //三个顶点
            GLfloat SV1[3];
            GLfloat SV2[3];
            GLfloat SV3[3];
            if (fSet.size() != 3) {
                cout << "the fSetsets_Size is not correct" << endl;
            } else {
                GLint firstVertexIndex = fSet[0];//取出顶点索引
                GLint secondVertexIndex = fSet[1];
                GLint thirdVertexIndex = fSet[2];

                SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
                SV1[1] = (vSets[firstVertexIndex])[1];
                SV1[2] = (vSets[firstVertexIndex])[2];

                SV2[0] = (vSets[secondVertexIndex])[0]; //第二个顶点
                SV2[1] = (vSets[secondVertexIndex])[1];
                SV2[2] = (vSets[secondVertexIndex])[2];

                SV3[0] = (vSets[thirdVertexIndex])[0]; //第三个顶点
                SV3[1] = (vSets[thirdVertexIndex])[1];
                SV3[2] = (vSets[thirdVertexIndex])[2];


                GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
                //(x2-x1,y2-y1,z2-z1)
                vec1[0] = SV1[0] - SV2[0];
                vec1[1] = SV1[1] - SV2[1];
                vec1[2] = SV1[2] - SV2[2];

                //(x3-x2,y3-y2,z3-z2)
                vec2[0] = SV1[0] - SV3[0];
                vec2[1] = SV1[1] - SV3[1];
                vec2[2] = SV1[2] - SV3[2];

                //(x3-x1,y3-y1,z3-z1)
                vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
                vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

                auto D = (float) sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

                VN[0] = vec3[0] / D;
                VN[1] = vec3[1] / D;
                VN[2] = vec3[2] / D;

                glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量
                glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制三角面片
                glVertex3f(SV2[0], SV2[1], SV2[2]);
                glVertex3f(SV3[0], SV3[1], SV3[2]);
            }
        }
        glEnd();//拿到obj文件中一行，作为一个字符串
        vector<string> parameters;
        string tailMark = " ", ans;

    }

    void changeRotate(float RotateX, float RotateY, float RotateZ) {
        this->RotateX = RotateX;
        this->RotateY = RotateY;
        this->RotateZ = RotateZ;
    }

    void changeTranslate(float TranslateX, float TranslateY, float TranslateZ) {
        this->TranslateX = TranslateX;
        this->TranslateY = TranslateY;
        this->TranslateZ = TranslateZ;
    }

    void changeScale(float ScaleXYZ) {
        this->ScaleXYZ = ScaleXYZ;
    }

    void setLastRotateXY(float LastRotateX, float LastRotateY, bool isLeftButtonClicked) {
        this->LastRotateX = LastRotateX;
        this->LastRotateY = LastRotateY;
        this->isLeftButtonClicked = isLeftButtonClicked;
    }


private:
    vector<vector<GLfloat >> vSets;
    vector<vector<GLint>> fSets;
    float RotateX = 0.0f;
    float RotateY = 0.0f;
    float RotateZ = 0.0f;
    float TranslateX = 0.0f;
    float TranslateY = 0.0f;
    float TranslateZ = 0.0f;
    float ScaleXYZ = 1.0f;
    float LastRotateX = 0.0f;
    float LastRotateY = 0.0f;
    bool isLeftButtonClicked = false;
};


#endif //BUNNY_VTMOEDEL_H
