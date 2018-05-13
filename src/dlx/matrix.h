#ifndef APS_DLX_MATRIX_H_
#define APS_DLX_MATRIX_H_

#include "common/types.h"
#include "problem.h"
#include <vector>

using std::vector;

class Matrix {
private:
  struct Node;
  vector<uint> _col_ids; // Each column will have a column id
  vector<uint> _sizes;
  vector<Node> _nodes; // Consists of: head node, column header nodes

  void add_row(uint y, const vector<uint>& xs);
  uint create_node(uint x, uint y);

  void detach_left_right(uint id);
  void detach_up_down(uint id);

  void reattach_left_right(uint id);
  void reattach_up_down(uint id);

public:
  Matrix(const Problem* problem);

  void cover_column(uint id);
  void uncover_column(uint id);

  uint width() const;
  uint root() const;

  uint X(uint id) const;
  uint Y(uint id) const;

  /* size */
  uint S(uint id) const;
  uint C(uint id) const;

  /* left */
  uint L(uint id) const;

  /* right */
  uint R(uint id) const;

  /* up */
  uint U(uint id) const;

  /* down */
  uint D(uint id) const;
};

struct Matrix::Node {
  uint node_id;
  uint x;
  uint y;
  uint left;
  uint right;
  uint up;
  uint down;

  Node(uint id, uint x, uint y):
    node_id(id), x(x), y(y),
    left(id), right(id), up(id), down(id) {}
};

#endif
