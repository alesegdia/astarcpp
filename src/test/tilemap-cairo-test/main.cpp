#include <stdio.h>

#include <SDL2/SDL.h>
#include <cairo.h>
#include <signal.h>

#include "tilemap-cairo-test.h"

cairo_format_t sdl2_to_cairo_format( uint32_t sdl_format )
{
	switch(sdl_format)
	{
	case SDL_PIXELFORMAT_ARGB8888:		return CAIRO_FORMAT_ARGB32;
	case SDL_PIXELFORMAT_RGB888:		return CAIRO_FORMAT_RGB24;
	case SDL_PIXELFORMAT_RGB565:		return CAIRO_FORMAT_RGB16_565;
	//case SDL_PIXELFORMAT_ARGB2101010:	return CAIRO_FORMAT_RGB30;
	default:							return CAIRO_FORMAT_INVALID;
	}
}

int main( int argc, char** argv )
{
	SDL_Window *window;
	SDL_Surface *screen;
	cairo_surface_t* cairo_surface;
	cairo_t* cairo_ctx;
	bool quit = false;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	if( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return -1;
	}

	// avoid CTRL+C intercepted by SDL
	struct sigaction action, old;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = SIG_DFL;
	sigaction(SIGINT, &action, &old);

	window = SDL_CreateWindow("VORONOI", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if( NULL == window )
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	screen = SDL_GetWindowSurface(window);

	cairo_surface = cairo_image_surface_create_for_data(
				static_cast<unsigned char*>(screen->pixels),
				sdl2_to_cairo_format( SDL_GetWindowPixelFormat(window) ),
				screen->w,
				screen->h,
				screen->pitch
				);

	cairo_ctx = cairo_create(cairo_surface);

	AStarTilemapCairoTest test(cairo_ctx);

	while(!quit)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch( event.type )
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
				{
					quit = true;
				}
				test.KeyDown( event.key.keysym.scancode );
				break;
			}
		}
		test.Step();
		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
