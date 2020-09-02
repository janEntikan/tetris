#include "text.h"


void Text::print(SDL_Renderer* renderer,  std::string string, int x, int y) {
  draw_rect.x = x; 
  draw_rect.y = y;
  draw_rect.w = 1;
  draw_rect.h = 1;
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); 
  int alphabet_index = 0;
  for (auto& string_letter : string) {
    alphabet_index = 0;
    alphabet_index = alphabet.find_first_of(string_letter);
    for (int cy = 0; cy<4; cy++) {
      for (int cx = 0; cx<5; cx++) {
        if (chars[alphabet_index][cy][cx] == 1) {
          draw_rect.x = (x+cx); 
          draw_rect.y = (y+cy);
          SDL_RenderFillRect(renderer, &draw_rect); 
        }
      }
    }
    x += 6;
  }
}
