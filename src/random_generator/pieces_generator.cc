#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include "common/coordinates_io.h"
#include "common/debugger.h"
#include "common/memory.h"
#include "common/point.h"
#include "common/puzzle_board.h"
#include "common/shape_matrix_io.h"
#include "common/shape_matrix.h"
#include "common/types.h"
#include "common/utils.h"

using namespace std;


#define EXPECTED_ARG_NUM {4}
#define ARG_FORMAT "bin/rg <max_piece_height> <max_piece_width> <number_pieces> <max_piece_area:greater than 0>"

// take in parameters of max piece height, max piece width, no. of pieces, max area of each piece

class Coord {
  uint x;
  uint y;
public:
  Coord(uint a=0, uint b=0){
    x = a;
    y = b;
  }
  uint getX() {return this->x;}
  uint getY() {return this->y;}
};

int main(int argc, char** argv) {
  
  // prepare parameters
  int max_height = atoi(argv[1]);
  int max_width = atoi(argv[2]);
  int pieces_count = atoi(argv[3]);
  int max_area = atoi(argv[4]);
  
  if (!valid_args(argc, EXPECTED_ARG_NUM, ARG_FORMAT))
    return -1;
  if (max_area ==0){
    return 0;
  }
  const int MIN = 1;
  
  vector<ShapeMatrix*> piece_container;

  
  for (int i=0; i < pieces_count; i++){
    cout << "piece number" << i <<endl;
    int height = MIN + (rand() % static_cast<int>(max_height - MIN + 1));
    int width = MIN + (rand() % static_cast<int>(max_width - MIN + 1));
    int mat_area  = height*width;
    int piece_area = MIN + (rand() % static_cast<int>(max_area - MIN + 1));
    
    ShapeMatrix* temp = new ShapeMatrix(i, width, height);
    cout << width << " " << height <<endl;
    if (mat_area == 1){
      temp->set(0,0, true);
    }
    else if (piece_area >= mat_area){
      for (uint h = 0; h < temp->getHeight(); h++){
        for (uint c =0; c < temp->getWidth(); c++){
          temp->set(h,c,true);
        }
      }
    }
    else {
      uint h = (MIN + (rand() % static_cast<int>(max_height - MIN + 1)))-1;
      uint c = (MIN + (rand() % static_cast<int>(max_width - MIN + 1))) -1;
      temp->set(h,c, true);
      --piece_area;
      while (piece_area > 0){
        vector<Coord*> store;
        if (h == 0 && c >= 0 && c < temp->getWidth()){
          if(!temp->get(h+1, c)){
            store.push_back(new Coord(h+1, c));
          }
          if (c+1 < temp->getWidth()){
            if(!temp->get(h, c+1)){
              store.push_back(new Coord(h, c+1));
            }
          }
          if (c-1 != 4294967295){
            if(!temp->get(h, c-1)){
              store.push_back(new Coord(h, c-1));
            }
          }
        }
        
        if (h == temp->getHeight()  && c >= 0 && c < temp->getWidth()){
          if(!temp->get(h-1, c)){
            store.push_back(new Coord(h-1, c));
          }
          if (c+1 < temp->getWidth()-1){
            if(!temp->get(h, c+1)){
              store.push_back(new Coord(h, c+1));
            }
          }
          if (c-1 != 4294967295){
            if(!temp->get(h, c-1)){
              store.push_back(new Coord(h, c-1));
            }
          }
        }
        
        if (h > 0 && h < temp->getHeight()-1 && c >= 0  && c < temp->getWidth()){
          if(!temp->get(h+1, c)){
            store.push_back(new Coord(h+1, c));
          }
          if(!temp->get(h-1, c)){
            store.push_back(new Coord(h-1, c));
          }
          if (c+1 < temp->getWidth()-1){
            if(!temp->get(h, c+1)){
              store.push_back(new Coord(h, c+1));
            }
          }
          if (c-1 != 4294967295){
            if(!temp->get(h, c-1)){
              store.push_back(new Coord(h, c-1));
            }
          }
        }
        
        int size = store.size();
        if (size == 0){
          break;
        }
        int pick = 0;
        if (size > 1) {
          pick = (1 + (rand() % static_cast<int>(size - 1 + 1)))-1;
          temp->set(store[pick]->getY(), store[pick]->getX());
        }
        h = store[pick]->getY();
        c = store[pick]->getX();
        store.clear();
      }
    }
    piece_container.push_back(temp);
    shape_matrix_print(temp);
    delete temp;
  }
  for (vector<ShapeMatrix*>::iterator it = piece_container.begin() ; it != piece_container.end(); ++it){
    shape_matrix_print(*it);
  }
  return 0;
}
