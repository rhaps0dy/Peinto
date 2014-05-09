#include "platform.h"
#include "framework.h"
#include "image.h"
#include "gui.h"
#include "utils.h"
#include "draw.h"
#include "application.h"

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);

	gs.lastMDown = mouse_position;
	gs.tool = FREEHAND;
	gs.dwg = 0;
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	showREADME();
	img = new Image(window_width, window_height);
	canvas = new Image(window_width, window_height);
	canvas->fill(Color::BLACK);
}

Application::~Application()
{
	delete img;
	delete canvas;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	img->fill(Color::BLACK);
	memcpy(img->pixels, canvas->pixels, window_width*window_height*sizeof(Color));
	if(gs.dwg && (gs.tool == LINE || gs.tool == POLYGON))
		drawLine(img, gs.lastMDown, mouse_position, Color::RED);
	renderImage(img);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
}

//keyboard press event
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_f: gs.tool = FREEHAND; break;
		case SDLK_c: gs.tool = CIRCLE; break;
		case SDLK_l: gs.tool = LINE; break;
		case SDLK_p: gs.tool = POLYGON; break;
		case SDLK_ESCAPE: if(gs.dwg) gs.dwg = 0; break;
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if(!gs.dwg) {
		gs.dwg = -1;
	}
	else {
		switch(gs.tool) {
			case LINE:
				gs.dwg = 0;
			case POLYGON:
				drawLine(canvas, gs.lastMDown, mouse_position, Color::WHITE);
				break;
			default: break;
		}
	}
	gs.lastMDown = mouse_position;
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if(gs.tool == FREEHAND && gs.dwg) gs.dwg = 0;
}

//when the app starts
void Application::start()
{
	launchLoop(this);
}
