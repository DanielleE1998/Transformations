#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "transformations.h"
using namespace std;

float transformationMatrix[4][4] = {{1,2,3,4},
                                    {5,6,7,8},
                                    {9,10,11,12},
                                    {13,14,15,16}};
                                    
float operationMatrix[4][4] =       {{1,2,3,4},
                                    {5,6,7,8},
                                    {9,10,11,12},
                                    {13,14,15,16}};


float translationMatrix[4][4] = {{1,0,0,0}, //1 0 0 tx 
                                 {0,1,0,0}, //0 1 0 ty 
                                 {0,0,1,0}, //0 0 0 tz
                                 {0,0,0,0}};

float scalingMatrix[4][4] = {{1,0,0,0}, //  Sx 0  0  0
                             {0,1,0,0}, //  0  Sy 0  0
                             {0,0,1,0}, //  0  0  Sz 0
                             {0,0,0,1}};

float rotationMatrix[4][4] =    {{0,0,0,0},   
                                {0,0,0,0},  
                                {0,0,0,0},
                                {0,0,0,0}}; 
 
float projectionMatrix[4][4] =  {{1,0,0,0},     // projection on xy plane
                                {0,1,0,0},  
                                {0,0,0,0},
                                {0,0,0,1}};     
//_______________________________________________________________________________________________________________
/*Reads ‘3dObjects’ from an external file (name of file is provided by the user). On
return `num' will contain the number of objects read from the file.
*/
void input3dObject( objectData &object, int* num)
{  
    int lineCount =0;
    string inFileName;
    ifstream inFile;
    while (1){
        cout <<"Enter Input File Name: ";
        cin >> inFileName;
        inFile.open(inFileName.c_str());
        if (!inFile) cout <<"unable to open input stream "<<inFileName<<endl;
        else break;
    }
    while (1){
        if (inFile.eof()) break;
        lineData line;
        inFile >> line.x0;       
        inFile >> line.y0;
        inFile >> line.z0;
        inFile >> line.x1;       
        inFile >> line.y1;
        inFile >> line.z1;
        
        line.x0=line.x0 *initScaleFactor;
        line.y0=line.y0 *initScaleFactor;
        line.z0=line.z0 *initScaleFactor;
        line.x1=line.x1*initScaleFactor;
        line.y1=line.y1*initScaleFactor;
        line.z1=line.z1*initScaleFactor;
        
        object.oLines.push_back(line);
        line.x0=0;
        line.y0=0;
        line.z0=0;
        line.x1=0;
        line.y1=0;
        line.z1=0;
        object.tLines.push_back(line); // init so we can use .at()
        lineCount++;   
    }
        // create projection lines
        for (int i=0;i<lineCount;i++){
            projectionCoord pLine;
            pLine.x0 = eyeX + (((screenZ-eyeZ)*(object.oLines.at(i).x0 - eyeX))/(object.oLines.at(i).z0 - eyeZ));   
            pLine.y0 = eyeY + (((screenZ-eyeZ)*(object.oLines.at(i).y0 - eyeY))/(object.oLines.at(i).z0 - eyeZ));
            pLine.x1 = eyeX + (((screenZ-eyeZ)*(object.oLines.at(i).x1 - eyeX))/(object.oLines.at(i).z1 - eyeZ)); 
            pLine.y1 = eyeY + (((screenZ-eyeZ)*(object.oLines.at(i).y1 - eyeY))/(object.oLines.at(i).z1 - eyeZ));
            object.oProjection.push_back(pLine);
            if (debug) cout <<"original projection "<<i<<" "<<pLine.x0<<","<<pLine.y0<<" to "
                                        <<pLine.x1<<","<<pLine.y1<<endl;
            pLine.x0=0;
            pLine.x1=0;
            pLine.y0=0;
            pLine.y1=0;
            object.tProjection.push_back(pLine); 
    }
     *num = lineCount;
}

//________________________________________________________________________________________________________________
void updateOriginalProjections(objectData &obj){
        for (int i=0;i<obj.oProjection.size();i++){
            obj.oProjection.at(i).x0 = eyeX + (((screenZ-eyeZ)*(obj.oLines.at(i).x0 - eyeX))/(obj.oLines.at(i).z0 - eyeZ));   
            obj.oProjection.at(i).y0 = eyeY + (((screenZ-eyeZ)*(obj.oLines.at(i).y0 - eyeY))/(obj.oLines.at(i).z0 - eyeZ));
            obj.oProjection.at(i).x1 = eyeX + (((screenZ-eyeZ)*(obj.oLines.at(i).x1 - eyeX))/(obj.oLines.at(i).z1 - eyeZ)); 
            obj.oProjection.at(i).y1 = eyeY + (((screenZ-eyeZ)*(obj.oLines.at(i).y1 - eyeY))/(obj.oLines.at(i).z1 - eyeZ));
            cout <<"original projection "<<i<<" "<<obj.oProjection.at(i).x0<<","<<obj.oProjection.at(i).y0<<" to "
                                        <<obj.oProjection.at(i).x1<<","<<obj.oProjection.at(i).y1<<endl;
    }

} 
 
