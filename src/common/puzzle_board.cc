#include "puzzle_board.h"
#include "shape_matrix.h"
#include <iostream>

using namespace std;

PuzzleBoard::PuzzleBoard(ShapeMatrix* shape){
  this->container = shape;
  int width = shape->getWidth();
  int height = shape->getHeight();
  this->current_board = new int*[height];
  for(int i = 0; i < height; i++){
    current_board[i] = new int[width];
    for(int j= 0; j < width; j++){
        current_board[i][j] = 0;
    }
  }
}

int PuzzleBoard::getHeight() {
  return this->container->getHeight();
}

int PuzzleBoard::getWidth() {
  return this->container->getWidth();
}

bool PuzzleBoard::placePiece(int x, int y, int idx, ShapeMatrix* piece){
  int piece_height = piece->getHeight();
  int piece_width = piece->getWidth();
  int container_height = this->container->getHeight();
  int container_width = this->container->getWidth();
  if(idx <= 0){
    return false;
  }
  if (y < 0 || y >= container_height){
//cout<<"idx is: "<<idx<<". fail here 1"<<endl;
    return false;
  }
  if (x < 0 || x >= container_width){
//cout<<"idx is: "<<idx<<". fail here 2"<<endl;
    return false;
  }
  if (y+piece_height > container_height){
//cout<<"idx is: "<<idx<<". fail here 3"<<endl;
    return false;
  }
  if (x+piece_width > container_width){
//cout<<"idx is: "<<idx<<". fail here 4"<<endl;
    return false;
    //cout<<"Trying piece number: "<<(currentIndex+1)<<"at coordinates "<<r <<", "<<c<<endl;

  }

  for (int i = 0; i < piece_height; i++){
    for (int j = 0; j < piece_width; j++){
      int board_y = y + i;
      int board_x = x + j;
      if (!piece->get(i, j)){
        continue;
      }
      if(!this->container->get(board_y, board_x)
          || this->current_board[board_y][board_x] > 0){
//            cout<<"idx is: "<<idx<<". fail here 5"<<endl;
        return false;
      }

    }
  }
  for (int i = 0; i < piece_height; i++){
    for (int j = 0; j < piece_width; j++){
      int board_y = y + i;
      int board_x = x + j;
      if (piece->get(i, j)) {
        this->current_board[board_y][board_x] = idx;
      }
    }
  }
  return true;
}

bool PuzzleBoard::removePiece(int x, int y, int indexToRemove){
  int container_height = this->container->getHeight();
  int container_width = this->container->getWidth();

  for (int i = 0; i < container_height; i++){
    for (int j = 0; j < container_width; j++){
      if(this->current_board[i][j] == indexToRemove) {
        this->current_board[i][j] = 0;
      }
    }
  }
  return true;
}

int** PuzzleBoard::getCurrentBoard(){
  return this->current_board;
}

int PuzzleBoard::getRemainingArea(){
  int area =0;
  int container_height = this->container->getHeight();
  int container_width = this->container->getWidth();
  for (int i = 0; i < container_height; i++){
    for (int j = 0; j < container_width; j++){
      if(this->container->get(i, j)
          && this->current_board[i][j] == 0){
        area++;
      }
    }
  }
  return area;
}

void PuzzleBoard::printBoard(){
for (int i = 0; i < this->container->getHeight(); i++){
  for (int j = 0; j < this->container->getWidth(); j++){
    cout << current_board[i][j];
    }
    cout << endl;
  }
}

PuzzleBoard::~PuzzleBoard(){
  int container_height = this->container->getHeight();
  delete container;
  for(int i = 0; i < container_height; i++){
    delete[] current_board[i];
  }
  delete[] current_board;
}
