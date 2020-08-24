#include <stdlib.h> 
#include "texture.h"

SDL_Rect CLIPRECT = 	{0, 0, 16, 16};
SDL_Rect RENDERQUAD = 	{0, 0, 16, 16};

Texture::Texture() {
    sdl_texture = NULL;
}

Texture::Texture(SDL_Renderer* sdl_renderer, std::string texture_file, std::string dat_file) {
    renderer = sdl_renderer;
    sdl_texture = NULL;
    load_texture_file(texture_file);
	dat = load_dat(dat_file);
}

bool Texture::load_texture_file(std::string path) {
    destroy();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (new_texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    texture_width = loaded_surface->w;
    texture_height = loaded_surface->h;
    sprite_width = texture_width;
    sprite_height = texture_height;
    SDL_FreeSurface(loaded_surface);
    sdl_texture = new_texture;
    return sdl_texture != NULL;
}

void Texture::set_animation_size(int w, int h) {
	sprite_width = w;
	sprite_height = h;
}

void Texture::destroy() {
    if (sdl_texture != NULL){
        SDL_DestroyTexture(sdl_texture);
        sdl_texture = NULL;
    }
}

void Texture::draw_animation(int x, int y, int w, int h, int frame) {
	RENDERQUAD.x = x;
	RENDERQUAD.y = y;
	RENDERQUAD.w = w;
	RENDERQUAD.h = h;
	if ((sprite_width == texture_width) && (sprite_height == texture_height)) {
		SDL_RenderCopy(renderer, sdl_texture, NULL, &RENDERQUAD);
	} else {
		CLIPRECT.x = (frame*sprite_width)%texture_width;
		CLIPRECT.y = ((frame*sprite_width)/texture_width)*sprite_height;
		CLIPRECT.w = sprite_width;
		CLIPRECT.h = sprite_height;
		RENDERQUAD.w = CLIPRECT.w;
		RENDERQUAD.h = CLIPRECT.h;
		SDL_RenderCopy(renderer, sdl_texture, &CLIPRECT, &RENDERQUAD);
	}
}

void Texture::draw(int ix, int iy, int iw, int ih, int ox, int oy, int ow, int oh) {
	RENDERQUAD.x = ix;
	RENDERQUAD.y = iy;
	RENDERQUAD.w = iw;
	RENDERQUAD.h = ih;
	CLIPRECT.x = ox;
	CLIPRECT.y = oy;
	CLIPRECT.w = ow;
	CLIPRECT.h = oh;
	SDL_RenderCopy(renderer, sdl_texture, &CLIPRECT, &RENDERQUAD);
}
