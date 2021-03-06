#include "coordinates_io.h"

#include <cassert>
#include <cstddef>
#include <fstream>

#include "types.h"

using std::ifstream;
using std::ws;

bool read_coordinates_file(const char* file, vector<ListOfPoints> &list) {
  ifstream input_file;
  input_file.open(file);
  if (input_file.fail()) {
    return false;
  }

  while (!input_file.eof()) {
    int num_points;
    input_file >> num_points;
    ListOfPoints points;
    for (int i = 0; i < num_points; ++i) {
      int x, y;
      input_file >> x;
      input_file >> y;
      points.push_back(Point(x, y));
    }
    list.push_back(points);
    input_file >> ws;
  }

  input_file.close();

  return true;
}
