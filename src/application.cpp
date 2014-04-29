#include "application.h"
#include "utils.h"
#include "image.h"
#include "readme.h"

Application::Application(const char* caption, int width, int height) :
	doMove(false), colorSelectorHeight(30)
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
	wp = new WavePool(window_width, window_height - colorSelectorHeight);
	colSel = new ColorSelect(0, window_height - colorSelectorHeight, window_width, window_height);
	colSel->paintOn(img);
}

Application::~Application()
{
	delete img;
	delete wp;
	delete colSel;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wp->generateNewImage(img);
	renderImage(img);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(Uint dt)
{
	wp->update(dt);
	if(doMove)
		wp->move(&mouse_position);
}

//keyboard press event
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDLK_p:
			wp->togglePause();
			break;
		case SDLK_a:
			wp->add(&mouse_position);
			break;
		case SDLK_d:
			wp->remove();
			break;
		case SDLK_q:
			wp->addWL(10);
			break;
		case SDLK_z:
			wp->rmWL(10);
			break;
		case SDLK_w:
			wp->addAmp(10);
			break;
		case SDLK_x:
			wp->rmAmp(10);
			break;
		case SDLK_e:
			wp->addSpd(10);
			break;
		case SDLK_c:
			wp->rmSpd(10);
			break;
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	Color c = colSel->getColor(&mouse_position);
	if(c!=ColorSelect::NONE)
	{
		wp->setColor(&c);
		return;
	}

	wp->select(&mouse_position);
	doMove = true;
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	doMove = false;
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
	wp->resize(width, height - colorSelectorHeight);
	colSel->setCoords(0, window_height - colorSelectorHeight, window_width, window_height);
	colSel->paintOn(img);
}
