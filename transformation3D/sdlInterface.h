#include "SDL2/SDL.h"
#include "transformations.h"
extern float screenZ;
#define WINDOW_WIDTH screenZ*2
#define WINDOW_HEIGHT screenZ*2
extern SDL_Window   *window;
extern SDL_Renderer *renderer;
extern void basicDrawLine(int x0, int y0, int x1, int y1);
extern int displayLines(vector<lineData> datalines); // do not pass address & 
extern int displayObject(objectData object); //since we are not modifying this data