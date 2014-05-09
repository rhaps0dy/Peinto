#include "application.h"
#include "utils.h"
#include "image.h"
#include "readme.h"
#include "bresenham.h"

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
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	showREADME();
	img = new Image(window_width, window_height);
}

Application::~Application()
{
	delete img;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawLine(img, Pos2(50, 0), Pos2(50, 100), &Color::RED);
	drawLine(img, Pos2(200, 50), Pos2(100, 50), &Color::RED);
	drawLine(img, Pos2(0, 150), Pos2(100, 150), &Color::RED);
	drawLine(img, Pos2(150, 200), Pos2(150, 100), &Color::RED);
	drawLine(img, Pos2(0, 200), Pos2(100, 300), &Color::RED);
	drawLine(img, Pos2(200, 300), Pos2(100, 200), &Color::RED);
	drawLine(img, Pos2(0, 400), Pos2(100, 300), &Color::RED);
	drawLine(img, Pos2(200, 300), Pos2(100, 400), &Color::RED);
	drawLine(img, Pos2(300, 100), Pos2(400, 150), &Color::GREEN);
	drawLine(img, Pos2(400, 150), Pos2(500, 100), &Color::GREEN);
	drawLine(img, Pos2(500, 100), Pos2(400, 50), &Color::GREEN);
	drawLine(img, Pos2(400, 50), Pos2(300, 100), &Color::GREEN);
	drawLine(img, Pos2(400, 200), Pos2(350, 300), &Color::WHITE);
	drawLine(img, Pos2(350, 300), Pos2(400, 400), &Color::WHITE);
	drawLine(img, Pos2(400, 400), Pos2(450, 300), &Color::WHITE);
	drawLine(img, Pos2(450, 300), Pos2(400, 200), &Color::WHITE);

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
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
}

//when the app starts
void Application::start()
{
	launchLoop(this);
}

void Application::setWindowSize(int width, int height)
{
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}
