#ifndef APS_DISCRETIZER_EDGE_H_
#define APS_DISCRETIZER_EDGE_H_

struct Edge {
  int start;
  int end;

  Edge(int start, int end): start(start), end(end) {}
};

#endif