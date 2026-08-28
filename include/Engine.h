#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"
#include "Move.h"
#include "Evaluation.h"
#include <vector>
#include <unordered_map>
#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    void setBoard(const Board& board);
    Move getBestMove(int depth);
    std::vector<Move> generatePossibleMoves();
    int evaluatePosition(const Board& board);

private:
    Board currentBoard;
    Evaluation evaluator;
    std::unordered_map<std::string, int> transpositionTable;

    int minimax(const Board& board, int depth, bool isMaximizing, int alpha, int beta);
};

#endif // ENGINE_H