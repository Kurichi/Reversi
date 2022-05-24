#ifndef BOARD_H_
#define BOARD_H_

#include <bits/stdc++.h>

class Board {
 private:
  uint64_t white, black;
  int passCount;

 public:
  Board() {
    passCount = 0;
    white = 0x0000001008000000;
    black = 0x0000000810000000;
  }

  bool canPut(const bool turn, const uint64_t put);

  uint64_t makeLegalBoard(const bool turn) const;

  void reverse(const uint64_t put, const bool turn);

  bool isContinue() const;

  const void pass() { ++passCount; }

  void printBoard() const;

  int count(const bool color) const;

  uint64_t getBoard(const bool turn) const { return (turn ? white : black); }

 private:
  uint64_t transfer(const uint64_t put, const int k) const;
};

#endif  // BOARD_H_
