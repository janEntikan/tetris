#ifndef WINDOW_H
#define WINDOW_H
#include "SDL2/SDL.h"

class Window {
public:
    char* title;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    bool initialize();
    void flip();
    void clear();
    void destroy();
};

#endif
