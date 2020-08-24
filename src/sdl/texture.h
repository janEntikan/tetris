#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "fileio.h"

class Texture {
public:
    Texture();
    Texture(SDL_Renderer*, std::string, std::string);
	std::string filename;
    SDL_Renderer* renderer = NULL;
	std::map<std::string, std::vector<int>> dat;
    int texture_width;
    int texture_height;	
    bool load_texture_file(std::string);
    void destroy();
    void set_animation_size(int, int);
    void draw_animation(int, int, int, int, int);
	void draw(int, int, int, int, int, int, int, int);

private:
	int sprite_width;
	int sprite_height;
    SDL_Texture* sdl_texture;
};

#endif
