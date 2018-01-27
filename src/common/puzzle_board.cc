
#include "puzzle_board.h"

#include <cstddef>
#include <iostream>
#include <fstream>

#include "shape_matrix.h"
#include "types.h"

using namespace std;

PuzzleBoard::PuzzleBoard(ShapeMatrix* shape):
    container(shape),
    current_board(NULL),
    remainingArea(shape->getShapeArea()) {
  int width = shape->getWidth();
  int height = shape->getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = 0;
    }
  }
}

PuzzleBoard::PuzzleBoard(const PuzzleBoard &copy):
    container(new ShapeMatrix(*copy.container)),
    current_board(NULL),
    remainingArea(copy.remainingArea) {
  int width = copy.container->getWidth();
  int height = copy.container->getHeight();
  this->current_board = new int*[height];
  for (int i = 0; i < height; i++) {
    current_board[i] = new int[width];
    for (int j = 0; j < width; j++) {
      current_board[i][j] = copy.current_board[i][j];
    }
  }
}

int PuzzleBoard::getHeight() const {
  return this->container->getHeight();
}

int PuzzleBoard::getWidth() const {
  return this->container->getWidth();
}

bool PuzzleBoard::placePiece(int x, int y, int idx, ShapeMatrix* piece) {
  int piece_height = piece->getHeight();
  int piece_width = piece->getWidth();
  int container_height = this->container->getHeight();
  int container_width = this->container->getWidth();
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
      if (!piece->get(i, j)){
        continue;
      }
      if (!this->container->get(board_y, board_x)
          || this->current_board[board_y][board_x] > 0) {
        return false;
      }

    }
  }
  for (int i = 0; i < piece_height; i++) {
    for (int j = 0; j < piece_width; j++) {
      int board_y = y + i;
      int board_x = x + j;
      if (piece->get(i, j)) {
        this->current_board[board_y][board_x] = idx;
      }
    }
  }
  this->remainingArea -= piece->getShapeArea();
  return true;
}

bool PuzzleBoard::removePiece(int x, int y, int indexToRemove, ShapeMatrix* piece) {
  int piece_height = piece->getHeight();
  int piece_width = piece->getWidth();
  int container_height = this->container->getHeight();
  int container_width = this->container->getWidth();

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
        ++this->remainingArea;
      }
    }
  }
  return true;
}

int** PuzzleBoard::getCurrentBoard() const {
  return this->current_board;
}

int PuzzleBoard::getRemainingArea() const {
  return this->remainingArea;
}

void PuzzleBoard::printBoard() const {
  int width = this->container->getWidth();
  int height = this->container->getHeight();
  cout << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  cout << "+";
  for (int c = 0; c < width; ++c) {
    if (c < width - 1
        && this->current_board[0][c] == this->current_board[0][c + 1]) {
      cout <<  "-----";
    } else {
      cout <<  "----+";
    }
  }
  cout << endl;
  for (int r = 0; r < height; ++r) {
    cout << "| ";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      if (current_num<10) {
        cout<<" ";
      }
      cout << current_num << " ";
      if (c < width - 1
          && this->current_board[r][c + 1] == current_num) {
        cout << "  ";
      } else {
        cout << "| ";
      }
    }
    cout << endl;
    cout << "+";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (this->current_board[r + 1][c + 1] == current_num
            && this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
          cout << "     ";
        } else if (this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
              cout << "    +";
        } else if (this->current_board[r + 1][c] == current_num) {
          cout << "    |";
        } else if (this->current_board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else if (r < height - 1 && c == width - 1) {
        if (this->current_board[r + 1][c] == current_num) {
          cout << "    |";
        } else {
          cout << "----+";
        }
      } else if(r == height - 1 && c < width - 1) {
        if (this->current_board[r][c + 1] == current_num) {
          cout << "-----";
        } else {
          cout << "----+";
        }
      } else {
        cout << "----+";
      }
    }
    cout << endl;
  }
}



void PuzzleBoard::printBoard(ostream& os) const {
  int width = this->container->getWidth();
  int height = this->container->getHeight();
  os << "-- [ rows: " << height << ", cols: " << width << " ] --" << endl;
  os << "+";
  for (int c = 0; c < width; ++c) {
    if (c < width - 1
        && this->current_board[0][c] == this->current_board[0][c + 1]) {
      os <<  "----";
    } else {
      os <<  "---+";
    }
  }
  os << endl;
  for (int r = 0; r < height; ++r) {
    os << "| ";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      os << current_num << " ";
      if (c < width - 1
          && this->current_board[r][c + 1] == current_num) {
        os << "  ";
      } else {
        os << "| ";
      }
    }
    os << endl;
    os << "+";
    for (int c = 0; c < width; ++c) {
      int current_num = this->current_board[r][c];
      if (r < height - 1 && c < width - 1) {
        if (this->current_board[r + 1][c + 1] == current_num
            && this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
          os << "    ";
        } else if (this->current_board[r + 1][c] == current_num
            && this->current_board[r][c + 1] == current_num) {
              os << "   +";
        } else if (this->current_board[r + 1][c] == current_num) {
          os << "   |";
        } else if (this->current_board[r][c + 1] == current_num) {
          os << "----";
        } else {
          os << "---+";
        }
      } else if (r < height - 1 && c == width - 1) {
        if (this->current_board[r + 1][c] == current_num) {
          os << "   |";
        } else {
          os << "---+";
        }
      } else if(r == height - 1 && c < width - 1) {
        if (this->current_board[r][c + 1] == current_num) {
          os << "----";
        } else {
          os << "---+";
        }
      } else {
        os << "---+";
      }
    }
    os << endl;
  }
}



PuzzleBoard& PuzzleBoard::operator=(const PuzzleBoard& rhs) {
  if (this == &rhs) {
    return *this;
  }

  int container_height = this->container->getHeight();
  delete container;
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;

  this->container = new ShapeMatrix(*rhs.container);

  int width = rhs.container->getWidth();
  int height = rhs.container->getHeight();
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
  int container_height = this->container->getHeight();
  delete container;
  for (int i = 0; i < container_height; i++) {
    delete[] current_board[i];
  }
  delete[] current_board;
}
