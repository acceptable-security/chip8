#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef void (*chip8_timer_cb_t)();

typedef struct {
	uint8_t tick;
	time_t start;
	double rate; // Should be 1/60 for 60Hz
	chip8_timer_cb_t cb;
} chip8_timer_t;

bool chip8_timer_tick(chip8_timer_t* timer);
void chip8_timer_set(chip8_timer_t* timer, uint8_t value);