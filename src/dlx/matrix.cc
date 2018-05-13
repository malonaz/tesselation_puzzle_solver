#include "matrix.h"
#include "problem.h"
#include <cassert>
#include <vector>
#include <iostream>

using std::vector;
using namespace std;

Matrix::Matrix(const Problem* problem):
    _col_ids(problem->width()), _sizes(problem->width()), _nodes() {
  uint root = this->create_node(0, 0); //creating head node 'h'
  for (uint x = 0; x < _col_ids.size(); ++x) {
    uint node_id = this->create_node(x, 0); // creating column header nodes
    _col_ids[x] = node_id;
    if (x >= problem->num_secondary_columns()) { // secondary columns currently not in use
      _nodes[node_id].right = root;
      _nodes[node_id].left = L(root);
      _nodes[L(root)].right = node_id;
      _nodes[root].left = node_id;
    }
  }
  vector<vector<uint>> rows = problem->rows();//
  for (uint y = 0; y < rows.size(); ++y) {
    this->add_row(y, rows[y]);
  }
}

uint Matrix::width() const {
  return _col_ids.size();
}

uint Matrix::root() const {
  return 0;
}

uint Matrix::X(uint id) const {
  return _nodes[id].x;
}

uint Matrix::Y(uint id) const {
  return _nodes[id].y;
}

uint Matrix::S(uint id) const {
  return _sizes[this->X(id)];
}

uint Matrix::C(uint id) const {
  return _col_ids[this->X(id)];
}

uint Matrix::L(uint id) const {
  return _nodes[id].left;
}

uint Matrix::R(uint id) const {
  return _nodes[id].right;
}

uint Matrix::U(uint id) const {
  return _nodes[id].up;
}

uint Matrix::D(uint id) const {
  return _nodes[id].down;
}

void Matrix::detach_left_right(uint id) {
  _nodes[L(id)].right = R(id);
  _nodes[R(id)].left = L(id);
}

void Matrix::detach_up_down(uint id) {
  _nodes[U(id)].down = D(id);
  _nodes[D(id)].up = U(id);
}

void Matrix::reattach_left_right(uint id) {
  _nodes[L(id)].right = id;
  _nodes[R(id)].left = id;
}

void Matrix::reattach_up_down(uint id) {
  _nodes[U(id)].down = id;
  _nodes[D(id)].up = id;
}

void Matrix::add_row(uint y, const vector<uint>& xs) {
  uint first_id = 0;
  for (uint x: xs) {
    uint id = this->create_node(x, y);
    _nodes[id].down = C(id);
    _nodes[id].up = U(C(id));
    _nodes[U(C(id))].down = id;
    _nodes[C(id)].up = id;
    ++_sizes[x];
    if (first_id == 0) {
      first_id = id;
    } else {
      _nodes[id].right = first_id;
      _nodes[id].left = L(first_id);
      _nodes[L(first_id)].right = id;
      _nodes[first_id].left = id;
    }
  }
}

uint Matrix::create_node(uint x, uint y) {
  assert(x <= width());
  uint new_id = _nodes.size();
  _nodes.emplace_back(new_id, x, y); // "new" constructor + push_back
  return new_id;
}

void Matrix::cover_column(uint cid) {
  uint column = C(cid);
  detach_left_right(column);
  for (uint i = D(column); i != column; i = D(i)) {
    for (uint j = R(i); j != i; j = R(j)) {
      detach_up_down(j);
      --_sizes[X(j)];
    }
  }
}

void Matrix::uncover_column(uint cid) {
  uint column = C(cid);
  for (uint i = U(column); i != column; i = U(i)) {
    for (uint j = L(i); j != i; j = L(j)) {
      reattach_up_down(j);
      ++_sizes[X(j)];
    }
  }
  reattach_left_right(column);
}
