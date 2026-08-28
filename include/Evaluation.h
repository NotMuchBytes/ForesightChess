#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h"

class Evaluation {
public:
    // Constructor
    Evaluation();

    // Method to evaluate the board position
    int evaluatePosition(const Board& board);

private:
    // Scoring values for pieces
    static const int pawnValue = 100;
    static const int knightValue = 320;
    static const int bishopValue = 330;
    static const int rookValue = 500;
    static const int queenValue = 900;
    static const int kingValue = 20000;

    // Method to calculate material score
    int calculateMaterialScore(const Board& board);
};

#endif // EVALUATION_H