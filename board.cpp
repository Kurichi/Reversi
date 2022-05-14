#include "board.hpp"

bool Board::canPut(const bool turn, const UINT64 put) {
  const UINT64 legalBoard = makeLegalBoard(turn);
  return (put & legalBoard);
}

UINT64 Board::transfer(const UINT64 put, const int k) {
  switch (k) {
    case 0:  //上
      return (put << 8) & 0xffffffffffffff00;
    case 1:  //右上
      return (put << 7) & 0x7f7f7f7f7f7f7f00;
    case 2:  //右
      return (put >> 1) & 0x7f7f7f7f7f7f7f7f;
    case 3:  //右下
      return (put >> 9) & 0x007f7f7f7f7f7f7f;
    case 4:  //下
      return (put >> 8) & 0x00ffffffffffffff;
    case 5:  //左下
      return (put >> 7) & 0x00fefefefefefefe;
    case 6:  //左
      return (put << 1) & 0xfefefefefefefefe;
    case 7:  //左上
      return (put << 9) & 0xfefefefefefefe00;
    default:
      return 0;
  }
}

void Board::reverse(const UINT64 put, const bool turn) {
  UINT64 *opponentBoard = (turn ? &black : &white);
  UINT64 *playerBoard = (turn ? &white : &black);

  UINT64 result = 0;
  for (int i = 0; i < 8; i++) {
    UINT64 tmp = 0;
    UINT64 mask = transfer(put, i);
    while (mask != 0 && (mask & *opponentBoard) != 0) {
      tmp |= mask;
      mask = transfer(mask, i);
    }
    if (mask & *playerBoard) result |= tmp;
  }

  *playerBoard ^= (put | result);
  *opponentBoard ^= result;
}

UINT64 Board::makeLegalBoard(const bool turn) {
  UINT64 opponentBoard = (turn ? black : white);
  UINT64 playerBoard = (turn ? white : black);
  UINT64 blankBoard = ~(opponentBoard | playerBoard);

  UINT64 horizontalWatchBoard = opponentBoard & 0x7e7e7e7e7e7e7e7e;
  UINT64 verticalWatchBoard = opponentBoard & 0x00ffffffffffff00;
  UINT64 allWatchBoard = opponentBoard & 0x007e7e7e7e7e7e00;

  UINT64 tmp, legalBoard;

  // left
  tmp = horizontalWatchBoard & (playerBoard << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  tmp |= horizontalWatchBoard & (tmp << 1);
  legalBoard = blankBoard & (tmp << 1);

  // right
  tmp = horizontalWatchBoard & (playerBoard >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  tmp |= horizontalWatchBoard & (tmp >> 1);
  legalBoard |= blankBoard & (tmp >> 1);

  // up
  tmp = verticalWatchBoard & (playerBoard << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  tmp |= verticalWatchBoard & (tmp << 8);
  legalBoard |= blankBoard & (tmp << 8);

  // down
  tmp = verticalWatchBoard & (playerBoard >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  tmp |= verticalWatchBoard & (tmp >> 8);
  legalBoard |= blankBoard & (tmp >> 8);

  // up-left
  tmp = allWatchBoard & (playerBoard << 9);
  tmp |= allWatchBoard & (tmp << 9);
  tmp |= allWatchBoard & (tmp << 9);
  tmp |= allWatchBoard & (tmp << 9);
  tmp |= allWatchBoard & (tmp << 9);
  tmp |= allWatchBoard & (tmp << 9);
  legalBoard |= blankBoard & (tmp << 9);

  // up-right
  tmp = allWatchBoard & (playerBoard << 7);
  tmp |= allWatchBoard & (tmp << 7);
  tmp |= allWatchBoard & (tmp << 7);
  tmp |= allWatchBoard & (tmp << 7);
  tmp |= allWatchBoard & (tmp << 7);
  tmp |= allWatchBoard & (tmp << 7);
  legalBoard |= blankBoard & (tmp << 7);

  // down-left
  tmp = allWatchBoard & (playerBoard >> 7);
  tmp |= allWatchBoard & (tmp >> 7);
  tmp |= allWatchBoard & (tmp >> 7);
  tmp |= allWatchBoard & (tmp >> 7);
  tmp |= allWatchBoard & (tmp >> 7);
  tmp |= allWatchBoard & (tmp >> 7);
  legalBoard |= blankBoard & (tmp >> 7);

  tmp = allWatchBoard & (playerBoard >> 9);
  tmp |= allWatchBoard & (tmp >> 9);
  tmp |= allWatchBoard & (tmp >> 9);
  tmp |= allWatchBoard & (tmp >> 9);
  tmp |= allWatchBoard & (tmp >> 9);
  tmp |= allWatchBoard & (tmp >> 9);
  legalBoard |= blankBoard & (tmp >> 9);

  return legalBoard;
}

bool Board::isContinue() {
  if (passCount == 2) return false;

  if ((white | black) == 0xffffffffffffffff) return false;

  return true;
}

void Board::printBoard() {
  putchar(' ');
  for (int i = 0; i < 8; i++) putchar('A' + i);

  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0) std::cout << std::endl << (i / 8) + 1;

    UINT64 mask = ((UINT64)1 << (63 - i));
    if (white & mask)
      std::cout << "●";
    else if (black & mask)
      std::cout << "○";
    else
      std::cout << "-";
  }
  putchar('\n');
}

int Board::count(bool color) {
  UINT64 board = (color ? white : black);
  int count = 0;
  for (int i = 0; i < 64; i++) {
    UINT64 mask = (UINT64)1 << i;
    if (board & mask) count++;
  }
  return count;
}
