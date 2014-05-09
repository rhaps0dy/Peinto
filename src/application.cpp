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

	curPolyline = NULL;
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
	if(curPolyline) destroyPolyline(curPolyline);
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
	if(gs.dwg) {
		switch(gs.tool) {
			case FREEHAND:
			case SHAPE:
			case POLYGONAL:
				drawPolyline(curPolyline, img, Color::RED);
			case LINE:
				drawLine(img, gs.lastMDown, mouse_position, Color::RED);
				break;
			case CIRCLE:
				drawCircle(img, gs.lastMDown, mouse_position.distTo(&gs.lastMDown),
					Color::RED, window_width, window_height);
		}
	}
	renderImage(img);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
	if(gs.dwg && gs.tool == FREEHAND)
	{
		addPosPolyline(curPolyline, mouse_position);
		gs.lastMDown = mouse_position;
	}
}

//keyboard press event
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_f: gs.tool = FREEHAND; break;
		case SDLK_c: gs.tool = CIRCLE; break;
		case SDLK_l: gs.tool = LINE; break;
		case SDLK_p: gs.tool = POLYGONAL; break;
		case SDLK_s: gs.tool = SHAPE; break;
		case SDLK_RETURN:
			if(gs.dwg) gs.dwg = 0;
			if(gs.tool == SHAPE)
				addPosPolyline(curPolyline, curPolyline->f->p);
			if(gs.tool == SHAPE || gs.tool == POLYGONAL)
				drawPolyline(curPolyline, canvas, Color::WHITE);
		/*case SDLK_ESCAPE: break;*/
	}
	if(curPolyline) {
		destroyPolyline(curPolyline);
		curPolyline = NULL;
		gs.dwg = 0;
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if(!gs.dwg) {
		gs.dwg = -1;
		if(gs.tool == POLYGONAL || (gs.tool == FREEHAND || gs.tool == SHAPE))
			curPolyline = newPolyline(mouse_position);
	}
	else {
		switch(gs.tool) {
			case POLYGONAL:
			case SHAPE:
			case FREEHAND:
				addPosPolyline(curPolyline, mouse_position);
				break;
			case LINE:
				gs.dwg = 0;
				drawLine(canvas, gs.lastMDown, mouse_position, Color::WHITE);
				break;
			case CIRCLE:
				gs.dwg = 0;
				drawCircle(canvas, gs.lastMDown, mouse_position.distTo(&gs.lastMDown),
					Color::WHITE, window_width, window_height);
			default: break;
		}
	}
	gs.lastMDown = mouse_position;
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if(gs.tool == FREEHAND && gs.dwg)
	{
		gs.dwg = 0;
		drawPolyline(curPolyline, canvas, Color::WHITE);
		destroyPolyline(curPolyline);
	}
}

//when the app starts
void Application::start()
{
	launchLoop(this);
}
