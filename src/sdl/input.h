#ifndef INPUT_H
#define INPUT_H
#include <map>
#include <vector>
#include <iostream>
#include "SDL2/SDL.h"

class Input {
public:
    Input();
	std::map<std::string, int> keys;
	void read_input();

private:
	SDL_Event event;
	std::vector<std::pair<std::string, SDL_Scancode>> binds;
};

#endif
