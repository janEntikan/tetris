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
  new_piece();
  level = 1;
  x_time = 10;
  y_time = 10;
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
  pieces.copy(1); //make random/tetris alg
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
  if (input.keys["rotate_l"] == 2) {
    pieces.flip_rows();
    pieces.transpose();
  } else if (input.keys["rotate_r"] == 2) {
    pieces.transpose();
    pieces.flip_rows();
  }
}

void Game::move_piece() {
  //X
  // increment x_hold each cycle move key is held
  x_time -= x_hold;
  if (x_time < 0) {
    x_time = 5; 
    if (input.keys["left"]) {
      location[0] -= 1;
      x_hold += 0.5;
    } else if (input.keys["right"]) {
      location[0] += 1;
      x_hold += 0.5;
    } else if (input.keys["down"]) {
      location[1] += 1;
      x_hold += 0.5;    
    }else {
      x_time = 5;
      x_hold = 1;
    }
  }
  //Y
  y_time -= level;
  if (y_time < 0) {
    y_time = 30;
    location[1] += 1;
  }
}

void Game::hittest() {
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int bx = x+location[0];
      int by = y+location[1];
      int val = pieces.current_piece[y][x];
      if (val){
        // HITTEST PIECE WITH BOARD WIDTH
        while (bx < 0) {
          bx = x+location[0];
          location[0] += 1;
        } 
        while (bx >= 10) {
          bx = x+location[0];
          location[0] -= 1;
        }


        if ((by >= 20) or (board[by][bx] > 0)) {
          apply_piece();
          new_piece();
          return;
        }
      }
    }
  }  
  // HITTEST PIECE WITH BOARD CONTENT
  // IF HITTEST WITH BOARD AT CEILING, GAME OVER
  // NEW PIECE IF HIT 
}

void Game::linetest() {
  // CHECK IF ANY LINES ARE COMPLETED (COULD PLAY ANIMATION?)
}

void Game::update() {
  cap_timer.start();
  input.read_input();
  rotate_piece();
  move_piece();
  hittest();
  linetest();
  draw();
  wait();
}

void Game::draw() {  
  // DRAW STUFF
  SDL_SetRenderDrawColor(window.renderer, 0x11, 0x11, 0x11, 0xFF);
  SDL_RenderClear(window.renderer);
  
  //  DRAW BOARD
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0x00, 0xFF, 0xFF);
  for (int y=0;y<20;y++) {
    for (int x=0;x<10;x++) {
      if (board[y][x] > 0) {
        DRAW_RECT.x = x*8;
        DRAW_RECT.y = y*8;
        SDL_RenderFillRect(window.renderer, &DRAW_RECT);
      }
    }
  }

  //  DRAW PIECE
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      if (pieces.current_piece[y][x] > 0) {
        DRAW_RECT.x = (location[0] + x)*8;
        DRAW_RECT.y = (location[1] + y)*8;
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
