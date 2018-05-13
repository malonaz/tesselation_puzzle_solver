#ifndef APS_DLX_SEARCH_H
#define APS_DLX_SEARCH_H

#include "matrix.h"
#include "problem.h"

class Search {
private:
  Matrix _matrix;

public:
  Search(const Problem* problem);
  uint search(vector<uint> stack);

};


#endif
