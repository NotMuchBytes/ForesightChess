#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include <string>
#include "Move.h"

class Board {
public:
    Board();
    void reset();
    bool isValidMove(const Move& move) const;
    void makeMove(const Move& move);
    bool isInCheck(bool white) const;
    bool isCheckmate(bool white) const;
    bool isStalemate(bool white) const;
    std::vector<Move> generateLegalMoves(bool white) const;
    int getPiece(int x, int y) const;
    bool isWhiteTurn() const;
    std::string getPositionKey() const;

private:
    std::array<std::array<int, 8>, 8> board; // 0 for empty, other values for pieces
    bool whiteToMove;
    bool whiteKingSide, whiteQueenSide, blackKingSide, blackQueenSide;
    int enPassantX, enPassantY;
    void updateBoard(const Move& move);
    bool isMoveLegal(const Move& move) const;
    bool isPseudoLegal(const Move& move, bool enforceTurn) const;
    bool isSquareAttacked(int x, int y, bool byWhite) const;
};

#endif // BOARD_H