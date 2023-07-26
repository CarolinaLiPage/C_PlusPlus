#ifndef RANDOM_H
#define RANDOM_H

#include <vector>

using namespace std;

class Random {
private:
  // internal counter
  static int counter;

  // list of random numbers
  static const vector<int> NUMS;

public:
  // return a pseudorandom number
  static int random();
};

#endif