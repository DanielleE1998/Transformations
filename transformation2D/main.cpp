#include <cmath>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <vector>
#define WINDOW_WIDTH 600
#define PI 3.14159265358979323846  
using namespace std;
SDL_Window     *window;
SDL_Renderer *renderer;
enum transformationType {basicTranslate,basicScale,basicRotate,
                        scale,rotate};
struct lineData{
    int x0;
    int y0;
    int x1;
    int y1;
    int x0t;
    int y0t;
    int x1t;
    int y1t;
    transformationType tt;
};

//Matricies
float transformationMatrix[3][3] = {{1,2,3},
                                    {4,5,6},
                                    {7,8,9}};
float operationMatrix[3][3] =      {{1,2,3},
                                    {4,5,6},
                                    {7,8,9}};
float translationMatrix[3][3] = {{1,0,0}, //1 0  tx 
                                 {0,1,0}, //0 1  ty 
                                 {0,0,1}};//0 0  1
float scalingMatrix[3][3] = {{1,0,0}, //  Sx 0  0
                             {0,1,0}, //  0  Sy 0
                             {0,0,1}};
float rotationMatrix[3][3] = {{0,0,0}, //cos  -sin 0
                              {0,0,0}, //sin  cos 0
                              {0,0,1}};// 0    0  1
                               

//Update/Display_______________________________________________________________________________________________________
void basicDrawLine(int x0, int y0, int x1, int y1){
    x0 = x0 + WINDOW_WIDTH/2;
    x1 = x1 + WINDOW_WIDTH/2;  //move x's to center

    y0 = WINDOW_WIDTH/2 - y0;
    y1 = WINDOW_WIDTH/2 - y1;

    //find change in x and y
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps;
    if ((abs(dx) >= abs(dy))) //greater change in abs(dx)
        steps = abs(dx);
    else   // greater change in abs(dy) 
        steps = abs(dy);
    if (steps){
        float xInc = dx / (float)steps;
        float yInc = dy / (float)steps;

        //set initial values and begin placing a pixel for each step
        float x = x0;
        float y = y0;
        for (int i = 0; i <= steps; i++){
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            x += xInc;
            y += yInc;
        }
    }
}

void updateLine(lineData &line){
     line.x0t = line.x0*transformationMatrix[0][0] +
                   line.y0*transformationMatrix[0][1]+
                   1*transformationMatrix[0][2];
                   
        line.y0t = line.x0*transformationMatrix[1][0] +
                   line.y0*transformationMatrix[1][1]+
                   1*transformationMatrix[1][2];
                   
        line.x1t = line.x1*transformationMatrix[0][0] +
                   line.y1*transformationMatrix[0][1]+
                   1*transformationMatrix[0][2];
                  
        line.y1t = line.x1*transformationMatrix[1][0] +
                   line.y1*transformationMatrix[1][1]+
                   1*transformationMatrix[1][2];  
}
//applies the transformation matrix to the lines that appear in “datalines”
void ApplyTransformation(vector<lineData> &dataLines, int num){ //issue with pass by value, local to method
    cout <<"in apply transform dataLines address "<<&dataLines<<"\n";
    for (int i = 0; i < num; i++){
        //lineData line = dataLines.at(i);
       updateLine(dataLines.at(i));
       
        //dataLines.push_back(line);
        cout <<"after transformation: ";
        cout <<"in vector "<<dataLines.at(i).x0t<<"\n";
    }
}
 

