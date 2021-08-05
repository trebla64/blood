#include "stdafx.h"
#include "error.h"
#include "debug4g.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

static SDL_Surface *screen = NULL;

extern int BloodMain(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	dprintf("We are here...\n");
/*
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		ThrowError("Failed initializing SDL");
		return 0;
	}
	screen = SDL_SetVideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 8, SDL_SWSURFACE);
	if (screen == NULL)
	{
		ThrowError("Failed setting video mode");
		SDL_Quit();
		return 0;
	}
	int res = BloodMain(argc, argv);
	SDL_Quit();
*/
	return 0;
}
