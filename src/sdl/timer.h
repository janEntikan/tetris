#ifndef TIMER_H
#define TIMER_H
#include <stdlib.h>
#include "SDL2/SDL.h"

class Timer {
public:
	Timer();
	void start();
	void stop();
	uint32_t get_ticks();
private:
	uint32_t start_ticks;
	bool started;
};


#endif
