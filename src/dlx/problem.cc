#include "problem.h"
#include "common/types.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using std::sort;
using std::move;
using std::ostream;

Problem::Problem(uint width, uint num_secondary_columns):
  _width(width), _num_secondary_columns(num_secondary_columns), _rows() {}

uint Problem::width() const {
  return this->_width;
}

uint Problem::num_secondary_columns() const {
  return this->_num_secondary_columns;
}

vector<vector<uint>> Problem::rows() const {
  return this->_rows;
}

void Problem::add_row(vector<uint>& row) {
  sort(row.begin(), row.end());
  for (uint x: row) {
    if (x >= this->_width) {
      throw std::invalid_argument("Column supplied is out of range");
    }
  }

  for (uint i = 1; i < row.size(); ++i) {
    if (row[i - 1] == row[i]) {
      throw std::invalid_argument("Row contains duplicate columns");
    }
  }

  _rows.emplace_back(move(row));
}

ostream& operator<<(ostream& os, const Problem& problem) {
  os << problem.width() << ' ' << problem.num_secondary_columns() << '\n';
  for (const auto& row : problem.rows()) {
    for (auto i = 0u; i < row.size(); ++i) {
      os << row[i] << " \n"[i + 1 == row.size()];
    }
  }
  return os;
}
