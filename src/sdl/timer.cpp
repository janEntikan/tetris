#include "timer.h"

Timer::Timer() {
	start_ticks = 0;
	started = false;
}

void Timer::start() {
	started = true;
	start_ticks = SDL_GetTicks();
}

void Timer::stop() {
	started = false;
	start_ticks = 0;
}

uint32_t Timer::get_ticks() {
	uint32_t time = 0;
	if (started) {
		time = SDL_GetTicks() - start_ticks;
	}
	return time;
}
