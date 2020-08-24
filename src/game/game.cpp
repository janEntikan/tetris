#include "game.h"
#include <algorithm>

SDL_Rect DRAW_RECT = { 0, 0, 8, 8 };

Game::Game() {
	char* title =  (char*)"inca 2000";
  window = Window{ title };
  window.initialize();
  SDL_SetRenderDrawColor(window.renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(window.renderer);
  input = Input();
  pieces = Pieces();
  clear_board();
  while (input.keys["quit"] == 0){
		update();
	}
	//TODO: destroy everything
}

void Game::clear_board() {
  for (int y=0;y<20;y++) {
    for (int x=0;x<10;x++) {
      board[y][x] = 0;
    }
  }
}

void Game::new_piece() {
  location[0] = 3;
  location[1] = 4;

}

void Game::update() {
  cap_timer.start();
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderClear(window.renderer);
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  input.read_input();
  // DRAW BOARD
  for (int y=0;y<20;y++) {
    for (int x=0;x<10;x++) {
      if (board[y][x] > 0) {
        DRAW_RECT.x = x*8;
        DRAW_RECT.y = y*8;
        SDL_RenderFillRect(window.renderer, &DRAW_RECT);
      }
    }
  }
  location[1] += 1;
  // DRAW CURRENT PIECE
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      if (pieces.pieces[1][y][x] > 0) {
        DRAW_RECT.x = (location[0] + x)*8;
        DRAW_RECT.y = (location[1] + y)*8;
        SDL_RenderFillRect(window.renderer, &DRAW_RECT); 
      }
    }
  }

  window.flip();

  int frame_ticks = cap_timer.get_ticks();
  if (frame_ticks < 1000/60) {
    SDL_Delay(1000/60 - frame_ticks);
  }
}
