#ifndef APS_DISCRETIZER_EDGE_H_
#define APS_DISCRETIZER_EDGE_H_

/**
 * Edge. Represents a horizontal edge, start and end represent the edge's x-coordinates
 */
struct Edge {
  int start;
  int end;

  Edge(int start, int end): start(start), end(end) {}
};

#endif
