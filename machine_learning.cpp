#include "machine_learning.hpp"

#include <cmath>

MachineLearning::MachineLearning(const int mode) {
  weightLine = std::vector<std::vector<int>>(3, std::vector<int>(10));
  weight = std::vector<std::vector<int>>(3, std::vector<int>(64));

  switch (mode) {
      // random
    case 0:
      for (int i = 0; i < 30; i++)
        weightLine[i / 10][i % 10] = rand() % 100 - 50;
      break;

      // load
    case 1:
      std::ifstream ifs("./data/weight.txt");
      if (ifs.fail()) {
        std::cerr << "Can't load weight file" << std::endl;
        exit(1);
      }
      for (int i = 0; i < 30; i++) {
        int x;
        if (ifs >> x) weightLine[i / 10][i % 10] = x;
      }
      break;
  }

  // 1d weight  =>  2d weight
  for (int i = 0; i < 3; i++) {
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
};

MachineLearning::MachineLearning(std::vector<std::vector<int>> _weightLine,
                                 bool mut) {
  weight = std::vector<std::vector<int>>(3, std::vector<int>(64));
  weightLine = _weightLine;
  if (mut) {
    int num = abs((rand() % 20 + rand() % 20 + rand() % 20) / 3 - 10);
    for (int i = 0; i < num; i++)
      weightLine[rand() % 3][rand() % 10] = rand() % 100 - 50;
  }

  // 1d weight  =>  2d weight
  for (int i = 0; i < 3; i++) {
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
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 64; j++) {
      std::cout << std::setw(3) << weight[i][j];
      if (j % 8 == 7) std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

UINT64 MachineLearning::makePut(Board board, int turn) {
  UINT64 legalBoard = board.makeLegalBoard(myColor);
  UINT64 legal;
  UINT64 mask, mask2, mask3;

  std::pair<UINT64, int> MAX(0, -(int)INFINITY);
  for (int i = 63; i >= 0; --i) {
    mask = (UINT64)1 << i;
    if (legalBoard & mask) {
      Board board2 = board;

      board2.reverse(mask, myColor);
      UINT64 legal = board2.makeLegalBoard(!myColor);
      int MIN = (int)INFINITY;
      if (!legal) return mask;
      for (int j = 63; j >= 0; --j) {
        mask2 = (UINT64)1 << j;
        if (legal & mask2) {
          Board board3 = board2;
          board2.reverse(mask2, myColor);

          int count = 0;
          for (int k = 63; k >= 0; --k) {
            mask3 = (UINT64)1 << k;
            if (board3.getBoard(!myColor) & mask3)
              count -= weight[turn / 21][63 - k];
            else if (board3.getBoard(myColor) & mask3)
              count += weight[turn / 21][63 - k];
          }
          MIN = std::min(MIN, count);
        }
      }
      if (MAX.second < MIN) {
        MAX.first = mask;
        MAX.second = MIN;
      }
    }
  }

  return MAX.first;
}

void MachineLearning::writeWeight(std::string fname) {
  std::ofstream ofs(fname);
  for (int i = 0; i < 30; i++) {
    ofs << weightLine[i / 10][i % 10] << (i % 10 == 9 ? "\n" : " ");
  }
}
