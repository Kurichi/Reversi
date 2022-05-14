#ifndef GAME_H_
#define GAME_H_

#include "board.hpp"

class Game {
 private:
  Board *board;
  bool turn = false;  // true:white, false:black
  short progress = 0;

 public:
  Game() { board = new Board(); }

  void start();
};

#endif  // GAME_H_
