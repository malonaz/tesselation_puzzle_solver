#include "shape_translate.h"
#include "common/point.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/shape_matrix_io.h"
#include "common/edge.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <vector>

using std::map;
using std::min;
using std::max;
using std::round;
using std::vector;
using std::sort;

// used as threshold to decide if two lengths are similar
#define THRESHOLD 0.40


/**
 * Computes the width and height of the shape inplied by the given shape's list of points.
 *   @param shape: The list of coordinates that define the shape.
 *   @param width: The width of the shape returned by reference.
 *   @param height: The height of the same returned by reference.
*/
void shape_get_size(const ListOfPoints &shape, uint& width, uint& height) {

  // used to store the max & min of all x and y coordinates
  int max_x = 0, max_y = 0, min_x = 0, min_y = 0;

  for (uint i = 0; i < shape.size(); i++){

    // get current point
    Point point = shape[i];

    // update min and max x,y coordinates
    min_x = min(min_x, point.x);
    max_x = max(max_x, point.x);
    min_y = min(min_y, point.y);
    max_y = max(max_y, point.y);

  }

  // compute the width and height of this shape
  width = (uint)(max_x - min_x);
  height = (uint)(max_y - min_y);
}

/**
 * Filter and process an edge, defined as two points, of a shape.
 * Copies all horizontal edges into output parameter horizontal_edges.
 *   @param horizontal_edges: The mapping of y-coordinate to the list of horizontal edges.
 *   @param edge_start: The first point of the edge to process
 *   @param edge_end: The second point of the edge to process
*/
void shape_process_edge(Point edge_start, Point edge_end, map<uint, vector<Edge>> &horizontal_edges) {

  // if y values are not equal, edge is not horizontal
  if (edge_start.y != edge_end.y) {
    return;
  }

  // edge is horizontal here. Save edge's y coordinate
  int y_coord = edge_start.y;

  // If there is no entry of this y coordinate in the map, add a new list of edges.
  if (horizontal_edges.find(y_coord) == horizontal_edges.end()) {
    horizontal_edges[y_coord] = vector<Edge>();
  }

  // create edge
  Edge edge = Edge(edge_start.x, edge_end.x);

  // push the edge onto the map
  horizontal_edges[y_coord].push_back(edge);
}


/**
 * This function looks up the edges present at the given row and if any, updates the row filter
 * by flipping the boolean values of the blocks implied by these edges.
 *  @params:
 *   horizontal_edges: a dictionary mapping a row number to its horizontal edges
 *   row: current row being processed
 *   row_filter: a boolean array that represents the state of the columns (filled or not filled)
 */
void process_row_filter(map<uint, vector<Edge>> &horizontal_edges, uint row, bool* row_filter) {

  // if there is no edge at this row, no processing to do
  if (horizontal_edges.find(row) == horizontal_edges.end()) {
    return;
  }

  // this row has edges. let's loop through and perform the flips
  vector<Edge> edges = horizontal_edges[row];

  for (uint i = 0; i < edges.size(); i++) {
    // get current edge
    Edge current_edge = edges[i];

    // get the min and max so we can loop through it properly
    int min_x = min(current_edge.start, current_edge.end);
    int max_x = max(current_edge.start, current_edge.end);

    // perform the flips
    for (int col = min_x; col < max_x; col++) {
      row_filter[col] = !row_filter[col];
    }
  }
}



/**
 * Helper function which uses a threshold to find the average of the shortest lengths
 * using this file's threshold constant
 *  @params:
 *   lengths: a vector of lengths
 */
float get_average_shortest_length(vector<float> &lengths){

  if (lengths.size() == 0) {
    // avoid division by zero
    return 0;
  }

  // sort the vector in ascending order
  sort(lengths.begin(), lengths.end());

  // now we wish to keep track of all the lengths that are within a threshold of each other
  // and average them. Total will be used to keep a total count
  float total = lengths[0];

  for (uint i = 1; i < lengths.size(); i++) {

    // gather information about previous and current lengths
    float previous_length = static_cast<float>(lengths[i - 1]);
    float current_length = static_cast<float>(lengths[i]);

    // calculate the length change
    float change = (current_length / previous_length) - 1;

    // stop if the percent change of the next edge's length is bigger than our threshold. returns avg
    if (change > THRESHOLD) {
      return total / i;
    }

    // add current length to total
    total += lengths[i];
  }

  // we will get here if all the lengths are of similar proportions
  return total / lengths.size();
}


/**
 * Helper function which returns the length of the shortest edge in the shape.
 *  @params:
 *   shape: shape as a list of points
 */
float find_shortest_edge_in_shape(const ListOfPoints &shape) {

  // used to store all the edge's lengths
  vector<float> edges_length;

  // get the shape's last point
  Point last_point = shape[shape.size() - 1];

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;

  for (uint i = 0; i < shape.size(); ++i) {

    // get current point
    Point current_point = shape[i];

    // compute the distance between the last processed point and the current point
    float length = current_point.distanceTo(last_processed_point);

    // push it onto the edges length vector
    edges_length.push_back(length);

    // update last processed point
    last_processed_point = current_point;

  }

  return get_average_shortest_length(edges_length);
}


/**
 * Helper function which returns the unit length implied by this set of shapes.
 *  @params:
 *   shapes: a list of shapes (each define as a list of points)
 */
