#ifndef __SDL_H
#define __SDL_H

#include <stdint.h>

typedef union colour_t {
	struct {
		uint8_t a, r, g, b;
	} c;
	uint32_t colour;
} colour_t;

colour_t *sdl_buffer;

void sdl_prepare(uint16_t width, uint16_t height);
void sdl_cleanup();
void sdl_draw();

#endif /* __SDL_H */
