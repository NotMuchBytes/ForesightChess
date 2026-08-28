#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "Board.h"
#include "Move.h"
#include <vector>

class MoveGenerator {
public:
    MoveGenerator(Board* board);
    std::vector<Move> generateLegalMoves();
    
private:
    Board* board;

    bool isMoveLegal(const Move& move);
    void generatePawnMoves(std::vector<Move>& moves);
    void generateKnightMoves(std::vector<Move>& moves);
    void generateBishopMoves(std::vector<Move>& moves);
    void generateRookMoves(std::vector<Move>& moves);
    void generateQueenMoves(std::vector<Move>& moves);
    void generateKingMoves(std::vector<Move>& moves);
};

#endif // MOVEGENERATOR_H