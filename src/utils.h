/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
/*requires
"includes.h"
"framework.h"
*/

//General functions **************
class Application;
class Image;

//check opengl errors
bool checkGLErrors();

SDL_Window* createWindow(const char* caption, int width, int height);
void launchLoop(Application* app);

void renderImage(Image* img);

inline Float _random() { return (rand() % 10000) / 10000.0; }
