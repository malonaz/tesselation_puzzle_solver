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
  uint r;
  uint c;
public:
  Coord(uint a=0, uint b=0){
    r = a;
    c = b;
  }
  uint getR() {return this->r;}
  uint getC() {return this->c;}
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
      for (uint row = 0; row < temp->getHeight(); row++){
        for (uint col =0; col < temp->getWidth(); col++){
          temp->set(row,col,true);
        }
      }
    }

    else {
      uint r = (MIN + (rand() % static_cast<int>(height - MIN + 1)))-1;
      uint c = (MIN + (rand() % static_cast<int>(width - MIN + 1))) -1;
      assert (r >= 0 && r < temp->getHeight());
      assert (c >= 0 && c < temp->getWidth());
      temp->set(r,c, true);
      --piece_area;

      while (piece_area > 0){
        vector<Coord*> store;

        if (r == 0 && c >= 0 && c < temp->getWidth()){
          if (r+1 < temp->getHeight()){
            if(!temp->get(r+1, c)){
              store.push_back(new Coord(r+1, c));
            }
          }
          if (c+1 < temp->getWidth()){
            if(!temp->get(r, c+1)){
              store.push_back(new Coord(r, c+1));
            }
          }
          if (c-1 < 4294967295){
            if(!temp->get(r, c-1)){
              store.push_back(new Coord(r, c-1));
            }
          }
        }

        if (r == temp->getHeight()-1  && c >= 0 && c < temp->getWidth()){
          if (r+1 < temp->getHeight()){
            if(!temp->get(r-1, c)){
              store.push_back(new Coord(r-1, c));
            }
          }
          if (c+1 < temp->getWidth()-1){
            if(!temp->get(r, c+1)){
              store.push_back(new Coord(r, c+1));
            }
          }
          if (c-1 < 4294967295){
            if(!temp->get(r, c-1)){
              store.push_back(new Coord(r, c-1));
            }
          }
        }

        if (r > 0 && r < temp->getHeight()-1 && c >= 0  && c < temp->getWidth()){
          if(!temp->get(r+1, c)){
            store.push_back(new Coord(r+1, c));
          }
          if(!temp->get(r-1, c)){
            store.push_back(new Coord(r-1, c));
          }
          if (c+1 < temp->getWidth()-1){
            if(!temp->get(r, c+1)){
              store.push_back(new Coord(r, c+1));
            }
          }
          if (c-1 < 4294967295){
            if(!temp->get(r, c-1)){
              store.push_back(new Coord(r, c-1));
            }
          }
        }
        int size = store.size();
        if (size == 0){
          break;
        }
        int pick = 0;
        if (size >= 1) {
          pick = (1 + (rand() % static_cast<int>(size - 1 + 1)))-1;
          temp->set(store[pick]->getR(), store[pick]->getC(), true);
        }
        r = store[pick]->getR();
        c = store[pick]->getC();
        --piece_area;
        store.clear();
      }
    }

    piece_container.push_back(temp);
  }
  int count = 0;
  for (vector<ShapeMatrix*>::iterator it = piece_container.begin() ; it != piece_container.end(); ++it){
    cout << " Piece Number: " << count <<endl;
    shape_matrix_print(*it);
    count++;
  }
  return 0;
}
