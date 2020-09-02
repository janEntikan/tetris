#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <vector>
#include <map>
#include <filesystem>
#include "../sdl/timer.h"
#include "../sdl/window.h"
#include "../sdl/input.h"
#include "pieces.h"
#include "text.h"

class Game {
public:
  Game();
	Input input;
private:
  Window window;
  Timer cap_timer;
  int board[20][10];
  Text text;
  Pieces pieces;
  int level;
  int score;
  int combo;
  int lines;

  double x_time;
  double x_hold;
  int y_time;
  int drop_time; 
  int location[2];
  int prev_x;

  void keyhold();
  void new_game();
  void new_piece();
  void apply_piece();
  void clear_board();
  void rotate_piece();
  void move_piece();
  void move_side();
  void move_down();
  bool hittest();
  int out_of_bounds();
  bool line_is_full(int);
  void linetest();
  void remove_line(int);

  void update();
  void draw();
  void wait();
  void end();
};

#endif
