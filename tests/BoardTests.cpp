#include "Board.h"
#include <gtest/gtest.h>

class BoardTests : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initialize(); // Assuming there's an initialize method to set up the board
    }
};

TEST_F(BoardTests, InitialPosition) {
    EXPECT_EQ(board.getPieceAt(0, 0), PieceType::Rook);
    EXPECT_EQ(board.getPieceAt(0, 1), PieceType::Knight);
    EXPECT_EQ(board.getPieceAt(0, 2), PieceType::Bishop);
    EXPECT_EQ(board.getPieceAt(0, 3), PieceType::Queen);
    EXPECT_EQ(board.getPieceAt(0, 4), PieceType::King);
    EXPECT_EQ(board.getPieceAt(0, 5), PieceType::Bishop);
    EXPECT_EQ(board.getPieceAt(0, 6), PieceType::Knight);
    EXPECT_EQ(board.getPieceAt(0, 7), PieceType::Rook);
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(board.getPieceAt(1, i), PieceType::Pawn);
        EXPECT_EQ(board.getPieceAt(6, i), PieceType::Pawn);
    }
}

TEST_F(BoardTests, MoveValidation) {
    EXPECT_TRUE(board.isMoveValid(1, 0, 2, 0)); // Pawn move
    EXPECT_FALSE(board.isMoveValid(1, 0, 3, 0)); // Invalid pawn move
    EXPECT_TRUE(board.isMoveValid(0, 1, 2, 2)); // Knight move
    EXPECT_FALSE(board.isMoveValid(0, 1, 2, 3)); // Invalid knight move
}

TEST_F(BoardTests, CheckDetection) {
    board.movePiece(1, 0, 2, 0); // Move pawn
    board.movePiece(0, 1, 2, 2); // Move knight
    EXPECT_FALSE(board.isInCheck(Color::White));
    board.movePiece(0, 3, 4, 7); // Move queen to threaten king
    EXPECT_TRUE(board.isInCheck(Color::White));
}

TEST_F(BoardTests, CheckmateDetection) {
    // Set up a checkmate position
    board.setPieceAt(0, 4, PieceType::King, Color::Black);
    board.setPieceAt(0, 3, PieceType::Queen, Color::White);
    EXPECT_TRUE(board.isCheckmate(Color::Black));
}