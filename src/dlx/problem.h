#ifndef APS_DLX_PROBLEM_H_
#define APS_DLX_PROBLEM_H_

#include "common/types.h"

#include <vector>

using std::vector;

class Problem {
private:
  uint _width;
  uint _num_secondary_columns;
  vector<vector<uint>> _rows;

public:
  Problem(uint width, uint num_secondary_columns = 0);

  uint width() const;
  uint num_secondary_columns() const;
  vector<vector<uint>> rows() const;

  void add_row(vector<uint>& row);
};

ostream& operator<<(ostream& os, const Problem& problem);

#endif
