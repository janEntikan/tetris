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
  new_game();
  while (input.keys["quit"] == 0){
		update();
	}
	//TODO: destroy everything
}

void Game::new_game() {
  x_hold = 1;
  x_time = 0;
  y_time = 0;
  drop_time = 10;

  level = 0;
  score = 0;
  combo = 0;
  lines = 0;
  clear_board();
 
  upcoming = {0,1,2,3,4,5,6};
  std::random_shuffle (upcoming.begin(), upcoming.end()); 

  new_piece();
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
  location[1] = -3;
  int p = upcoming.front();
  upcoming.erase(upcoming.begin());
  pieces.copy(p);
  
  if (upcoming.size() == 1) {
    p = upcoming.front();
    upcoming = {0,1,2,3,4,5,6};
    std::random_shuffle(upcoming.begin(), upcoming.end());
  }
  x_time = 0;
}

void Game::apply_piece() {
  location[1] -= 1;
  score++;
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int val = pieces.current_piece[y][x];
      if (val){
        int bx = x+location[0];
        int by = y+location[1];
        if (by > 0) {
          board[by][bx] = 1;
        } 
      }
    }
  }
  falling = false;
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
  //FIXME: this is repeated
  if (out_of_bounds() or hittest()){
    if (input.keys["rotate_l"] == 2) {
      pieces.flip_rows();
      pieces.transpose();
      x_hold = 1;
    } else if (input.keys["rotate_r"] == 2) {
      pieces.transpose();
      pieces.flip_rows();
      x_hold = 1;
    }
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
  // TODO: press up for instant drop
  drop_time -= (level+4);
  if (drop_time < 0) {
    drop_time = 60;
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
  if (hittest() or (out_of_bounds() != 0)) {
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
}

// is piece outside of board
int Game::out_of_bounds() {
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int val = pieces.current_piece[y][x];
      if (val) {
        int bx = x+location[0];
        int by = y+location[1];
        if (bx < 0) {
          return 1;
        } else if (bx >= 10) {
          return -1;
        }
      }
    }
  }
  return 0;
}

// piece is touching board
bool Game::hittest() {
  for (int y=0;y<5;y++) {
    for (int x=0;x<5;x++) {
      int val = pieces.current_piece[y][x];
      if (val){
        int bx = x+location[0];
        int by = y+location[1];
        if (by > 0) {
          if ((by >= 20) or (board[by][bx] > 0)) {
            return true;
          }
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

void Game::animate_line(std::vector<int> lines) {
  for (int i=0;i<5;i++) {
    cap_timer.start();
    for (auto& line : lines) {
      board[line][i+5] = 0;
      board[line][5-i] = 0;
    }
    draw();
    wait(20);
  }
  for (auto&line : lines) {
    remove_line(line);
  }
}


void Game::linetest() {
  // CHECK IF ANY LINES ARE COMPLETED (PLAY ANIMATION?)
  std::vector<int> lines_checked = {};
  int accumulated_score = 0;
  for (int y = 0; y < 20; y++) {
    if (line_is_full(y)) {
      lines++;
      lines_checked.push_back(y);
      //remove_line(y);
    }
  }
  if (lines_checked.size() > 0) {
    animate_line(lines_checked); 
    accumulated_score += lines_checked.size() * 5 * 10;
    if (lines_checked.size() == 4) {
      combo += 1;
      accumulated_score *= combo;
    } else {
      combo = 0;
    }
    while (lines > level*10) {
      level ++;
    }  
  }
  score += accumulated_score;
}

void Game::move() {
  move_piece();
  linetest();
}

void Game::update() {
  cap_timer.start();
  input.read_input();
  rotate_piece();
  if (input.keys["up"] == 2) {
    falling = true;
    while (falling) {
      move();
    }
  } else {
    move();
  }
  draw();
  wait(60);
}

void Game::draw() {
  int frame_x = 80;
  int frame_y = 8;
  int block_size = 8; //in pixels
  // CLEAR
  SDL_SetRenderDrawColor(window.renderer, 0x11, 0x11, 0x11, 0xFF);
  SDL_RenderClear(window.renderer);
  // DRAW FRAME
  SDL_SetRenderDrawColor(window.renderer, 0x22, 0x22, 0x22, 0xFF);
  DRAW_RECT.w = 12*block_size;
  DRAW_RECT.h = 22*block_size;
  DRAW_RECT.x = frame_x-block_size;
  DRAW_RECT.y = frame_y-block_size;
  SDL_RenderFillRect(window.renderer, &DRAW_RECT);
  // DRAW FIELD
  SDL_SetRenderDrawColor(window.renderer, 0x11, 0x11, 0x11, 0xFF);
  DRAW_RECT.w = 10*block_size;
  DRAW_RECT.h = 20*block_size;
  DRAW_RECT.x = frame_x;
  DRAW_RECT.y = frame_y;
  SDL_RenderFillRect(window.renderer, &DRAW_RECT);
  DRAW_RECT.w = block_size;
  DRAW_RECT.h = block_size;
  //  DRAW BOARD
  SDL_SetRenderDrawColor(window.renderer, colors[level][0], colors[level][1], colors[level][2], 0xFF);
  for (int y=0;y<20;y++) {
    for (int x=0;x<10;x++) {
      if (board[y][x] > 0) {
        DRAW_RECT.x = frame_x+(x*block_size);
        DRAW_RECT.y = frame_y+(y*block_size);
        SDL_RenderFillRect(window.renderer, &DRAW_RECT);
      }
    }
  }
  // DRAW PIECE
  SDL_SetRenderDrawColor(window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int y=0;y<5;y++) {
    if (location[1]+y >= 0) {
      for (int x=0;x<5;x++) {
        if (pieces.current_piece[y][x] > 0) {
          DRAW_RECT.x = frame_x+((location[0] + x)*block_size);
          DRAW_RECT.y = frame_y+((location[1] + y)*block_size);
          SDL_RenderFillRect(window.renderer, &DRAW_RECT); 
        }
      }
    }
  }
  // DRAW TEXT
  int text_x = frame_x+(10*block_size)+block_size+2;
  text.print(window.renderer, "score-", text_x, frame_y);
  text.print(window.renderer, std::to_string(score), text_x+35, frame_y);
  text.print(window.renderer, "level-", text_x, frame_y+6);
  text.print(window.renderer, std::to_string(level), text_x+35, frame_y+6);
  text.print(window.renderer, "combo-", text_x, frame_y+12);
  text.print(window.renderer, std::to_string(combo), text_x+35, frame_y+12);
  text.print(window.renderer, "lines-", text_x, frame_y+18);
  text.print(window.renderer, std::to_string(lines), text_x+35, frame_y+18);
  window.flip();
}

void Game::wait(int framerate) {
  int frame_ticks = cap_timer.get_ticks();
  if (frame_ticks < 1000/framerate) {
    SDL_Delay(1000/framerate - frame_ticks);
  }
}
