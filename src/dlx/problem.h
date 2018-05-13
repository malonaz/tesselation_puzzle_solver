#include "common/types.h"

#include <vector>

using std::vector;

class Problem {
private:
  vector<vector<uint>> _rows;
  uint _width;
  uint _num_secondary_columns;

public:
  Problem(uint width, uint num_secondary_columns);

  uint width() const;
  uint num_secondary_columns() const;
  vector<vector<uint>> rows() const;

  void add_row(vector<uint>& row);
};
