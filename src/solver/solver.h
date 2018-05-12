#ifndef APS_SOLVER_SOLVER_H_
#define APS_SOLVER_SOLVER_H_

#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/types.h"

#include <string>

#define UNDERSIZED -1
#define OVERSIZED -2
#define UNSOLVABLE -3
#define INTERNALERROR -4
#define INCORRECT_STATE -5
#define SOLVED 1
#define UNSOLVED 0

using std::string;

PuzzleBoard* create_board(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& containerArea, int& totalPieceArea);

bool recursive_solver (PuzzleBoard* board,
		       const vector<ShapeMatrix> &pieces,
		       unsigned int currentIndex);

int** copy_board(PuzzleBoard* board);

void delete_2d_array(int** array, uint height);

int** puzzle_solver(const vector<ShapeMatrix> &matrices, int& returnCode,
      uint& board_height, uint& board_width);

int** partial_solver(string puzzle_directory, int* partial_board, const vector<ShapeMatrix> &pieces, int& returnCode,
		     uint& board_height, uint& board_width, bool mute_debugging_messages);
#endif
