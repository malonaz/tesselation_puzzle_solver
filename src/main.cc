#include "imagereader/image_read.h"
#include "discretizer/shape_rotate.h"
#include "common/memory.h"
#include "common/types.h"
int main(int argc, char** argv){
  ListOfShapes* list = new ListOfShapes();
  ListOfShapes* rotatedList = new ListOfShapes();

  find_coordinates(argv[1],list);
  debug_coordinates(argv[1],list);
  
  rotate_shapes(list, rotatedList);
  cleanup_list(list);
  for (uint i = 0; i < rotatedList->size(); ++i) {
    ListOfPoints* shape = (*rotatedList)[i];
    print_list_of_points(shape);
  }
  
  cleanup_list(rotatedList);

  return 0;
}
