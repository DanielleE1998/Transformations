#include <iostream>
#include <cstdlib>
#include <cmath>
#include "sdlInterface.h"
#include "SDL2/SDL.h"
using namespace std;
//__________________________________________________________________________________________________________
void basicDrawLine(int x0, int y0, int x1, int y1){
    //find change in x and y
    int dx = x1 - x0;
    int dy = y1 - y0;

    //cout <<"dx = "<<dx<<" and dy = "<<dy<<"\n";
    // Depending on the absolute value of dx and dy, find steps
    int steps;
    if ((abs(dx) >= abs(dy))) //greater change in abs(dx)
    steps = abs(dx);
    else   // greater change in abs(dy) 
    steps = abs(dy);

    //cout <<"steps = "<<steps<<"\n";
    // calculate increment in x & y for each steps
    if (steps){
        float xInc = dx / (float)steps;
        float yInc = dy / (float)steps;
        //cout << "x inc = " <<xInc << "\n  ";
        //cout << "y inc = " <<yInc << "\n  ";


        //set initial values and begin placing a pixel for each step
        float x = x0;
        float y = y0;
        for (int i = 0; i <= steps; i++)
        {
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            x += xInc;
            y += yInc;
        }
    }

    else{
        //just render 1 point 
        SDL_RenderDrawPoint(renderer, (int)x0, (int)y0);
    }
}//end drawline
//________________________________________________________________________________________________________
int displayObject(objectData obj){
    int lineCount = obj.oLines.size();
    cout <<"lineCount "<<lineCount<<endl;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    // remember to make sure coordinates do not exceeld WINDOW_WIDTH
    if(window==NULL)
    {  
        cout <<"Could not create window.\n";
        return 1;
    }
                  
    for (int i=0;i<lineCount;i++){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        basicDrawLine((int)round(obj.oProjection.at(i).x0), (int)round(obj.oProjection.at(i).y0),
                      (int)round(obj.oProjection.at(i).x1), (int)round(obj.oProjection.at(i).y1)); 
        if (obj.tProjection.size()){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            basicDrawLine((int)round(obj.tProjection.at(i).x0), (int)round(obj.tProjection.at(i).y0),
                          (int)round(obj.tProjection.at(i).x1), (int)round(obj.tProjection.at(i).y1));   
        }
    }
    
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;  //success
}