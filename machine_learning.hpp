#ifndef ML_H_
#include <bits/stdc++.h>

#include "board.hpp"
class MachineLearning {
 private:
  std::vector<std::vector<int>> weightLine;
  std::vector<std::vector<int>> weight;
  bool myColor;
  std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> mp;
  bool isShot = false;

 public:
  MachineLearning();
  MachineLearning(const std::string path);
  MachineLearning(std::vector<std::vector<int>> _weightLine, bool mut = false);

  void lineToMap();

  void setTurn(bool turn) { myColor = turn; }

  std::vector<std::vector<int>> getWeight() { return weightLine; }

  void printWeight();

  uint64_t makePut(const Board &board, const int turn);

  int search(const Board &board, bool color, int depth, const int currentScore,
             const int turn);

  void writeWeight(std::string fname);

  bool shot(const Board board, bool turn, const clock_t start);
};

#endif  // ML_H_
