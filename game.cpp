#include "game.hpp"

#include <bits/stdc++.h>

#include "board.hpp"

void Game::start() {
  while (board->isContinue()) {
    char column, row;
    board->printBoard();
    std::cout << "Turn " << std::setw(2) << ++progress << " "
              << (turn ? " White " : " Black ") << " : ";

    UINT64 legalBoard = board->makeLegalBoard(turn);
    if (legalBoard) {
      std::cin >> column >> row;
      column -= 'A';
      row -= '1';
      UINT64 put = (UINT64)1 << (8 * (7 - row) + (7 - column));
      while (!(legalBoard & put)) {
        std::cout << "Don't put there!!" << std::endl << " : ";
        std::cin >> column >> row;
        column -= 'A';
        row -= '1';
        put = (UINT64)1 << (8 * (7 - row) + (7 - column));
      }

      board->reverse(put, turn);

    } else {
      std::cout << "Pass" << std::endl;
      board->pass();
    }
    turn = !turn;
  }

  int black = board->count(0);
  int white = board->count(1);

  std::cout << std::endl << "========== Result ==========" << std::endl;
  std::cout << "Black " << std::setw(2) << black << " : " << std::setw(2)
            << white << " White" << std::endl;
  std::cout << (black == white ? "Draw"
                               : (black > white ? "Black win!" : "White win!"))
            << std::endl;
}
