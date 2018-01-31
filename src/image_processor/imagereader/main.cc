#include <iostream>
#include "image_read.h"

using std::cout;
using std::endl;

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    cout << "Expecting file name as first argument." << endl;;
    return 1;
  }
  find_coordinates(argv[1]);
  return 0;
}
