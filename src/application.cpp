#include "platform.h"
#include <math.h>
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

	curLLPos2 = NULL;
	colorSelector.c = Color::BLACK;
	colorSelector.margins = 3;
	colorSelector.h = 50;
	colorSelector.w = 300;
	colorSelector.x = 0;
	colorSelector.y = window_height - colorSelector.h;
	colorSelector.visible = ~0;
	
	checkColor = 0;
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	showREADME();
	img = new Image(window_width, window_height);
	canvas = new Image(window_width, window_height);
	canvas->fill(Color::WHITE);
}

Application::~Application()
{
	if(curLLPos2) LLPos2Destroy(curLLPos2);
	delete img;
	delete canvas;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	memcpy(img->pixels, canvas->pixels, window_width*window_height*sizeof(Color));
	if(gs.dwg) {
		switch(gs.tool) {
			case FREEHAND:
			case SHAPE:
			case POLYGONAL:
				LLPos2Draw(curLLPos2, img, colorSelector.c);
			case LINE:
				drawLine(img, gs.lastMDown, mouse_position, colorSelector.c);
				break;
			case CIRCLE:
				drawCircle(img, gs.lastMDown, mouse_position.distTo(&gs.lastMDown),
					colorSelector.c);
			default: ;
		}
	}
	drawColorSelector(&colorSelector, img);
	renderImage(img);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
	if(gs.dwg && gs.tool == FREEHAND)
	{
		LLPos2Add(curLLPos2, mouse_position);
		gs.lastMDown = mouse_position;
	}
	if(checkColor) handleClick(&colorSelector, mouse_position);
}

//keyboard press event
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_q: exit(0);
		case SDLK_v: colorSelector.visible = !colorSelector.visible; break;
		case SDLK_b: gs.tool = BUCKET; break;
		case SDLK_f: gs.tool = FREEHAND; break;
		case SDLK_c: gs.tool = CIRCLE; break;
		case SDLK_l: gs.tool = LINE; break;
		case SDLK_p: gs.tool = POLYGONAL; break;
		case SDLK_s: gs.tool = SHAPE; break;
		case SDLK_RETURN:
			if(gs.tool == SHAPE)
				LLPos2Add(curLLPos2, curLLPos2->f->p);
			if(gs.tool == SHAPE || gs.tool == POLYGONAL)
				LLPos2Draw(curLLPos2, canvas, colorSelector.c);
		/*case SDLK_ESCAPE: break;*/
	}
	if(curLLPos2) {
		LLPos2Destroy(curLLPos2);
		curLLPos2 = NULL;
	}
	gs.dwg = 0;
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if(checkColor = handleClick(&colorSelector, mouse_position)) return;
	if(gs.tool == BUCKET) {
		fill(canvas, mouse_position, colorSelector.c);
		return;
	}
	if(!gs.dwg) {
		gs.dwg = -1;
		if(gs.tool == POLYGONAL || (gs.tool == FREEHAND || gs.tool == SHAPE))
		{
			curLLPos2 = LLPos2New();
			LLPos2Add(curLLPos2, mouse_position);
		}
	}
	else {
		switch(gs.tool) {
			case POLYGONAL:
			case SHAPE:
			case FREEHAND:
				LLPos2Add(curLLPos2, mouse_position);
				break;
			case LINE:
				gs.dwg = 0;
				drawLine(canvas, gs.lastMDown, mouse_position, colorSelector.c);
				break;
			case CIRCLE:
				gs.dwg = 0;
				drawCircle(canvas, gs.lastMDown, mouse_position.distTo(&gs.lastMDown),
					colorSelector.c);
			default: ;
		}
	}
	gs.lastMDown = mouse_position;
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if(gs.tool == FREEHAND && gs.dwg)
	{
		gs.dwg = 0;
		LLPos2Draw(curLLPos2, canvas, colorSelector.c);
		LLPos2Destroy(curLLPos2);
		curLLPos2 = NULL;
	}
	checkColor = 0;
}

//when the app starts
void Application::start()
{
	launchLoop(this);
}
