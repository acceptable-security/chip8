#include "timer.h"

bool chip8_timer_tick(chip8_timer_t* timer) {
	if ( timer->tick == 0 ) {
		return true;
	}

	time_t now = time(NULL);
	double diff = difftime(now, timer->start);

	if ( diff >= timer->rate ) {
		return true;
	}

	timer->tick--;

	if ( timer->cb != NULL ) {
		timer->cb();
	}

	return false;
}

void chip8_timer_set(chip8_timer_t* timer, uint8_t value) {
	timer->tick = value;
	timer->start = time(NULL);
}