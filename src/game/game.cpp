#include "game.h"
#include <algorithm>
#include <stdlib.h>

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
  new_piece();
  level = 1;
  x_time = 0;
  y_time = 0;
  drop_time = 10;
  x_hold = 1;
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
  location[1] = 0;
  pieces.copy(rand()%7); //make random/tetris alg
  x_time = 0;
}

void Game::apply_piece() {
  location[1] -= 1;
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int bx = x+location[0];
      int by = y+location[1];
      int val = pieces.current_piece[y][x];
      if (val){
        board[by][bx] = 1; 
      }
    }
  }
}

void Game::rotate_piece() {
  if (input.keys["rotate_r"] == 2) {
    pieces.flip_rows();
    pieces.transpose();
    x_hold = 1;
  } else if (input.keys["rotate_l"] == 2) {
    pieces.transpose();
    pieces.flip_rows();
    x_hold = 1;
  }
}

void Game::keyhold() {
  x_hold += 0.25;
  x_time = 8;
}

void Game::move_down() {
  y_time -= 1;
  if (y_time < 0) {
    if (input.keys["down"]) {
      y_time = 2;
      location[1] += 1;
      return;
    }
  }
  drop_time -= level;
  if (drop_time < 0) {
    drop_time = 30;
    location[1] += 1;
  }
}

void Game::move_side() {
  x_time -= x_hold;
  prev_x = location[0];
  if (x_time < 0) {
    if (input.keys["left"]) {
      location[0] -= 1;
      keyhold();
    } else if (input.keys["right"]) {
      location[0] += 1;
      keyhold();
   } else {
      x_time = 0;
      x_hold = 1;
    }
  }
  if (hittest() or out_of_bounds()) {
    location[0] = prev_x;
  }

}

void Game::move_piece() {
  move_side();
  move_down();  
  if (hittest()) {
    apply_piece();
    new_piece();  
  }
  // HITTEST PIECE WITH BOARD CONTENT
  // IF HITTEST WITH Y AT 0, GAME OVER
  // ELSE NEW PIECE IF HIT 
}

// See if piece is outside of board
bool Game::out_of_bounds() {
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int bx = x+location[0];
      int by = y+location[1];
      int val = pieces.current_piece[y][x];
      if (val) {
        if ((bx < 0) or (bx >= 10)) {
          return true;
        }
      }
    }
  }
  return false;
}

// See if piece is touching board
bool Game::hittest() {
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int bx = x+location[0];
      int by = y+location[1];
      int val = pieces.current_piece[y][x];
      if (val){
        if ((by >= 20) or (board[by][bx] > 0)) {
          return true;
        }
      }
    }
  }
  return false;
} 

bool Game::line_is_full(int y) {
  for (int x = 0; x < 10; x++) {
    int tile = board[y][x];
    if (tile == 0) {
      return false;
    }
  }
  return true;
}


void Game::remove_line(int y) {
  for (int i = y; i > 0; i--) {
    for (int x = 0; x < 10; x++) {
      if (i==0) {
        board[i][x] = 0;
      } else {
        board[i][x] = board[i-1][x];
      }
    } 
  }
}


void Game::linetest() {
  // CHECK IF ANY LINES ARE COMPLETED (PLAY ANIMATION?)
  for (int y = 0; y < 20; y++) {
    if (line_is_full(y)) {
      remove_line(y);
    }
  }
}

void Game::update() {
  cap_timer.start();
  input.read_input();
  rotate_piece();
  move_piece();
  linetest();
  draw();
  wait();
}

void Game::draw() {



  // DRAW STUFF

  SDL_SetRenderDrawColor(window.renderer, 0x11, 0x11, 0x11, 0xFF);
  SDL_RenderClear(window.renderer);

  int frame_x = 42;
  int frame_y = 8;
  int piece_size = 8; //in pixels
  // DRAW FRAME
  SDL_SetRenderDrawColor(window.renderer, 0x22, 0x22, 0x22, 0xFF);
  DRAW_RECT.w = 12*piece_size;
  DRAW_RECT.h = 22*piece_size;
  DRAW_RECT.x = frame_x-piece_size;
  DRAW_RECT.y = frame_y-piece_size;
  SDL_RenderFillRect(window.renderer, &DRAW_RECT);
  // DRAW FIELD
  SDL_SetRenderDrawColor(window.renderer, 0x11, 0x11, 0x11, 0xFF);
  DRAW_RECT.w = 10*piece_size;
  DRAW_RECT.h = 20*piece_size;
  DRAW_RECT.x = frame_x;
  DRAW_RECT.y = frame_y;
  SDL_RenderFillRect(window.renderer, &DRAW_RECT);

  DRAW_RECT.w = piece_size;
  DRAW_RECT.h = piece_size;
  //  DRAW BOARD
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0x00, 0xFF, 0xFF);
  for (int y=0;y<20;y++) {
    for (int x=0;x<10;x++) {
      if (board[y][x] > 0) {
        DRAW_RECT.x = frame_x+(x*piece_size);
        DRAW_RECT.y = frame_y+(y*piece_size);
        SDL_RenderFillRect(window.renderer, &DRAW_RECT);
      }
    }
  }

  //  DRAW PIECE
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      if (pieces.current_piece[y][x] > 0) {
        DRAW_RECT.x = frame_x+((location[0] + x)*piece_size);
        DRAW_RECT.y = frame_y+((location[1] + y)*piece_size);
        SDL_RenderFillRect(window.renderer, &DRAW_RECT); 
      }
    }
  }
  window.flip();
}

void Game::wait() {
  int frame_ticks = cap_timer.get_ticks();
  if (frame_ticks < 1000/60) {
    SDL_Delay(1000/60 - frame_ticks);
  }
}