void updateTransformedProjections(objectData &obj){
    for (int i=0;i<obj.tProjection.size();i++){
            obj.tProjection.at(i).x0 = eyeX + (((screenZ-eyeZ)*(obj.tLines.at(i).x0 - eyeX))/(obj.tLines.at(i).z0 - eyeZ));   
            obj.tProjection.at(i).y0 = eyeY + (((screenZ-eyeZ)*(obj.tLines.at(i).y0 - eyeY))/(obj.tLines.at(i).z0 - eyeZ));
            obj.tProjection.at(i).x1 = eyeX + (((screenZ-eyeZ)*(obj.tLines.at(i).x1 - eyeX))/(obj.tLines.at(i).z1 - eyeZ)); 
            obj.tProjection.at(i).y1 = eyeY + (((screenZ-eyeZ)*(obj.tLines.at(i).y1 - eyeY))/(obj.tLines.at(i).z1 - eyeZ));
            cout <<"transformedd projection "<<i<<" "<<obj.tProjection.at(i).x0<<","<<obj.tProjection.at(i).y0<<" to "
                                                <<obj.tProjection.at(i).x1<<","<<obj.tProjection.at(i).y1<<endl;
    }
}

//____________________________________________________________________________________________________________________

void showLine(lineData line){
   if (debug) cout <<"  x0,y0,z0  "<<line.x0<<","<<line.y0<<","<<line.z0<<"  x1,y1,z1 "<<line.x1<<","<<line.y1<<","<<line.z1<<"\n";
}

void showMatrix(float matrix[4][4]){
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
                cout <<"i ="<<i<<" j= "<<j<< "holds value = "<<matrix[i][j]<<"\n";
        }
    }  
}

void resetTransformationMatrix()
{    
    //init transformation matrix to unity matrix:
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
                if (i==j)
                    transformationMatrix[i][j]=1;
                else
                    transformationMatrix[i][j]=0;
                    
        }
    }
}
float multiplyrowcolumn(int row,int col){
    return  (transformationMatrix[row][0]*operationMatrix[0][col] +
             transformationMatrix[row][1]*operationMatrix[1][col] +
             transformationMatrix[row][2]*operationMatrix[2][col]+
             transformationMatrix[row][3]*operationMatrix[3][col]);
}

void updateTransformation(float operation[4][4]){
    //copy operation to operationMatrix
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            operationMatrix[i][j]=operation[i][j];
        }
    }
    float result[4][4];    
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            result[i][j]=multiplyrowcolumn(i,j);
        }
    }
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            transformationMatrix[i][j]=result[i][j];
        }
    }
}

//________________________________________________________________________________________________
void transformObject(objectData &obj){ 
    //multiply the rows in the transformation matrix with the column for  in point xyz
    for (unsigned int i=0;i<obj.tLines.size();i++){

            obj.tLines.at(i).x0 = obj.oLines.at(i).x0 * transformationMatrix[0][0] +
                                  obj.oLines.at(i).y0 * transformationMatrix[0][1] +
                                  obj.oLines.at(i).z0 * transformationMatrix[0][2] +
                                  1*transformationMatrix[0][3];

            obj.tLines.at(i).y0 = obj.oLines.at(i).x0 * transformationMatrix[1][0] +
                                  obj.oLines.at(i).y0 * transformationMatrix[1][1] +
                                  obj.oLines.at(i).z0 * transformationMatrix[1][2] +
                                  1*transformationMatrix[1][3];
                                            
            obj.tLines.at(i).z0 =  obj.oLines.at(i).x0 * transformationMatrix[2][0] +
                                   obj.oLines.at(i).y0 * transformationMatrix[2][1] +
                                   obj.oLines.at(i).z0 * transformationMatrix[2][2] +
                                   1*transformationMatrix[2][3];
                                            
            obj.tLines.at(i).x1 =  obj.oLines.at(i).x1 * transformationMatrix[0][0] +
                                   obj.oLines.at(i).y1 * transformationMatrix[0][1] +
                                   obj.oLines.at(i).z1 * transformationMatrix[0][2] +
                                   1*transformationMatrix[0][3];
    
            obj.tLines.at(i).y1 =  obj.oLines.at(i).x1 * transformationMatrix[1][0] +
                                   obj.oLines.at(i).y1 * transformationMatrix[1][1] +
                                   obj.oLines.at(i).z1 * transformationMatrix[1][2] +
                                   1*transformationMatrix[1][3];
                                            
            obj.tLines.at(i).z1 =  obj.oLines.at(i).x1 * transformationMatrix[2][0] +
                                   obj.oLines.at(i).y1 * transformationMatrix[2][1] +
                                   obj.oLines.at(i).z1 * transformationMatrix[2][2] +
                                   1*transformationMatrix[2][3];   
   
            cout <<"translated segments: "<<i<<"\n";
            cout <<"segment AB "<<obj.tLines.at(i).x0<<","<<obj.tLines.at(i).y0<<","<<obj.tLines.at(i).z0<<"to "<<
                                  obj.tLines.at(i).x1<<","<<obj.tLines.at(i).y1<<","<<obj.tLines.at(i).z1<<"\n";
                                  
    }           
}

float getFloatFromCin(){
    float val = 0;
    bool cinValid = false; // assume invalid entry

    while (!cinValid){
        cinValid = true; //Assume the cin will be a character.
        cin >> val;
        cout <<val<<"\n";
        if(cin.fail()) // verify a character was entered
        {
            cin.clear(); //clear error
            cin.ignore(); //skip left over stream data.
            cout << "\nPlease enter a numeric value only\n" ;
            cinValid = false; //Try again.
        }
    }
    return val;
    
}

