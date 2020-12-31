#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include "transformations.h"
#include "sdlInterface.h"
using namespace std;

SDL_Window   *window;
SDL_Renderer *renderer;
bool debug = true;  //to control debug statements

/*Write a program to use Perspective Projection to display the cube - given that the
viewpoint is at (6, 8, 7.5) with the viewing axis, Ze, pointed directly at the origin
of the WCS and Xe-axis lies on the Z=7.5 plane. Assume that the screen is a
square of size 30 cm, designed to be viewed from 60 cm away, and that the
coordinate system of the screen runs from 0 to 399.*/
//__________________________________________________________________________________________________________
float eyeX = 6;  
float eyeY = 8;
float eyeZ = 7.5; 
float screenZ = 200;  // 200 pixels to center,  
                    //z's inside screen are negative values from 0 to -399
float initScaleFactor = 1; // apply after adding x y z offsets
int main(int argc, char* argv[]){

    float PI = std::acos(-1); //let the  computer to find out what PI is
    string lineCoordinates;
    int objectCount =0;
    int lineCount =0;
    objectData object;
    input3dObject (object,&lineCount); // read in input lines from file
    if (debug) cout <<"the number of lines in object  is "<<lineCount<<"\n";
    updateOriginalProjections(object); // clacualte projection given current viewpoint
    resetTransformationMatrix();

    char choice;
    bool timeToQuit= false;
    float cx,cy,cz,angle;
    resetTransformationMatrix();
    //_________________________________________________________________________________________________________
    while (!timeToQuit){ 
        cout <<"current view point  eye coord "<<eyeX<<","<<eyeY<<","<<eyeZ<<"    screen z = "<<screenZ<<endl;
        cout <<"\nEnter desired operation:\n";
        cout << "(a) basic translation     (b) basic scaling      (c) basic rotation\n";
        cout << "(d) scale                 (e) draw lines\n";
        cout << "(f) clear transformation  (g) change view point\n";
        cout << "(s) save               (q) quit\n";
        cin >> choice;

        switch (choice){
        case'a': 
        {
                cout <<"enter basic translation parameters\n";
                cout <<"enter horizontal (x) displacement: ";
                translationMatrix[0][3]=getFloatFromCin();
                cout <<"\nenter vertical (y) displacement: ";
                translationMatrix[1][3]=getFloatFromCin();
                cout <<"\nenter depth (z) displacement: ";
                translationMatrix[2][3]=getFloatFromCin();
                if (debug) {
                    cout <<"\ntranslation matrix is:\n";
                    showMatrix(translationMatrix);
                }
                updateTransformation(translationMatrix);
                if (debug){
                    cout <<"\nupdated transformation matrix:\n";
                    showMatrix(transformationMatrix);
                }
                transformObject(object);
                updateTransformedProjections(object);
                displayObject(object);
                break;
        }         
        case'b': 
        {       cout <<"enter basic scaling parameters\n";
                cout <<"enter horizontal scale factor: ";
                scalingMatrix[0][0]=getFloatFromCin();;
                cout <<"enter vertical scale factor: ";
                scalingMatrix[1][1]=getFloatFromCin();
                cout <<"enter depth scale factor: ";
                scalingMatrix[2][2]=getFloatFromCin();
                if (debug) {
                    cout <<"scaling matrix is:\n";
                    showMatrix(scalingMatrix);
                }
                updateTransformation(scalingMatrix);
                if (debug){
                    cout <<"\nupdated transformation matrix:\n";
                    showMatrix(transformationMatrix);
                }
                transformObject(object);
                updateTransformedProjections(object);
                displayObject(object);
                break;  
        }
        case'c': 
        {       
                char axis;
                while (1)
                {
                    cout <<"select rotation axis : x, y or z: ";
                    cin >>axis;
                    if (axis == 'x' || axis == 'y' || axis == 'z') break;
                }
                cout <<"enter rotation angle\n";
                angle=getFloatFromCin();
                if (debug) {
                    cout <<"selected angle is "<<angle<<"\n";
                    cout <<"cosine is "<<cos(angle*PI/180)<<"\n";
                    cout <<"sine is "<<sin(angle*PI/180)<<"\n";
                }
                // set up rotation matrix depending on axis selection
                
                for (int i=0;i<4;i++){
                    for (int j=0;j<4;j++){
                        rotationMatrix[i][j] = 0;
                    }
                }
                
                if (axis == 'z'){
                    rotationMatrix[0][0] = cos(angle*PI/180);
                    rotationMatrix[0][1] = sin(angle*PI/180);
                    rotationMatrix[1][0] = -sin(angle*PI/180);
                    rotationMatrix[1][1] = cos(angle*PI/180);
                    rotationMatrix[2][2] =1;
                    rotationMatrix[3][3] =1;
                }
                else if (axis == 'x'){
                    rotationMatrix[1][1] = cos(angle*PI/180);
                    rotationMatrix[1][2] = sin(angle*PI/180);
                    rotationMatrix[2][1] = -sin(angle*PI/180);
                    rotationMatrix[2][2] = cos(angle*PI/180);
                    rotationMatrix[0][0] =1;
                    rotationMatrix[3][3] =1;
                }
                else{ // must be y
                    rotationMatrix[0][0] = cos(angle*PI/180);
                    rotationMatrix[0][2] = -sin(angle*PI/180);
                    rotationMatrix[2][0] = sin(angle*PI/180);
                    rotationMatrix[2][2] = cos(angle*PI/180);
                    rotationMatrix[1][1] =1;
                    rotationMatrix[3][3] =1;
                }
                 if (debug) {
                    cout <<"rotation matrix is:\n";
                    showMatrix(rotationMatrix);
                 }
                 updateTransformation(rotationMatrix);
                 if (debug) {
                    cout <<"\nupdated transformation matrix:\n";
                    showMatrix(transformationMatrix);
                 }
                transformObject(object);
                updateTransformedProjections(object);
                displayObject(object);
                 break;
        }
        case 'd': 
        {        cout <<"Enter Scaling Parameters:\n";
                 cout <<"Enter scale point X coordinate: ";
                 cx=getFloatFromCin();
                 cout <<"\nEnter scale rotation point Y coordinate: ";
                 cy=getFloatFromCin();
                 cout <<"\nEnter scale rotation point Z coordinate: ";
                 cz=getFloatFromCin();
                 cout <<"enter horizontal scale factor: ";
                 scalingMatrix[0][0]=getFloatFromCin();
                 cout <<"enter vertical scale factor: ";
                 scalingMatrix[1][1]=getFloatFromCin();
                 cout <<"enter depth scale factor: ";
                 scalingMatrix[2][2]=getFloatFromCin();

                //resetTransformationMatrix();
                //1. translate point to desired center of rotation
                translationMatrix[0][3]= (cx);
                translationMatrix[1][3]= (cy);
                translationMatrix[2][3]= (cz);
                updateTransformation(translationMatrix);
                if (debug) {
                    cout <<"\nupdated transformation matrix with first translation:\n";
                    showMatrix(transformationMatrix);
                }

                //2. scale point point
                // scaling matrix already loaded
                updateTransformation(scalingMatrix);
                if(debug){
                    cout <<"\nupdated transformation matrix with scaling:\n";
                    showMatrix(transformationMatrix);
                }

                //3/ translate back to original point
                translationMatrix[0][3]= -cx;  
                translationMatrix[1][3]= -cy;
                translationMatrix[2][3]= -cz;
                updateTransformation(translationMatrix);
                if (debug){
                    cout <<"\nupdated transformation matrix ith return translation:\n";
                    showMatrix(transformationMatrix);
                }
                transformObject(object);
                updateTransformedProjections(object);
                displayObject(object);
                break;
        }
        case 'e':

                updateTransformation(transformationMatrix);
                 if (debug){
                    cout <<"\nupdated transformation matrix ith return translation:\n";
                    showMatrix(transformationMatrix);
                }
                displayObject(object);
                break;
        case 'f':
                resetTransformationMatrix();
                transformObject(object);
                updateTransformedProjections(object);
                break;
        case 'g':
        {
            cout <<"Enter new X eye coordinate:";
            eyeX = getFloatFromCin();
            cout <<"Enter new Y eye coordinate:";
            eyeY = getFloatFromCin();
            cout <<"Enter new Z eye coordinate:";
            eyeZ = getFloatFromCin();
            
            cout <<"Enter new screen Z coordinate:";
            screenZ = getFloatFromCin();
            updateOriginalProjections(object);
            updateTransformedProjections(object);
            displayObject(object);
            break;
        }
        case 's':
        {
                string  fileName;
                ofstream outputFile;
                cout <<"Enter Output File Name where to save transformed lines: ";
                cin >> fileName;
                outputFile.open(fileName.c_str());
                for (int k=0;k<object.tLines.size();k++){
                    outputFile << object.tLines.at(k).x0 <<" "<< object.tLines.at(k).y0<<" "
                               <<object.tLines.at(k).x1<<" "<<object.tLines.at(k).y1<<"\n";
                }
                outputFile.close();
                break;
        }
        case'q':
                timeToQuit=true;
                break;
        default: cout <<"invalid choice, use a b c d e f g s q\n";
      }              
    }
    return 0;
}



