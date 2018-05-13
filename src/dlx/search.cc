#include <iostream>
#include "search.h"
#include "matrix.h"
#include "problem.h"

#include <random>
using namespace std;

Search::Search(const Problem* problem): _matrix(problem) {
}

uint Search::search(vector<uint>& stack) {
  uint h = _matrix.root();

  if (_matrix.R(h) == h) {
    return 1; //return success code
  }

  vector<uint> cs; //column selection by min. size

  for (uint j = _matrix.R(h); j != h; j = _matrix.R(j)) {
    if (!cs.empty() && _matrix.S(j) < _matrix.S(cs[0])) {
      cs.clear();
    }

    if (cs.empty() || _matrix.S(j) == _matrix.S(cs[0])) {
      cs.push_back(j);
    }
  }

  if (_matrix.S(cs[0]) < 1) {
    return 0; // backtracking needed
  }

  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_int_distribution<int> dist(0, cs.size() - 1);
  uint c = cs[dist(engine)];

  _matrix.cover_column(c);
  for (uint r = _matrix.D(c); r != c; r = _matrix.D(r)) {
    // searching down the column
    stack.push_back(_matrix.Y(r));

    for (uint j = _matrix.R(r); j != r; j = _matrix.R(j)) {
      _matrix.cover_column(j);
    }

    uint result = search(stack); //recursion
    if (result == 1) {
      return result; // do not backtrack if a result is found
    }

    for (uint j = _matrix.L(r); j != r; j = _matrix.L(j)) {
      _matrix.uncover_column(j);
    }

    stack.pop_back();
  }

  _matrix.uncover_column(c);
  return 0; // backtrack is needed
}
