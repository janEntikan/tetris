#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <vector>
#include <map>
#include <filesystem>
#include "../sdl/timer.h"
#include "../sdl/window.h"
#include "../sdl/input.h"
#include "../sdl/texture.h"
#include "pieces.h"

class Game {
public:
  Game();
	Input input;
private:
  Window window;
	std::map<std::string, Texture> texture_dict;
  Timer cap_timer;
  int board[20][10];
  Pieces pieces;
  int level;
  double x_time;
  double x_hold;
  int y_time;
  int location[2];

  void new_piece();
  void apply_piece();
  void clear_board();
  void rotate_piece();
  void move_piece();
  void hittest();
  void linetest();

  void update();
  void draw();
  void wait();
  void end();
};

#endif
