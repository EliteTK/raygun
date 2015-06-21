#ifndef __SDL_H
#define __SDL_H

#include <stdint.h>

typedef uint8_t channel_t;
typedef uint32_t colour_t;

static inline colour_t argb8888(channel_t a, channel_t r, channel_t g, channel_t b)
{
	return a << 24 | r << 16 | g << 8 | b;
}

colour_t *sdl_buffer;

void sdl_prepare(uint16_t width, uint16_t height);
void sdl_cleanup();
void sdl_draw();

#endif /* __SDL_H */
