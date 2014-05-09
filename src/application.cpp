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

	c.set(255.,255.,255.);
	p1.set(400, 400);
	p2.set(400, 400);
	whichDirection = 0;
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


	img->fill(Color::BLUE);
	drawLine(img, p1, p2, &c);
	drawCircle(img, p2, p2Timer/100, &Color::RED);
	renderImage(img);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
	p2Timer += dt;
	while(p2Timer > 10000) {
		p2Timer -= 10000;
		whichDirection = (whichDirection+1)%4;
	}
	switch(whichDirection) {
		case 0:
			p2.set(500, 300+p2Timer/50);
			break;
		case 1:
			p2.set(500-p2Timer/50, 500);
			break;
		case 2:
			p2.set(300, 500-p2Timer/50);
			break;
		case 3:
			p2.set(300+p2Timer/50, 300);
			break;
	}
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
	img->resizeNoCopy(width, height);
}
