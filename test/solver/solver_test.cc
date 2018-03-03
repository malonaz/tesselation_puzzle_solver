#include <cstddef>
#include <gtest/gtest.h>
#include "common/point.h"
#include "common/types.h"
#include "discretizer/shape_rotate.h"
#include "common/shape_matrix.h"
#include "common/puzzle_board.h"
#include <vector>
#include "solver/solver.h"
#include "common/memory.h"
using namespace std;
namespace {

      TEST(SolverTest, BoardCreation) {
        //this is the container
        ShapeMatrix* shape = new ShapeMatrix(2, 3, 3);
        shape->set(0, 0, true);
        shape->set(0, 1, true);
        shape->set(0, 2, true);
        shape->set(1, 0, true);
        shape->set(1, 1, true);
        shape->set(1, 2, true);
        shape->set(2, 0, true);
        shape->set(2, 1, true);
        shape->set(2, 2, true);

        //Big L shape (3 by 3, area 5)
        ShapeMatrix* shape1 = new ShapeMatrix(2, 3, 3);
        shape1->set(0, 0, true);
        shape1->set(0, 1, true);
        shape1->set(0, 2, true);
        shape1->set(1, 0, true);
        shape1->set(2, 0, true);

        //Small L shape (2 by 2, area 3)
        ShapeMatrix* shape2 = new ShapeMatrix(2, 2, 2);
        shape2->set(0, 0, true);
        shape2->set(0, 1, true);
        shape2->set(1, 0, true);

        //Single unit-sized block ()
        ShapeMatrix* shape3 = new ShapeMatrix(2, 1, 1);
        shape3->set(0, 0, true);

        vector<ShapeMatrix> matrices;
        //shapes placed in random order
        matrices.push_back(*shape2);
        matrices.push_back(*shape);
        matrices.push_back(*shape3);
        matrices.push_back(*shape1);
        vector<ShapeMatrix> shapes;
//        const vector<ShapeMatrix> matrices1;
        int containerArea = 0;
        int totalPieceArea = 0;

        PuzzleBoard* board = createBoard(matrices, shapes,
            containerArea, totalPieceArea);


            /*Check total area, chec number of shapes?*/


        EXPECT_EQ(shape->getShapeArea(), board->getRemainingArea());

        EXPECT_EQ(solvableConfig(board,shapes, 0), true);

        int returnCode = 0;
        uint board_height=0, board_width=0;
        int** solution = NULL;
        solution = puzzleSolver(matrices, returnCode,
              board_height, board_width);
        int filledArea = 0;

        for (int i = 0; i <3; i++){
          for (int j = 0; j <3; j++){
            if (solution[i][j])
            { filledArea++;}
          }
        }
        EXPECT_EQ(filledArea,9);
        // delete shape1;
        // delete shape2;
        // delete shape3;
        delete board;
      }


} // namespace
