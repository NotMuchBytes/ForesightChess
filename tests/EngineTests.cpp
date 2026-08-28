#include "Engine.h"
#include <gtest/gtest.h>

class EngineTests : public ::testing::Test {
protected:
    Engine engine;

    void SetUp() override {
        // Initialize the engine or set up any necessary state before each test
    }

    void TearDown() override {
        // Clean up any resources after each test if necessary
    }
};

TEST_F(EngineTests, TestInitialPositionEvaluation) {
    // Arrange
    Board board;
    board.setInitialPosition();
    
    // Act
    int evaluation = engine.evaluate(board);
    
    // Assert
    EXPECT_EQ(evaluation, 0); // Initial position should be neutral
}

TEST_F(EngineTests, TestMinimaxBasicFunctionality) {
    // Arrange
    Board board;
    board.setInitialPosition();
    
    // Act
    Move bestMove = engine.findBestMove(board, 3); // Search depth of 3
    
    // Assert
    EXPECT_NE(bestMove, Move()); // Ensure a valid move is returned
}

TEST_F(EngineTests, TestAlphaBetaPruning) {
    // Arrange
    Board board;
    board.setInitialPosition();
    
    // Act
    Move bestMove = engine.findBestMove(board, 4); // Search depth of 4
    
    // Assert
    EXPECT_NE(bestMove, Move()); // Ensure a valid move is returned
}

TEST_F(EngineTests, TestEvaluationAfterMove) {
    // Arrange
    Board board;
    board.setInitialPosition();
    Move move = Move(/* parameters for a specific move */);
    board.makeMove(move);
    
    // Act
    int evaluation = engine.evaluate(board);
    
    // Assert
    EXPECT_GT(evaluation, -10000); // Ensure evaluation is reasonable after a move
}