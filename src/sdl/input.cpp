#include "input.h"

Input::Input() {
	binds = {
		{"up", SDL_SCANCODE_W},
		{"down", SDL_SCANCODE_S},
		{"left", SDL_SCANCODE_A},
		{"right",SDL_SCANCODE_D},
		{"shoot_up", SDL_SCANCODE_UP},
		{"shoot_down", SDL_SCANCODE_DOWN},
		{"shoot_middle", SDL_SCANCODE_RIGHT},
		{"e", SDL_SCANCODE_E},
		{"f", SDL_SCANCODE_F},
		{"enter", SDL_SCANCODE_RETURN},
		{"space", SDL_SCANCODE_SPACE},
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
