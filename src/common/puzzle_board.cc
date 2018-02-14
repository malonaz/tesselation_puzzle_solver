
#include "puzzle_board.h"

#include <cstddef>
#include <iostream>
#include <fstream>

#include "shape_matrix.h"
#include "types.h"

using namespace std;

PuzzleBoard::PuzzleBoard(const ShapeMatrix &shape):
    container(shape),
    current_board(NULL),
    remainingArea(shape.getShapeArea()) {
  int width = shape.getWidth();
  int height = shape.getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = 0;
    }
  }
}

PuzzleBoard::PuzzleBoard(const PuzzleBoard &copy):
    container(copy.container),
    current_board(NULL),
    remainingArea(copy.remainingArea) {
  int width = copy.container.getWidth();
  int height = copy.container.getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = copy.current_board[i][j];
    }
  }
}

uint PuzzleBoard::getHeight() const {
  return this->container.getHeight();
}

uint PuzzleBoard::getWidth() const {
  return this->container.getWidth();
}

bool PuzzleBoard::placePiece(int x, int y, int idx, const ShapeMatrix &piece) {
  int piece_height = piece.getHeight();
  int piece_width = piece.getWidth();
  int container_height = this->container.getHeight();
  int container_width = this->container.getWidth();
  if (idx <= 0) {
    return false;
  }
  if (y < 0 || y >= container_height) {
    return false;
  }
  if (x < 0 || x >= container_width) {
    return false;
  }
  if (y + piece_height > container_height) {
    return false;
  }
  if (x + piece_width > container_width) {
    return false;
  }

  for (int i = 0; i < piece_height; i++) {
    for (int j = 0; j < piece_width; j++) {
      int board_y = y + i;
      int board_x = x + j;
      if (!piece.get(i, j)) {
        continue;
      }
      if (!this->container.get(board_y, board_x)
          || this->current_board[board_y][board_x] > 0) {
        return false;
      }
    }
  }
  for (int i = 0; i < piece_height; i++) {
    for (int j = 0; j < piece_width; j++) {
      int board_y = y + i;
      int board_x = x + j;
      if (piece.get(i, j)) {
        this->current_board[board_y][board_x] = idx;
      }
    }
  }
  this->remainingArea -= piece.getShapeArea();
  return true;
}

bool PuzzleBoard::removePiece(int x, int y, int indexToRemove, const ShapeMatrix &piece) {
  int piece_height = piece.getHeight();
  int piece_width = piece.getWidth();
  int container_height = this->container.getHeight();
  int container_width = this->container.getWidth();

  if (y < 0 || y >= container_height) {
    return false;
  }
  if (x < 0 || x >= container_width) {
    return false;
  }
  if (y + piece_height > container_height) {
    return false;
  }
  if (x + piece_width > container_width) {
    return false;
  }

  for (int i = 0; i < piece_height; i++) {
    for (int j = 0; j < piece_width; j++) {
      int board_y = y + i;
      int board_x = x + j;
      if (this->current_board[board_y][board_x] == indexToRemove) {
        this->current_board[board_y][board_x] = 0;
      }
    }
  }
  this->remainingArea += piece.getShapeArea();
  return true;
}

int** PuzzleBoard::getCurrentBoard() const {
  return this->current_board;
}

uint PuzzleBoard::getRemainingArea() const {
  return this->remainingArea;
}

PuzzleBoard& PuzzleBoard::operator=(const PuzzleBoard& rhs) {
  if (this == &rhs) {
    return *this;
  }

  int container_height = this->container.getHeight();
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;

  this->container = rhs.container;

  int width = rhs.container.getWidth();
  int height = rhs.container.getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = rhs.current_board[i][j];
    }
  }


  return *this;
}

PuzzleBoard::~PuzzleBoard() {
  int container_height = this->container.getHeight();
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;
}