float find_unit_length(const vector<ListOfPoints> &shapes) {

  if (shapes.size() == 0) {
    // avoid division by zero
    return 0;
  }

  // used to store all shortest edges's lengths
  vector<float> shortest_edges_length;

  for (uint i = 0; i < shapes.size(); ++i) {

    // compute this shape's shortest edge's length
    float shortest_edge_length = find_shortest_edge_in_shape(shapes[i]);

    if (shortest_edge_length == 0) {
      continue;
    }

    // add this shape's shortest edge's length to the vector that holds them for all shapes
    shortest_edges_length.push_back(shortest_edge_length);
  }

  return get_average_shortest_length(shortest_edges_length);
}



/**
 * Helper function which changing the coordinates of the given shape to normalize it.
 *  @params:
 *   shape: a shape as a list of points
 *   unit_length: the normalizing constant
 */
void normalize_shape(vector<Point> &shape, float unit_length) {

  for (uint i = 0; i < shape.size(); ++i) {
    // divide each coordinate by the unit length of each shape
    shape[i].x = round(static_cast<float>(shape[i].x) / unit_length);
    shape[i].y = round(static_cast<float>(shape[i].y) / unit_length);
  }

}


/**
 * Helper function which modifies the shapes' coordinate to move it in the first quadrant
 *  @params:
 *   shape: a shape as a list of points
 */
void move_shape_to_first_quadrant(vector<Point> &shape) {

  // set min_x and min_y to the first point of the shape
  int min_x = shape[0].x;
  int min_y = shape[0].y;

  for (uint i = 1; i < shape.size(); ++i) {
    // get current point
    Point current_point = shape[i];

    // update mininum x and y values
    min_x = min(current_point.x, min_x);
    min_y = min(current_point.y, min_y);
  }

  for (uint i = 0; i < shape.size(); i++) {
    // now substract the min x and min y from all the x and y coordinates of shape's points.
    shape[i].x -= min_x;
    shape[i].y -= min_y;
  }
}


/**
 * Translate a given shape into a ShapeMatrix (logical matrix).
 *  @params
 *   shape: a shape as a list of points
 *   matrix: output parameter which contains the logical matrix translation
 */
void shape_translate(const vector<Point> &shape, ShapeMatrix &matrix) {

  // create the row filter, initializing all values to false.
  bool* row_filter = new bool[matrix.getWidth()];
  for (uint i = 0; i < matrix.getWidth(); ++i) {
    row_filter[i] = false;
  }

  // used to store the map of edges organized by their row-value
  map<uint, vector<Edge>> horizontal_edges;

  // get last point of shape
  Point last_point = shape[shape.size() - 1];

  // used to keep track of the last point processed. initially equal to the last point
  Point last_processed_point = last_point;

  for (uint i = 0; i < shape.size(); ++i) {
    // get current point
    Point current_point = shape[i];

    // process the edge from last processed point to the current point
    shape_process_edge(last_processed_point, current_point, horizontal_edges);

    // update the last processed point
    last_processed_point = current_point;
  }

  // process the matrix row-wise
  for (uint row = 0; row < matrix.getHeight(); ++row) {
    // update the row filter
    process_row_filter(horizontal_edges, row, row_filter);

    // process this row, column by column
    for (uint col = 0; col < matrix.getWidth(); ++col) {
      matrix.set(row, col, row_filter[col]);
    }
  }

  // free row_filter
  delete row_filter;
}


bool shape_translate_all_shapes(const vector<ListOfPoints>& shapes,
				vector<ShapeMatrix>& matrices, float unit_length) {

  if (unit_length == 0 || shapes.size() == 0) {
    // avoid division by zero
    return false;
  }

  for (uint i = 0; i < shapes.size(); ++i) {

    // get the current shape
    ListOfPoints shape = shapes[i];

    // normalize the shape using the unit length
    normalize_shape(shape, unit_length);

    // move the shape to the 1st quadrant
    move_shape_to_first_quadrant(shape);

    // get width and height of the shape
    uint width = 0, height = 0;
    shape_get_size(shape, width, height);

    // skip invalid shapes
    if (width == 0 || height == 0) {
      continue;
    }

    // define identifier of current matrix
    int identifier = i + 1;

    // used to hold the matrix
    ShapeMatrix matrix(identifier, width, height);

    // translate the shape
    shape_translate(shape, matrix);

    // push this matrix onto the matrices
    matrices.push_back(matrix);
  }

  return true;
}

bool shape_translate_all_shapes(const vector<ListOfPoints>& shapes, vector<ShapeMatrix>& matrices){

  // get raw unit length implied by the shapes
  float raw_unit_length = find_unit_length(shapes);

  // setup scaling factors
  vector<float> scaling_factors = {1.40};

  for (int i = 1; i < 15; i++){
    scaling_factors.push_back(scaling_factors[0] + i * 0.01);
    scaling_factors.push_back(scaling_factors[0] - i * 0.01);
  }


  for (uint i = 0; i < scaling_factors.size(); i++){

    // clear matrices
    matrices.clear();

    // compute unit_length
    float unit_length = raw_unit_length * scaling_factors[i];

    // attempt to translate shapes
    if (!shape_translate_all_shapes(shapes, matrices, unit_length))
      continue;

    // sort shapes in descending area shape
    sort(matrices.rbegin(), matrices.rend());

    // get area container
    int container_area = matrices[0].getShapeArea();

    // get area of rest of pieces
    int pieces_area = 0;
    for (uint i = 1; i < matrices.size(); i++)
      pieces_area += matrices[i].getShapeArea();

    if (pieces_area == container_area)
      // scaling factor is correct
      return true;

  }

  matrices.clear();
  return false;

}