//displays (i.e., scan-converts) ‘datalines’ containing `num' lines
void Displaypixels(vector<lineData> &dataLines, int num){
    //SDL stuff
                SDL_Event event;
                SDL_Init(SDL_INIT_VIDEO);
                SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
                
                // need to make sure coordinates do not exceeld WINDOW_WIDTH
                if(window==NULL)
                    cout <<"Could not create window: "<<SDL_GetError()<<"\n";

                 //apply transformation and draw the lines//transform input lines:
                SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
                basicDrawLine((int)(-WINDOW_WIDTH/2)+1,0,(int)(WINDOW_WIDTH/2)-1,0); //draw x axis
                basicDrawLine(0,(WINDOW_WIDTH/2)-1,0,(-WINDOW_WIDTH/2)+1); //draw y axis
                   
                
                for (int i = 0; i < num; i++){
                    cout <<"show line at "<< i <<"\n";
                    lineData line = dataLines.at(i);
                    if  (abs(line.x0) < (WINDOW_WIDTH/2) &&
                         abs(line.y0) < (WINDOW_WIDTH/2) &&
                         abs(line.x1) < (WINDOW_WIDTH/2) &&
                         abs(line.y1) < (WINDOW_WIDTH/2)) {
                             
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//before
                            basicDrawLine((int)line.x0, (int)line.y0,
                                        (int)line.x1, (int)line.y1); //before
                                        
                            SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);//after
                            basicDrawLine((int)line.x0t, (int)line.y0t,
                                        (int)line.x1t, (int)line.y1t); //after
                      }
                    else
                       cout <<"Skiping line at position "<< i <<"coordinates out of range\n";
                 
                }
                SDL_RenderPresent(renderer);
                while (1) {
                        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                        break;
                }
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
}

void showMatrix(float matrix[3][3]){
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
                cout <<"i = "<<i<<" j = "<<j<< "; holds value = "<<matrix[i][j]<<"\n";
}
 

float sumOfProducts(int row,int col){
    return  (operationMatrix[row][0]*transformationMatrix[0][col] +
            operationMatrix[row][1]*transformationMatrix[1][col] +
            operationMatrix[row][2]*transformationMatrix[2][col]);
}

void updateTransformation(float operation[3][3]){
    //copy operation to operationMatrix
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
            operationMatrix[i][j]=operation[i][j];

    float result[3][3];    
    result[0][0] = sumOfProducts(0,0);
    result[0][1] = sumOfProducts(0,1);
    result[0][2] = sumOfProducts(0,2);
    result[1][0] = sumOfProducts(1,0);
    result[1][1] = sumOfProducts(1,1);
    result[1][2] = sumOfProducts(1,2);
    result[2][0] = sumOfProducts(2,0);
    result[2][1] = sumOfProducts(2,1);
    result[2][2] = sumOfProducts(2,2);
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            transformationMatrix[i][j]=result[i][j];
        }
    }
}

//Transformations___________________________________________________________________________________________________
//`Tx' is the horizontal and `Ty' is the vertical displacements.
void BasicTranslate(float Tx, float Ty){
    translationMatrix[0][2] = Tx;
    translationMatrix[1][2] = Ty;
    
    updateTransformation(translationMatrix);
    cout <<"\nupdated transformation matrix:\n";
    showMatrix(transformationMatrix);
}

//`Sx' and `Sy' are the horizontal and vertical scaling factors;
//center of scale is at the origin of the Coordinate System.
void BasicScale(float Sx, float Sy){
    scalingMatrix[0][0] = Sx;
    scalingMatrix[1][1] = Sy;

    updateTransformation(scalingMatrix);
    cout <<"\nupdated transformation matrix:\n";
    showMatrix(transformationMatrix);
}

//angle of rotation is `angle' degrees (clockwise); 
//center of rotation is at the origin of the Coordinate System.
void BasicRotate(int angle){
    rotationMatrix[0][0] = cos(angle*PI/180);
    rotationMatrix[0][1] = -sin(angle*PI/180);
    rotationMatrix[1][0] = sin(angle*PI/180);
    rotationMatrix[1][1] = cos(angle*PI/180);
   
    updateTransformation(rotationMatrix);
    cout <<"\nupdated transformation matrix:\n";
    showMatrix(transformationMatrix);
}

//`Sx' and `Sy' are the horizontal and vertical scaling factors; 
//center of scale is at Cx, Cy.
void Scale(float Sx, float Sy, float Cx, float Cy){
    BasicTranslate(-Cx,-Cy);
    BasicScale(Sx, Sy);
    BasicTranslate(Cx, Cy);
}

//angle of rotation is `angle' degrees (clockwise); 
//center of rotation is at Cx, Cy.
void Rotate(float angle, float Cx, float Cy){
    BasicTranslate(-Cx,-Cy);
    BasicRotate(angle);
    BasicTranslate(Cx, Cy);
}

