
#include <cstddef>
#include <gtest/gtest.h>
#include "common/shape_matrix.h"
#include "common/puzzle_board.h"

namespace {

TEST(PuzzleBoardTest, DataStructure) {
  ShapeMatrix* shape = new ShapeMatrix(2, 2,2);
  shape->set(0, 0, true);
  shape->set(0, 1, true);
  shape->set(1, 0, true);
  shape->set(1, 1, true);
  PuzzleBoard* board = new PuzzleBoard(*shape);
  int height = 2;
  int width = 2;
  int area = 2 * 2;
  /* test the parameters of the board and the get functions */
  EXPECT_EQ(height, board->getHeight());
  EXPECT_EQ(width, board->getWidth());
  EXPECT_EQ(area, board->getRemainingArea());

  /* test PuzzleBoard initialization of integer 2D array */
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++ ){
      EXPECT_EQ(0, (board->getCurrentBoard())[i][j]);
    }
  }

  delete board;
}

TEST(PuzzleBoardTest, CopyConstructor) {
  ShapeMatrix* shape = new ShapeMatrix(2, 2, 2);
  shape->set(0, 0, true);
  shape->set(0, 1, true);
  shape->set(1, 0, true);
  shape->set(1, 1, true);
  PuzzleBoard* board = new PuzzleBoard(*shape);
  PuzzleBoard* board2 = new PuzzleBoard(*board);

  EXPECT_NE(board, board2);

  delete board;
  delete board2;
}

TEST(PuzzleBoardTest, AddPieceOOB) {
  ShapeMatrix* shape = new ShapeMatrix(2, 2, 2);
  shape->set(0, 0, true);
  shape->set(0, 1, true);
  shape->set(1, 0, true);
  shape->set(1, 1, true);
  PuzzleBoard* board = new PuzzleBoard(*shape);

  ShapeMatrix* shape1 = new ShapeMatrix(2, 1, 1);
  shape1->set(0, 0, true);

  ShapeMatrix* shape2 = new ShapeMatrix(2, 1, 3);
  shape2->set(0, 0, true);
  shape2->set(1, 0, true);
  shape2->set(2, 0, true);

  ShapeMatrix* shape3 = new ShapeMatrix(2, 3, 1);
  shape3->set(0, 0, true);
  shape3->set(0, 1, true);
  shape3->set(0, 2, true);

  /* test the addition of piece*/
  EXPECT_FALSE(board->placePiece(0, 0, *shape1));
  EXPECT_FALSE(board->placePiece(-1, 0, *shape1));
  EXPECT_FALSE(board->placePiece(0,- 1, *shape1));
  EXPECT_FALSE(board->placePiece(2, 0, *shape1));
  EXPECT_FALSE(board->placePiece(0, 2, *shape1));

  /*test addition of oversized piece */
  EXPECT_FALSE(board->placePiece(0, 0, *shape2));
  EXPECT_FALSE(board->placePiece(0, 0, *shape3));

  delete shape1;
  delete shape2;
  delete shape3;
  delete board;
}

TEST(PuzzleBoardTest, AddRemovePiece) {
  ShapeMatrix* shape = new ShapeMatrix(2, 2,2);
  shape->set(0, 0, true);
  shape->set(0, 1, true);
  shape->set(1, 0, true);
  shape->set(1, 1, true);

  PuzzleBoard* board = new PuzzleBoard(*shape);

  ShapeMatrix* shape1 = new ShapeMatrix(2, 1,1);
  shape1->set(0, 0, true);

  /* test the addition of piece*/
  EXPECT_TRUE(board->placePiece(0, 0, *shape1));
  EXPECT_EQ(3, board->getRemainingArea());
  EXPECT_TRUE(board->placePiece(0, 1, *shape1));
  EXPECT_EQ(2, board->getRemainingArea());
  EXPECT_TRUE(board->placePiece(1, 0, *shape1));
  EXPECT_EQ(1, board->getRemainingArea());
  EXPECT_TRUE(board->placePiece(1, 1, *shape1));
  EXPECT_EQ(0, board->getRemainingArea());

  /*test removePiece */
  EXPECT_TRUE(board->removePiece(0, 0, *shape1));
  EXPECT_EQ(1, board->getRemainingArea());
  EXPECT_TRUE(board->removePiece(0, 1, *shape1));
  EXPECT_EQ(2, board->getRemainingArea());
  EXPECT_TRUE(board->removePiece(1, 0, *shape1));
  EXPECT_EQ(3, board->getRemainingArea());
  EXPECT_TRUE(board->removePiece(1, 1, *shape1));
  EXPECT_EQ(4, board->getRemainingArea());

  delete shape1;
  delete board;
}

TEST(PuzzleBoardTest, RemovePieceOOB) {
  ShapeMatrix* shape = new ShapeMatrix(2, 2,2);
  shape->set(0, 0, true);
  shape->set(0, 1, true);
  shape->set(1, 0, true);
  shape->set(1, 1, true);

  PuzzleBoard* board = new PuzzleBoard(*shape);
  ShapeMatrix* shape1 = new ShapeMatrix(2, 1,1);
  shape1->set(0, 0, true);

  ShapeMatrix* shape2 = new ShapeMatrix(2, 1,3);
  shape2->set(0, 0, true);
  shape2->set(1, 0, true);
  shape2->set(2, 0, true);

  ShapeMatrix* shape3 = new ShapeMatrix(2, 3,1);
  shape3->set(0, 0, true);
  shape3->set(0, 1, true);
  shape3->set(0, 2, true);

  /* test the addition of piece*/
  EXPECT_TRUE(board->placePiece(0, 0, *shape1));
  EXPECT_TRUE(board->placePiece(0, 1, *shape1));
  EXPECT_TRUE(board->placePiece(1, 0, *shape1));
  EXPECT_TRUE(board->placePiece(1, 1, *shape1));
  EXPECT_EQ(0, board->getRemainingArea());

  /*test removePieceOOB */
  EXPECT_FALSE(board->removePiece(-1, 0, *shape1));
  EXPECT_FALSE(board->removePiece(0,- 1, *shape1));
  EXPECT_FALSE(board->removePiece(2,- 0, *shape1));
  EXPECT_FALSE(board->removePiece(0, 2, *shape1));

  /*test addition of oversized piece */
  EXPECT_EQ(0, board->getRemainingArea());
  EXPECT_FALSE(board->placePiece(0, 0, *shape2));
  EXPECT_FALSE(board->placePiece(0, 0, *shape3));

  delete shape1;
  delete shape2;
  delete shape3;
  delete board;
}

} // namespace
