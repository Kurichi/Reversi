#include "machine_learning.hpp"

#include <bits/stdc++.h>

#include <ctime>

MachineLearning::MachineLearning(const int mode) {
  weightLine = std::vector<std::vector<int>>(10, std::vector<int>(10));
  weight = std::vector<std::vector<int>>(10, std::vector<int>(64));

  switch (mode) {
      // random
    case 0:
      for (int i = 0; i < 100; i++)
        weightLine[i / 10][i % 10] = rand() % 100 - 50;
      break;

      // load
    case 1:
      std::ifstream ifs("./data/weight.txt");
      if (ifs.fail()) {
        std::cerr << "Can't load weight file" << std::endl;
        exit(1);
      }
      for (int i = 0; i < 100; i++) {
        int x;
        if (ifs >> x) weightLine[i / 10][i % 10] = x;
      }
      break;
  }

  lineToMap();
};

MachineLearning::MachineLearning(std::vector<std::vector<int>> _weightLine,
                                 bool mut) {
  weight = std::vector<std::vector<int>>(10, std::vector<int>(64));
  weightLine = _weightLine;
  if (mut) {
    int num = abs((rand() % 20 + rand() % 20 + rand() % 20) / 3 - 10);
    for (int i = 0; i < num; i++)
      weightLine[rand() % 10][rand() % 10] = rand() % 100 - 50;
  }

  lineToMap();
}

void MachineLearning::lineToMap() {
  // 1d weight  =>  2d weight
  for (int i = 0; i < 10; i++) {
    std::vector<int> &w = weight[i];
    w[0] = w[7] = w[56] = w[63] = weightLine[i][0];
    w[1] = w[6] = w[8] = w[15] = w[48] = w[55] = w[57] = w[62] =
        weightLine[i][1];
    w[2] = w[5] = w[16] = w[23] = w[40] = w[47] = w[58] = w[61] =
        weightLine[i][2];
    w[3] = w[4] = w[24] = w[31] = w[32] = w[39] = w[59] = w[60] =
        weightLine[i][3];
    w[9] = w[14] = w[49] = w[54] = weightLine[i][4];
    w[10] = w[13] = w[17] = w[22] = w[41] = w[46] = w[50] = w[53] =
        weightLine[i][5];
    w[11] = w[12] = w[25] = w[30] = w[33] = w[38] = w[51] = w[52] =
        weightLine[i][6];
    w[18] = w[21] = w[42] = w[45] = weightLine[i][7];
    w[19] = w[20] = w[26] = w[29] = w[34] = w[37] = w[43] = w[44] =
        weightLine[i][8];
    w[27] = w[28] = w[35] = w[36] = weightLine[i][9];
  }
}

void MachineLearning::printWeight() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 64; j++) {
      std::cout << std::setw(4) << weight[i][j];
      if (j % 8 == 7) std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

UINT64 MachineLearning::makePut(const Board &board, const int turn) {
  if (turn == 41)
    mp = std::unordered_map<UINT64, std::unordered_map<UINT64, UINT64>>();
  if (turn > 40) {
    if (isShot) {
      return mp[board.getBoard(myColor)][board.getBoard(!myColor)];
    } else if (shot(board, myColor, clock())) {
      isShot = true;
      /* std::cout << "読み切った" << std::endl; */
      return mp[board.getBoard(myColor)][board.getBoard(!myColor)];
    } else {
      /* std::cout << "読み切りタイムオーバー" << std::endl; */
    }
  } else
    isShot = false;

  const UINT64 legalBoard = board.makeLegalBoard(myColor);

  std::pair<UINT64, int> MAX(0, -(int)INFINITY);
  for (int i = 63; i >= 0; --i) {
    const UINT64 mask = (UINT64)1 << i;
    if (legalBoard & mask) {
      Board board2 = board;
      board2.reverse(mask, myColor);
      const int score = search(board2, !myColor, 1, MAX.second);

      if (MAX.second < score) {
        MAX.first = mask;
        MAX.second = score;
      }
    }
  }

  return MAX.first;
}

int MachineLearning::search(const Board &board, bool color, int depth,
                            const int currentScore) {
  // end search
  if (depth > 8) {
    const UINT64 myBoard = board.getBoard(myColor);
    const UINT64 oppoBoard = board.getBoard(!myColor);
    int score = 0;
    for (int i = 0; i < 64; i++) {
      const UINT64 mask = (UINT64)1 << i;
      if (mask & myBoard) {
        score += weight[i][i];
      } else if (mask & oppoBoard) {
        score -= weight[i][i];
      }
      return score;
    }
  }

  const UINT64 legalBoard = board.makeLegalBoard(color);

  // can't put anywhere
  if (!legalBoard) {
    return search(board, !color, depth + 1, currentScore);
  }

  int score = (int)INFINITY * (color ? -1 : 1);
  for (int i = 0; i < 64; i++) {
    const UINT64 mask = (UINT64)1 << i;
    if (legalBoard & mask) {
      Board putBoard = board;
      putBoard.reverse(mask, color);
      if (color) {
        score = std::max(score, search(putBoard, !color, depth + 1, score));
        if (currentScore < score) return score;
      } else {
        score = std::min(score, search(putBoard, !color, depth + 1, score));
        if (currentScore > score) return score;
      }
    }
  }

  return score;
}

void MachineLearning::writeWeight(std::string fname) {
  std::ofstream ofs(fname);
  for (int i = 0; i < 100; i++) {
    ofs << weightLine[i / 10][i % 10] << (i % 10 == 9 ? "\n" : " ");
  }
}

bool MachineLearning::shot(Board board, bool turn, clock_t start) {
  if ((double)(clock() - start) / CLOCKS_PER_SEC > 2) return false;
  if (mp.find(board.getBoard(myColor)) != mp.end() &&
      mp[board.getBoard(myColor)].find(board.getBoard(!myColor)) !=
          mp[board.getBoard(myColor)].end())
    return true;

  UINT64 legalBoard = board.makeLegalBoard(turn);
  if (!legalBoard) {
    turn = !turn;
    legalBoard = board.makeLegalBoard(turn);
  }

  bool canWin = (turn != myColor);
  if (legalBoard) {
    for (int i = 0; i < 64; i++) {
      UINT64 mask = (UINT64)1 << i;
      if (legalBoard & mask) {
        Board b = board;
        b.reverse(mask, turn);
        // my turn
        if (turn == myColor) {
          // can win
          if (shot(b, !turn, start)) {
            mp[board.getBoard(myColor)][board.getBoard(!myColor)] = mask;
            return true;
          }
        }
        // opponent turn
        else {
          canWin &= shot(b, !turn, start);
          if (!canWin) return false;
        }
      }
    }
    return canWin;
  } else {
    return (board.count(myColor) > board.count(!myColor));
  }
}
