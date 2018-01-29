#include "debugger.h"

#include <iostream>

#include "types.h"

using std::cout;
using std::endl;

void print_list_of_shapes(const ListOfShapes* const list) {
 for (uint i = 0; i < list->size(); i++){
   cout << "Shape " << i << endl;
   print_list_of_points((*list)[i]);
 }

 cout << endl;
}

void print_list_of_points(const ListOfPoints* const shape_points) {
 for (uint i = 0; i < shape_points->size(); i++){
   cout << (*shape_points)[i];

   if (i < shape_points->size() - 1) {
     // add comma after each (x, y) unless it is the last
     cout << ", ";
   }
 }

 cout << endl;
}
