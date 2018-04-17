#ifndef APS_SOLVER_SOLVER_H_
#define APS_SOLVER_SOLVER_H_

#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include "common/types.h"

#define UNDERSIZED -1
#define OVERSIZED -2
#define UNSOLVABLE -3
#define INTERNALERROR -4
#define INCORRECT_STATE -5
#define SOLVED 1
#define UNSOLVED 0


PuzzleBoard* create_board(const vector<ShapeMatrix> &matrices,
    vector<ShapeMatrix> &pieces, int& containerArea, int& totalPieceArea);

bool recursive_solver (PuzzleBoard* board,
		       const vector<ShapeMatrix> &pieces,
		       unsigned int currentIndex);

int** copy_board(PuzzleBoard* board);

void delete_2d_array(int** array, uint height);

int** puzzle_solver(const vector<ShapeMatrix> &matrices, int& returnCode,
      uint& board_height, uint& board_width);

bool solvable_config(PuzzleBoard* board,const vector<ShapeMatrix> &pieces,
    uint currentIndex);

int** partial_board(char* directoryName, int* partialBoard, int count, const vector<ShapeMatrix> &allPieces, int& returnCode,
      uint& board_height, uint& board_width);


int** partial_solver(char* directoryName, int* partial_board, int count, const vector<ShapeMatrix> &pieces, int& returnCode,
		     uint& board_height, uint& board_width, bool mute_debugging_messages);
#endif
