#include <stdint.h>

#define CHIP8_FB_WIDTH 64
#define CHIP8_FB_HEIGHT 32
#define CHIP8_FB_POS(X, Y) ((X) + ((Y) * CHIP8_FB_WIDTH))

typedef struct {
	uint8_t pixels[CHIP8_FB_WIDTH * CHIP8_FB_HEIGHT];
} chip8_fb_t;

void chip8_fb_clear(chip8_fb_t* fb);
uint8_t chip8_fb_render_sprite(
	chip8_fb_t* fb,
	uint8_t x, uint8_t y,
	uint8_t* data, uint8_t len
);