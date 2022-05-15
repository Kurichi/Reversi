#include <bits/stdc++.h>

#include <cstdlib>

#include "board.hpp"
#include "machine_learning.hpp"

bool run(MachineLearning& ml1, MachineLearning& ml2) {
  bool turn = false;
  int prog = 0;
  Board board = Board();

  while (board.isContinue()) {
    prog++;

    UINT64 legalBoard = board.makeLegalBoard(turn);
    if (legalBoard) {
      UINT64 put;
      if (!turn) {
        put = ml1.makePut(board, prog);
      } else {
        put = ml2.makePut(board, prog);
      }

      board.reverse(put, turn);

    } else {
      board.pass();
    }
    turn = !turn;
  }

  int black = board.count(0);
  int white = board.count(1);

  return black > white;
}

bool runRand(MachineLearning& ml, bool t) {
  bool turn = false;
  int prog = 0;
  Board board = Board();

  while (board.isContinue()) {
    prog++;

    UINT64 legalBoard = board.makeLegalBoard(turn);
    if (legalBoard) {
      UINT64 put;
      if (t) {
        std::vector<UINT64> l;
        for (int i = 0; i < 64; i++) {
          UINT64 mask = (UINT64)1 << i;
          if (legalBoard & mask) l.push_back(mask);
        }
        put = l[rand() % l.size()];
      } else {
        put = ml.makePut(board, prog);
      }
      board.reverse(put, turn);
    } else {
      board.pass();
    }
    turn = !turn;
    t = !t;
  }

  int black = board.count(0);
  int white = board.count(1);

  return black > white;
}

int main() {
  srand((unsigned)time(NULL));
  std::vector<std::pair<MachineLearning, int>> v(
      100, std::pair<MachineLearning, int>(NULL, 0));
  for (auto& a : v) a.first = MachineLearning(0);

  for (auto& a : v) {
    for (int j = 0; j < 100; j++)
      if (runRand(a.first, j % 2)) a.second++;
  }

  sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) {
    return lhs.second > rhs.second;
  });
  v.erase(v.begin() + 20, v.end());

  for (auto& a : v) std::cout << a.second << std::endl;

  v[0].first.printWeight();

  int cnt = 0;
  while (true) {
    std::cout << ++cnt << "回目" << std::endl;
    for (auto& a : v) a.second = 0;
    for (int i = 0; i < 19; i++) {
      for (int j = i + 1; j < 20; j++) {
        for (int k = 0; k < 2; k++) {
          if (k % 2 == 0) {
            if (run(v[i].first, v[j].first))
              v[i].second += 100;
            else
              v[j].second += 100;
          } else {
            if (run(v[j].first, v[i].first))
              v[j].second += 100;
            else
              v[i].second += 100;
          }
        }
      }
      for (int j = 0; j < 1000; j++)
        if (runRand(v[i].first, j % 2)) v[i].second++;
    }
    sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) {
      return lhs.second > rhs.second;
    });
    for (auto& a : v) std::cout << a.second << std::endl;
    v[0].first.printWeight();
    v[0].second = 0;

    for (int j = 0; j < 1000; j++)
      if (runRand(v[0].first, j % 2)) v[0].second++;
    std::cout << "勝率 : " << (double)v[0].second / 10 << "%" << std::endl;

    if (v[0].second > 950) {
      break;
    }

    std::vector<std::vector<int>> ml1 = v[0].first.getWeight();
    std::vector<std::vector<int>> ml2 = v[1].first.getWeight();

    std::vector<std::vector<int>> tmp(3, std::vector<int>(10, 0));
    for (int i = 3; i < 10; i++) {
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

    for (int i = 10; i < 15; i++) v[i].first = MachineLearning(ml1, true);
    for (int i = 15; i < 20; i++) v[i].first = MachineLearning(ml2, true);
  }

  v[0].first.writeWeight("./data/weight1.txt");
  v[1].first.writeWeight("./data/weight2.txt");
}

