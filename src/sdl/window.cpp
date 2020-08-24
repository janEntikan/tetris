#define FULLSCREEN_ON
#include "window.h"

bool Window::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %/\n", SDL_GetError());
        return false;
    }
    #ifdef FULLSCREEN_ON
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return false;
	}
	int w, h;
	w = dm.w;
	h = dm.h;
	#else
	int w, h;
	w = 800;
	h = 480;
	#endif
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }	
    SDL_RenderSetScale(renderer, w/(8*20), h/(8*20));

    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init( img_flags ) & img_flags)) {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

void Window::flip() {
    SDL_RenderPresent(renderer);
}
    
void Window::clear() {
    SDL_RenderClear(renderer);
}

void Window::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
