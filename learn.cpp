#include <bits/stdc++.h>

#include <cstdlib>

#include "board.hpp"
#include "machine_learning.hpp"

bool runRand(MachineLearning& ml, bool t) {
  bool turn = false;
  int prog = 0;
  Board board = Board();

  while (board.isContinue()) {
    /* board.printBoard(); */

    UINT64 legalBoard = board.makeLegalBoard(turn);
    if (legalBoard) {
      UINT64 put;
      do {
        // random turn
        if (t) {
          std::vector<UINT64> l;
          for (int i = 0; i < 64; i++) {
            UINT64 mask = (UINT64)1 << i;
            if (legalBoard & mask) l.push_back(mask);
          }
          put = l[rand() % l.size()];
        }
        // AI turn
        else {
          put = ml.makePut(board, prog);
        }
      } while (!(legalBoard & put));

      prog++;
      board.reverse(put, turn);
    } else {
      board.pass();
    }
    turn = !turn;
    t = !t;
  }

  /* board.printBoard(); */

  int black = board.count(0);
  int white = board.count(1);
  /* std::cout << "Black " << std::setw(2) << black << " : " << std::setw(2) */
  /*           << white << std::endl; */

  return black < white;
}

int main() {
  srand((unsigned)time(NULL));

  // random weight generate
  std::vector<std::pair<MachineLearning, int>> v(
      1000, std::pair<MachineLearning, int>(NULL, 0));
  for (auto& a : v) a.first = MachineLearning(0);

  // 1000 battle per 1 cli
  int test = 0;
  for (auto& a : v) {
    for (int j = 0; j < 1000; j++) {
      a.first.setTurn(j % 2);
      if (runRand(a.first, j % 2) == (j % 2)) a.second++;
    }
    std::cout << ++test << std::endl;
  }

  // sort by win-count
  sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) {
    return lhs.second > rhs.second;
  });
  v.erase(v.begin() + 100, v.end());

  // 1000 => 100
  for (auto& a : v) std::cout << a.second << std::endl;

  v[0].first.printWeight();

  // learn start
  int cnt = 0;
  while (true) {
    std::cout << ++cnt << "回目" << std::endl;

    // win count reset
    for (auto& a : v) a.second = 0;

    // 1000 battle
    for (auto& a : v) {
      for (int j = 0; j < 100; j++) {
        a.first.setTurn(j % 2);
        if (runRand(a.first, j % 2) == (j % 2)) a.second++;
      }
    }

    // sort by win-count
    sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) {
      return lhs.second > rhs.second;
    });

    // print win-count
    for (auto& a : v) std::cout << a.second << std::endl;
    v[0].first.printWeight();
    v[0].second = 0;

    // challenge
    for (int j = 0; j < 1000; j++) {
      v[0].first.setTurn(j % 2);
      if (runRand(v[0].first, j % 2) == (j % 2)) v[0].second++;
    }
    std::cout << "勝率 : " << (double)v[0].second / 10 << "%" << std::endl;

    if (v[0].second > 950) {
      break;
    }

    std::vector<std::vector<int>> ml1 = v[0].first.getWeight();
    std::vector<std::vector<int>> ml2 = v[1].first.getWeight();

    // make child
    std::vector<std::vector<int>> tmp(3, std::vector<int>(10, 0));
    for (int i = 2; i < 80; i++) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 10; k++) {
          int s = rand() % 11;
          if (s < 5)
            tmp[j][k] = ml1[j][k];
          else if (s < 10)
            tmp[j][k] = ml2[j][k];
          else
            tmp[j][k] = rand() % 100 - 50;
        }
      }
      v[i].first = MachineLearning(tmp);
    }

    for (int i = 80; i < 90; i++) v[i].first = MachineLearning(ml1, true);
    for (int i = 90; i < 100; i++) v[i].first = MachineLearning(ml2, true);
  }

  v[0].first.writeWeight("./data/weight1.txt");
  v[1].first.writeWeight("./data/weight2.txt");
}

