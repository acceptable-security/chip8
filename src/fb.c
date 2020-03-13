#include <string.h>

#include "fb.h"

void chip8_fb_clear(chip8_fb_t* fb) {
	memset(fb->pixels, 0, CHIP8_FB_WIDTH * CHIP8_FB_HEIGHT);
}

uint8_t chip8_fb_render_sprite(
	chip8_fb_t* fb,
	uint8_t x, uint8_t y,
	uint8_t* data, uint8_t len
) {
	uint8_t of = 0;

	for ( uint8_t i = 0; i < len; i++ ) {
		uint8_t bin = data[i];

		for ( uint8_t j = 0; j < 8; j++ ) {
			uint8_t old = fb->pixels[CHIP8_FB_POS(i, j)];
			uint8_t next = (bin >> j) & 1;

			fb->pixels[CHIP8_FB_POS(i, j)] = next;

			if ( old == 1 && next == 0 ) {
				of = 1;
			}
		}
	}

	return of;
}