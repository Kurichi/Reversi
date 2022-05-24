#include <bits/stdc++.h>

#include <cstdlib>
#include <ctime>

#include "board.hpp"
#include "machine_learning.hpp"

bool runRand(MachineLearning& ml, bool t) {
  bool turn = false;
  int prog = 0;
  Board board = Board();
  ml.setTurn(t);

  while (board.isContinue()) {
    board.printBoard();

    uint64_t legalBoard = board.makeLegalBoard(turn);
    if (legalBoard) {
      uint64_t put;
      do {
        // random turn
        if (t) {
          std::cout << ">>";
          char column, row;
          std::cin >> column >> row;
          column -= 'A';
          row -= '1';
          put = (uint64_t)1 << (8 * (7 - row) + (7 - column));
          while (!(legalBoard & put)) {
            std::cout << "Don't put there!!" << std::endl << " : ";
            std::cin >> column >> row;
            column -= 'A';
            row -= '1';
            put = (uint64_t)1 << (8 * (7 - row) + (7 - column));
          }

          /* std::vector<uint64_t> l; */
          /* for (int i = 0; i < 64; i++) { */
          /*   uint64_t mask = (uint64_t)1 << i; */
          /*   if (legalBoard & mask) l.push_back(mask); */
          /* } */
          /* put = l[rand() % l.size()]; */
        }
        // AI turn
        else {
          put = ml.makePut(board, prog);
          /* std::cout << put << std::endl; */
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

  board.printBoard();

  int black = board.count(0);
  int white = board.count(1);
  std::cout << "Black " << std::setw(2) << black << " : " << std::setw(2)
            << white << std::endl;

  return black < white;
}

int main() {
  const int NUM_CLI = 100;
  srand((unsigned)time(NULL));

  MachineLearning ml = MachineLearning(1);
  int x;
  std::cin >> x;
  runRand(ml, x);
  return 0;

  int count = 0;
  for (int i = 0; i < 100; i++)
    if (runRand(ml, i % 2) == i % 2) count++;
  std::cout << count << std::endl;
  return 0;

  // random weight generate
  std::vector<std::pair<MachineLearning, int>> v(
      NUM_CLI, std::pair<MachineLearning, int>(NULL, 0));
  for (auto& a : v) a.first = MachineLearning(0);

  // learn start
  int cnt = 0;
  while (true) {
    clock_t start = clock();
    std::cout << ++cnt << "回目" << std::endl;

    // win count reset
    for (auto& a : v) a.second = 0;

    // 1000 battle
    int progress = 0;
    for (auto& a : v) {
      {
        for (int j = 0; j < 1000; j++) {
          if (runRand(a.first, j % 2) == (j % 2)) a.second++;
        }
      }
      std::cout << "Cli : " << ++progress << " win:" << a.second << std::endl;
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
      if (runRand(v[0].first, j % 2) == (j % 2)) v[0].second++;
    }
    std::cout << "勝率 : " << (double)v[0].second / 10 << "%" << std::endl;

    if (v[0].second > 980) {
      break;
    }

    std::vector<std::vector<int>> ml1 = v[0].first.getWeight();
    std::vector<std::vector<int>> ml2 = v[1].first.getWeight();

    // make child
    std::vector<std::vector<int>> tmp(10, std::vector<int>(10, 0));
    for (int i = 2; i < NUM_CLI * 0.8; i++) {
      for (int j = 0; j < 10; j++) {
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

    for (int i = NUM_CLI * 0.8; i < NUM_CLI * 0.9; i++)
      v[i].first = MachineLearning(ml1, true);
    for (int i = NUM_CLI * 0.9; i < NUM_CLI; i++)
      v[i].first = MachineLearning(ml2, true);

    clock_t end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
  }

  v[0].first.writeWeight("./data/weight1.txt");
  v[1].first.writeWeight("./data/weight2.txt");
}
