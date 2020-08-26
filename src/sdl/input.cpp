#include "input.h"

Input::Input() {
	binds = {
		{"up", SDL_SCANCODE_UP},
		{"down", SDL_SCANCODE_DOWN},
		{"left", SDL_SCANCODE_LEFT},
		{"right",SDL_SCANCODE_RIGHT},
		{"rotate_l", SDL_SCANCODE_Z},
		{"rotate_r", SDL_SCANCODE_X},
    {"quit", SDL_SCANCODE_ESCAPE},
	};

	for (auto& bind : binds) {
		keys[bind.first] = 0;
	}
}

void Input::read_input() {
    while(SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
			keys["quit"] = 2;
        }
    }
	const Uint8* keyboard_state = SDL_GetKeyboardState( NULL );
	for (int k=0; k<binds.size(); k++) {
		std::pair<std::string, SDL_Scancode> key = binds[k];
		if (keyboard_state[key.second]) {
			if (keys[key.first] == 0) {
				keys[key.first] = 2;
			} else if (keys[key.first] == 2) {
				keys[key.first] = 1;
			}
		} else {
			keys[key.first] = 0;
		}
	}
}
