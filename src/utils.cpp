#include "platform.h"
#include "framework.h"
#include "utils.h"
#include "gui.h"
#include "draw.h"
#include "image.h"
#include "application.h"
#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif
#include <iostream>

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

bool checkGLErrors()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}

	return true;
}

//create a window using SDL
SDL_Window* createWindow(const char* caption, int width, int height )
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); //or 24
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//antialiasing (disable this lines if it goes too slow)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8 ); //increase to have smoother polygons

	//create the window
	SDL_Window *window = SDL_CreateWindow(
		caption, 100, 100, width, height,
		SDL_WINDOW_OPENGL);

	if(!window)
	{
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		exit(-1);
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//in case of exit...
	atexit(SDL_Quit);

	//get events from the queue of unprocessed events
	SDL_PumpEvents(); //without this line asserts could fail on windows

	return window;
}

//The application main loop
void launchLoop(Application* app)
{
	SDL_Event sdlEvent;
	Uint last_time = SDL_GetTicks();
	int x,y;

	SDL_GetMouseState(&x,&y);
	app->mouse_position.set(x, y);

#ifdef APP_TIME_ENABLED
	Uint start_time = SDL_GetTicks();
#endif //APP_TIME_ENABLED

	//infinite loop
	while (1)
	{
		//read keyboard state and stored in keystate
		app->keystate = SDL_GetKeyboardState(NULL);

		//render frame
		app->render();

		//update events
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
				{
					case SDL_QUIT: return; break; //EVENT for when the user clicks the [x] in the corner
					case SDL_MOUSEBUTTONDOWN: //EXAMPLE OF sync mouse input
						app->onMouseButtonDown(sdlEvent.button);
						break;
					case SDL_MOUSEBUTTONUP:
						app->onMouseButtonUp(sdlEvent.button);
						break;
					case SDL_KEYDOWN: //EXAMPLE OF sync keyboard input
						app->onKeyPressed(sdlEvent.key);
						break;
				}
		}

		//get mouse position and delta
		app->mouse_state = SDL_GetMouseState(&x,&y);
#ifdef APP_DELTA_ENABLED
		app->mouse_delta.set( app->mouse_position.x - x, app->mouse_position.y - y );
#endif //APP_DELTA_ENABLED
		app->mouse_position.set(x,app->window_height - y);

		//update logic
		Uint now = SDL_GetTicks();
		Uint elapsed_time = (now - last_time);
#ifdef APP_TIME_ENABLED
		app->time = (now - start_time);
#endif //APP_TIME_ENABLED
		app->update(elapsed_time);
		last_time = now;

		//check errors in opengl only when working in debug
		#ifdef _DEBUG
			checkGLErrors();
		#endif
	}

	return;
}

void renderImage( Image* img )
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glDrawPixels(img->width, img->height, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
}
