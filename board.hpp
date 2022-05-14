#ifndef BOARD_H_
#define BOARD_H_

#include <bits/stdc++.h>
using UINT64 = long long;

class Board {
 private:
  UINT64 white, black;
  int passCount;

 public:
  Board() {
    passCount = 0;
    white = 0x0000001008000000;
    black = 0x0000000810000000;
  }

  bool canPut(const bool turn, const UINT64 put);

  UINT64 makeLegalBoard(const bool turn);

  void reverse(const UINT64 put, const bool turn);

  bool isContinue();

  const void pass() { ++passCount; }

  void printBoard();

  int count(bool color);

 private:
  UINT64 transfer(const UINT64 put, const int k);
};

#endif  // BOARD_H_