//Input and Output___________________________________________________________________________________________________________________
//reads ‘datalines’ from an external file (name of file is provided by the user). 
//on return `num' will contain the number of lines read from the file. 
int Inputlines(vector<lineData> &dataLines, int num){
    ifstream inFile;
    inFile.open("C:\\Users\\Dell\\Desktop\\datafile.txt");
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        sleep(1);
        exit(1); 
    }    
    while (!inFile.eof()){
        lineData line;
        int i;
        inFile >> i;
        line.x0 = i;
        inFile >> i;
        line.y0 = i;
        inFile >> i;
        line.x1 = i;
        inFile >> i;
        line.y1 = i;
        line.x0t= line.x0;
        line.y0t= line.y0;
        line.x1t= line.x1;
        line.y1t= line.y1;
        dataLines.push_back(line);
        cout<< "Inputs: (" << line.x0 <<", "<< line.y0 <<"), (" << line.x1 <<", "<<line.y1 <<") \n";
        num++;
    }
     inFile.close();
     return num;
}


//Outputs ‘datalines’ containing `num' lines to an external file 
//(name of file is provided by the user). Embed the functions above together.
void Outputlines(vector<lineData> &dataLines, int num){
    std::ofstream out("C:\\Users\\Dell\\Desktop\\output.txt"); 
    
    cout <<"in output dataLines address "<<&dataLines<<"\n";
    lineData line;
    for(int i = 0; i < num; i++){
        line = dataLines.at(i);
        cout << "Outputs: (" << line.x0t <<", "<< line.y0t <<"), (" << line.x1t <<", "<<line.y1t <<") \n";
        out << "Outputs: (" << line.x0t <<", "<< line.y0t <<"), (" << line.x1t <<", "<<line.y1t <<") \n";
    }
}
 
//Main___________________________________________________________________________________________________________________________________
int main(int argc, char** argv){
    vector<lineData> dataLines;
    int num = 0;
    num = Inputlines(dataLines, num);
    cout <<"in main dataLines address "<<&dataLines<<"\n";
    float tx, ty, sx, sy, angle, cx, cy;

        //init transformation matrix
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++){
                    if (i==j)
                        transformationMatrix[i][j]=1.0;
                    else
                        transformationMatrix[i][j]=0.0;
            }
        }
        
    sleep(3);
    char choice;
    bool quit= false;
    while (!quit){ 
        cout <<"\nEnter desired operation:\n";
        cout << "(a) basic translation     (b) basic scaling      (c) basic rotation\n";
        cout << "(d) scale                 (e) rotate             (f) draw lines\n";
        cout << "(q) quit\n";
        cin >> choice;
        switch (choice){
        case'a':  
                cout <<"enter horizontal displacement: ";
                cin >> tx;
                cout <<"enter vertical displacement: ";
                cin >> ty;
                BasicTranslate(tx, ty);
                ApplyTransformation(dataLines, num);  
            
                
                break;
                 
        case'b': 
                cout <<"enter horizontal scale factor: ";
                cin >> sx;
                cout <<"enter vertical scale factor: ";
                cin >> sy;
                BasicScale(sx, sy); 
                cout <<"about to apply transform dataLines address "<<&dataLines<<"\n";
                
                ApplyTransformation(dataLines, num); 
                 cout <<"after apply transform dataLines address "<<&dataLines<<"\n";
                break;  
        case'c': 
                 cout <<"enter rotation angle\n";
                 cin >> angle;
                 BasicRotate(angle);
                 ApplyTransformation(dataLines, num); 
                 break;
                 
        case 'd': 
                cout <<"enter horizontal scale factor: ";
                cin >> sx;
                cout <<"enter vertical scale factor: ";
                cin >> sy;
                cout <<"enter center's x coordinate: ";
                cin >> cx;
                cout <<"enter center's y coordinate: ";
                cin >> cy;
                Scale(sx, sy, cx, cy); 
                ApplyTransformation(dataLines, num); 
                break;
                
        case 'e':
                cout <<"enter rotation angle: ";
                cin >> angle;
                cout <<"enter center's x coordinate: ";
                cin >> cx;
                cout <<"enter center's y coordinate: ";
                cin >> cy;
                Rotate(angle, cx, cy); 
                ApplyTransformation(dataLines, num); 
                break;

        case 'f':   
                Displaypixels(dataLines, num);
                break;
                
        case 'q':   
                quit = true;
                break;
        }
    }
    cout <<"about to output lines dataLines address "<<&dataLines<<"\n";
    sleep(2);
    Outputlines(dataLines, num);
    return EXIT_SUCCESS;
}

