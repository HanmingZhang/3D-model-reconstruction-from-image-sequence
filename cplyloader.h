#ifndef CPLYLOADER_H
#define CPLYLOADER_H


#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;


struct SModelData
{
    vector <float> vecFaceTriangles;        // = face * 9
    vector <float> vecFaceTriangleColors;   // = face * 9
    vector <float> vecNormals;              // = face * 9
    int iTotalConnectedTriangles;
};

class CPLYLoader
{
public:
    CPLYLoader();
    int LoadModel(char *filename);
    void Draw();

private:
    float* mp_vertexXYZ;
    float* mp_vertexNorm;
    float* mp_vertexRGB;
    int m_totalConnectedQuads;
    int m_totalConnectedPoints;
    int m_totalFaces;
    SModelData m_ModelData;
};

#endif // CPLYLOADER_H
