#ifndef APS_COMMON_DEBUGGER_H_
#define APS_COMMON_DEBUGGER_H_

#include "types.h"

void print_list_of_shapes(const ListOfShapes* const list);

void print_area(const ListOfShapeMatrices* const matrices);

void print_list_of_points(const ListOfPoints* const shape_points);

void print_solution_board(int** const board, uint height, uint width);

#endif
