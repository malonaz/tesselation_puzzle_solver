#ifndef APS_COMMON_DEBUGGER_H_
#define APS_COMMON_DEBUGGER_H_

#include <vector>

#include "types.h"

using std::vector;

void print_list_of_shapes(const vector<ListOfPoints> &list);

void print_area(const vector<ShapeMatrix> &matrices);

void print_list_of_points(const ListOfPoints &shape_points);

void print_solution_board(int** const board, uint height, uint width);

#endif
