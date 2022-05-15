#ifndef ML_H_
#include <bits/stdc++.h>

#include "board.hpp"
class MachineLearning {
 private:
  std::vector<std::vector<int>> weightLine;
  std::vector<std::vector<int>> weight;
  bool myColor;
  std::string path = "./data/weight.txt";

 public:
  MachineLearning(const int mode);
  MachineLearning(std::vector<std::vector<int>> _weightLine, bool mut = false);

  void setTurn(bool turn) { myColor = turn; }

  std::vector<std::vector<int>> getWeight() { return weightLine; }

  void printWeight();

  UINT64 makePut(Board board, int turn);

  void writeWeight(std::string fname);
};

#endif  // ML_H_
