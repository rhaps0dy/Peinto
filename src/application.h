/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

/*requires
"platform.h"
"framework.h"
"image.h"
"gui.h"
"draw.h"
*/

class Application
{
public:
	//window
	SDL_Window* window;
	Float window_width;
	Float window_height;

	//world renderer image
	Image *img;
	//We paint in this image
	Image *canvas;
	GUIState gs;
	Polyline *curPolyline;
	ColorSelector colorSelector;
	char checkColor;

#ifdef APP_TIME_ENABLED
	Uint time;
#endif //APP_TIME_ENABLED

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Pos2 mouse_position; //last mouse position
#ifdef APP_DELTA_ENABLED
	Vector2 mouse_delta; //mouse movement in the last frame
#endif //APP_DELTA_ENABLED

	//constructor and destructor
	Application(const char* caption, int width, int height);
	~Application();

	//main methods
	void init( void );
	void render( void );
	void update( Uint dt );

	//methods for events
	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp( SDL_MouseButtonEvent event );

	//other methods to control the app
	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
};
