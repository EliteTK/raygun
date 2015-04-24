#include <stdint.h>
#include <SDL2/SDL.h>
#include "sdl.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *screen;

uint16_t bwidth, bheight;

void sdl_prepare(uint16_t width, uint16_t height)
{
	SDL_Init(SDL_INIT_VIDEO);
	atexit(sdl_cleanup);

	window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	sdl_buffer = malloc(sizeof *sdl_buffer * width * height);

	bwidth = width;
	bheight = height;
}

void sdl_cleanup()
{
	free(sdl_buffer);
	SDL_Quit();
}

void sdl_draw()
{
	SDL_UpdateTexture(screen, NULL, sdl_buffer, bwidth * sizeof *sdl_buffer);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, screen, NULL, NULL);
	SDL_RenderPresent(renderer);
}
