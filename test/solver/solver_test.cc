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

      class SolverTest: public ::testing::Test{
        public:
          //setup always called before the test starts
          virtual void SetUp(){
            //this is the container
            ShapeMatrix* shape = new ShapeMatrix(1, 3, 3);
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

            //Small L shape (Area 3)
            ShapeMatrix* shape2 = new ShapeMatrix(3, 2, 2);
            shape2->set(0, 0, true);
            shape2->set(0, 1, true);
            shape2->set(1, 0, true);

            //Single unit-sized block
            ShapeMatrix* shape3 = new ShapeMatrix(4, 1, 1);
            shape3->set(0, 0, true);

            vector<ShapeMatrix> matrices;
            matrices.push_back(*shape1);
            matrices.push_back(*shape2);
            matrices.push_back(*shape3);
            matrices.push_back(*shape);

            vector<ShapeMatrix> shapes;

            int containerArea = 0, totalPieceArea = 0;

            board1 = createBoard(matrices, shapes,
                containerArea, totalPieceArea);
          }

          //teardown always called after the test ends
          virtual void TearDown(){
            delete board1;
          }

          PuzzleBoard* board1;
          //PuzzleBoard* board2;
      };




      TEST_F(SolverTest, testPop){
        EXPECT_EQ(9, board1->getRemainingArea());

      }


      TEST(SolverTest1, BoardCreation_Solvable) {
        //this is the container
        ShapeMatrix* shape = new ShapeMatrix(1, 3, 3);
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
        ShapeMatrix* shape1 = new ShapeMatrix(2,3, 3);
        shape1->set(0, 0, true);
        shape1->set(0, 1, true);
        shape1->set(0, 2, true);
        shape1->set(1, 0, true);
        shape1->set(2, 0, true);

        //Small L shape (2 by 2, area 3)
        ShapeMatrix* shape2 = new ShapeMatrix(3, 2, 2);
        shape2->set(0, 0, true);
        shape2->set(0, 1, true);
        shape2->set(1, 0, true);

        //Single unit-sized block ()
        ShapeMatrix* shape3 = new ShapeMatrix(4, 1, 1);
        shape3->set(0, 0, true);

        vector<ShapeMatrix> matrices;
        matrices.push_back(*shape2);
        matrices.push_back(*shape);
        matrices.push_back(*shape3);
        matrices.push_back(*shape1);

        vector<ShapeMatrix> shapes;

        int containerArea = 0, totalPieceArea = 0;

        PuzzleBoard* board1 = createBoard(matrices, shapes,
            containerArea, totalPieceArea);

        //Area check: largest piece within vector selected as container
        EXPECT_EQ(9, board1->getRemainingArea());

        //Area check: total area of shape pieces equal the container's
        EXPECT_EQ(shape->getShapeArea(), board1->getRemainingArea());

        //Height and width check
        uint height = board1->getHeight();
        uint width = board1->getWidth();
        EXPECT_EQ(3, height);
        EXPECT_EQ(3, width);

        //Empty container check: container should be initialized as empty
        for (uint i = 0; i < height; i++) {
          for (uint j = 0; j < width; j++ ) {
            EXPECT_EQ(0 , (board1->getCurrentBoard())[i][j]);
          }
        }

        //Solvable state check: puzzle board is initialized in a solvable configuration
        EXPECT_EQ(solvableConfig(board1,shapes, 0), true);

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
        // EXPECT_EQ(filledArea,9);
        // delete shape1;
        // delete shape2;
        // delete shape3;
        delete board1;
      }


} // namespace
