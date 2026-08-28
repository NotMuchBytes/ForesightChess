#include "MoveGenerator.h"
#include "MoveGenerator.h"

MoveGenerator::MoveGenerator(Board* board) : board(board) {}
std::vector<Move> MoveGenerator::generateLegalMoves() {
    return board ? board->generateLegalMoves(true) : std::vector<Move>();
}
bool MoveGenerator::isMoveLegal(const Move& move) { return board && board->isValidMove(move); }
void MoveGenerator::generatePawnMoves(std::vector<Move>&) {}
void MoveGenerator::generateKnightMoves(std::vector<Move>&) {}
void MoveGenerator::generateBishopMoves(std::vector<Move>&) {}
void MoveGenerator::generateRookMoves(std::vector<Move>&) {}
void MoveGenerator::generateQueenMoves(std::vector<Move>&) {}
void MoveGenerator::generateKingMoves(std::vector<Move>&) {}