#include "pieces.h"
#include <cstdio>

int PIECE_SIZE = 5;

void Pieces::transpose() {
  for (int y = 0; y<PIECE_SIZE; y++) {
    for (int x = 0; x<PIECE_SIZE; x++) {
      transformed_piece[y][x] = current_piece[x][y];
    }
  }
  apply_transform();
}

void Pieces::flip_rows() {
  for (int y = 0; y<PIECE_SIZE; y++) {
    for (int x = 0; x<PIECE_SIZE; x++) {
      transformed_piece[y][x] = current_piece[y][PIECE_SIZE-x-1];
    }
  }
  apply_transform();
}

void Pieces::apply_transform() {
  for (int y = 0; y<PIECE_SIZE; y++) {
    for (int x = 0; x<PIECE_SIZE; x++) {
      current_piece[y][x] = transformed_piece[y][x];
    }
  }
}

void Pieces::copy(int piece) {
  for (int y = 0; y<PIECE_SIZE; y++) {
    for (int x = 0; x<PIECE_SIZE; x++) {
      current_piece[y][x] = pieces[piece][y][x];
    }
  }
}


