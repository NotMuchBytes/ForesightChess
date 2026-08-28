#include "Engine.h"
#include "Board.h"
#include "Move.h"
#include "Evaluation.h"
#include <vector>
#include <limits>
#include <algorithm>

Engine::Engine() {}
Engine::~Engine() {}
void Engine::setBoard(const Board& board) { currentBoard = board; }
std::vector<Move> Engine::generatePossibleMoves() { return currentBoard.generateLegalMoves(true); }
int Engine::evaluatePosition(const Board& board) { return evaluator.evaluatePosition(board); }

Move Engine::getBestMove(int depth) {
    transpositionTable.clear();
    const bool engineWhite = currentBoard.isWhiteTurn();
    const std::vector<Move> moves = currentBoard.generateLegalMoves(engineWhite);
    if (moves.empty()) return Move();
    Move best = moves.front();
    int bestScore = engineWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    for (const Move& move : moves) {
        Board next = currentBoard;
        next.makeMove(move);
        int score = minimax(next, depth > 0 ? depth - 1 : 0, true,
                    std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        if ((engineWhite && score > bestScore) || (!engineWhite && score < bestScore)) {
            bestScore = score; best = move;
        }
    }
    return best;
}

int Engine::minimax(const Board& board, int depth, bool maximizing, int alpha, int beta) {
    if (depth == 0) return evaluatePosition(board);
    const std::string key = board.getPositionKey() + std::to_string(depth) + (maximizing ? "w" : "b");
    const auto cached = transpositionTable.find(key);
    if (cached != transpositionTable.end()) return cached->second;
    const std::vector<Move> moves = board.generateLegalMoves(maximizing);
    if (moves.empty()) {
        if (board.isInCheck(maximizing)) return maximizing ? -1000000 : 1000000;
        return 0;
    }
    std::vector<Move> ordered = moves;
    std::sort(ordered.begin(), ordered.end(), [&board](const Move& a, const Move& b) {
        return std::abs(board.getPiece(b.getToX(), b.getToY())) < std::abs(board.getPiece(a.getToX(), a.getToY()));
    });
    int result = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    bool cutoff = false;
    for (const Move& move : ordered) {
        Board next = board;
        next.makeMove(move);
        int value = minimax(next, depth - 1, !maximizing, alpha, beta);
        result = maximizing ? std::max(result, value) : std::min(result, value);
        if (maximizing) alpha = std::max(alpha, result);
        else beta = std::min(beta, result);
        if (beta <= alpha) { cutoff = true; break; }
    }
    if (!cutoff) transpositionTable[key] = result;
    return result;
}