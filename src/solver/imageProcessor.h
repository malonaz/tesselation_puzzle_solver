#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <vector>


/**
 * Function which given the the coordinates of each of the shapes, rotates the shapes such that 
 * all lines of the polygon are vertical or horizontal.
 */
std::vector< std::vector<int> > rotate_shapes(std::vector< std::vector<int> > &shapes);


#endif
