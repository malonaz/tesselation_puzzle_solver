#ifndef APS_COMMON_SHAPE_MATRIX_IO_H_
#define APS_COMMON_SHAPE_MATRIX_IO_H_

#include "shape_matrix.h"
#include "types.h"

using namespace std;

/**
  Prints a shape matrix as a 2D array on the standard output.

  \param shape The pointer to the shape matrix to print.
*/
void shape_matrix_print(const ShapeMatrix* const shape);

/**
  Prints a shape matrix as a 2D array on the standard output.

  \param shape The shape matrix to print.
*/
void shape_matrix_print(const ShapeMatrix& shape);

/**
  Write a list of shape matrices to a file

  \param filename The path name to the file to save to.
  \param list The list of shapes.
*/
void shape_matrix_write(const char* filename, const vector<ShapeMatrix> &list);

/**
  Read a list of shape matrices from a file

  \param filename The path name to the file to read from.
  \param list The list to add the newly read in shapes to.
*/
void shape_matrix_read(const char* filename, vector<ShapeMatrix> &list, bool mute_debugging_messages);

#endif
