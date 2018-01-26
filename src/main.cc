#include "imagereader/image_read.h"
#include "common/types.h"
int main(int argc, char** argv){
  ListOfShapes* list = new ListOfShapes();

  find_coordinates(argv[1],list);
  debug_coordinates(argv[1],list);
  delete list;

  return 0;
}
