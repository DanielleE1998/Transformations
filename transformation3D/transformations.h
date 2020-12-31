#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <vector>
#include <cmath>

using namespace std;
enum transformationType {basicTranslate,basicScale,basicRotate,
                        scale,rotate};
struct coordXYZ{
    float x;
    float y;
    float z;
};                        

struct lineData{ //3D
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;
};

struct projectionCoord{ //2D
    float x0;
    float y0;
    float x1;
    float y1;
};

struct objectData{
    vector <lineData> oLines;
    vector <lineData> tLines;
    vector <projectionCoord> oProjection; // original projections
    vector <projectionCoord> tProjection;// transformend projections
};

extern bool debug;
extern float eyeX;
extern float eyeY;
extern float eyeZ; 
extern float screenZ;
extern float initScaleFactor;

//where to keep overall transformation:
extern float transformationMatrix[4][4];
extern float translationMatrix[4][4];
extern float scalingMatrix[4][4];
extern float rotationMatrix[4][4];     
extern void showObjectVertices(objectData obj);
extern void resetTransformationMatrix();
extern  void showMatrix(float matrix[4][4]);  
extern void transformObject(objectData &object);
extern  void updateTransformation(float operation[4][4]);
extern void input3dObject(objectData &object, int* num);   
extern void updateOriginalProjections(objectData &obj);
extern void updateTransformedProjections(objectData &obj);
extern float getFloatFromCin();
#endif