#include "Evaluation.h"
#include <algorithm>
#include <cstdlib>

Evaluation::Evaluation() {}
namespace {
const int pawnTable[8][8] = {
	{0,0,0,0,0,0,0,0}, {5,10,10,-20,-20,10,10,5}, {5,-5,-10,0,0,-10,-5,5},
	{0,0,0,20,20,0,0,0}, {5,5,10,25,25,10,5,5}, {10,10,20,30,30,20,10,10},
	{50,50,50,50,50,50,50,50}, {0,0,0,0,0,0,0,0}};
const int knightTable[8][8] = {
	{-50,-40,-30,-30,-30,-30,-40,-50}, {-40,-20,0,5,5,0,-20,-40}, {-30,5,10,15,15,10,5,-30},
	{-30,0,15,20,20,15,0,-30}, {-30,5,15,20,20,15,5,-30}, {-30,0,10,15,15,10,0,-30},
	{-40,-20,0,0,0,0,-20,-40}, {-50,-40,-30,-30,-30,-30,-40,-50}};
int pieceSquare(int type, int x, int y, bool white) {
	const int row = white ? y : 7 - y;
	if (type == 1) return pawnTable[row][x];
	if (type == 2) return knightTable[row][x];
	if (type == 3) return 5 * (3 - std::min(3, std::abs(3 - x))) + 5 * (3 - std::min(3, std::abs(3 - y)));
	if (type == 4) return 2 * (white ? 7 - y : y);
	if (type == 5) return 3 * (3 - std::min(3, std::abs(3 - x)));
	return 0;
}
}

int Evaluation::evaluatePosition(const Board& board) { return calculateMaterialScore(board); }
int Evaluation::calculateMaterialScore(const Board& board) {
	const int values[] = {0, 100, 320, 330, 500, 900, 20000};
	int score = 0;
	for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
		const int piece = board.getPiece(x, y);
		const int type = piece < 0 ? -piece : piece;
		if (type >= 1 && type <= 6) {
			const int positional = pieceSquare(type, x, y, piece > 0);
			score += piece > 0 ? values[type] + positional : -values[type] - positional;
		}
	}
	score += 4 * (static_cast<int>(board.generateLegalMoves(true).size()) - static_cast<int>(board.generateLegalMoves(false).size()));
	if (board.isInCheck(true)) score -= 80;
	if (board.isInCheck(false)) score += 80;
	for (int x = 0; x < 8; ++x) {
		int whitePawns = 0, blackPawns = 0;
		for (int y = 0; y < 8; ++y) {
			whitePawns += board.getPiece(x, y) == 1;
			blackPawns += board.getPiece(x, y) == -1;
		}
		if (whitePawns > 1) score -= 15 * (whitePawns - 1);
		if (blackPawns > 1) score += 15 * (blackPawns - 1);
		bool whiteNeighbor = false, blackNeighbor = false;
		for (int neighbor : {x - 1, x + 1}) if (neighbor >= 0 && neighbor < 8) {
			for (int y = 0; y < 8; ++y) {
				whiteNeighbor = whiteNeighbor || board.getPiece(neighbor, y) == 1;
				blackNeighbor = blackNeighbor || board.getPiece(neighbor, y) == -1;
			}
		}
		if (whitePawns && !whiteNeighbor) score -= 10;
		if (blackPawns && !blackNeighbor) score += 10;
	}
	return score;
}