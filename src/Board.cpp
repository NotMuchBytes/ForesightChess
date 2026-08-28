#include "Board.h"
#include <stdexcept>
#include <string>

namespace {
int absInt(int value) { return value < 0 ? -value : value; }
}

Board::Board() { reset(); }

void Board::reset() {
    for (auto& row : board) row.fill(0);
    board[0] = {{-4, -2, -3, -5, -6, -3, -2, -4}};
    board[1] = {{-1, -1, -1, -1, -1, -1, -1, -1}};
    board[6] = {{1, 1, 1, 1, 1, 1, 1, 1}};
    board[7] = {{4, 2, 3, 5, 6, 3, 2, 4}};
    whiteToMove = true;
    whiteKingSide = whiteQueenSide = blackKingSide = blackQueenSide = true;
    enPassantX = enPassantY = -1;
}

void Board::setupPuzzle(int puzzleNumber) {
    reset();
    for (auto& row : board) row.fill(0);
    const int pattern = (puzzleNumber - 1) % 6;
    board[2][6] = 6;
    board[0][6] = -6;
    if (pattern == 0) { board[3][7] = 5; board[7][5] = 4; }
    if (pattern == 1) { board[3][0] = 5; board[7][2] = 4; board[0][1] = -6; board[0][6] = 0; }
    if (pattern == 2) { board[3][4] = 5; board[7][0] = 4; board[5][7] = 3; board[0][3] = -6; board[0][6] = 0; }
    if (pattern == 3) { board[3][4] = 5; board[7][7] = 4; board[5][0] = 3; board[0][5] = -6; board[0][6] = 0; }
    if (pattern == 4) { board[3][1] = 5; board[7][6] = 4; board[5][7] = 2; board[0][2] = -6; board[0][6] = 0; }
    if (pattern == 5) { board[3][5] = 5; board[7][1] = 4; board[5][7] = 3; board[0][4] = -6; board[0][6] = 0; }
    whiteToMove = true;
    whiteKingSide = whiteQueenSide = blackKingSide = blackQueenSide = false;
    enPassantX = enPassantY = -1;
}

bool Board::isPseudoLegal(const Move& move, bool enforceTurn) const {
    const int fx = move.getFromX(), fy = move.getFromY();
    const int tx = move.getToX(), ty = move.getToY();
    if (fx < 0 || fx >= 8 || fy < 0 || fy >= 8 || tx < 0 || tx >= 8 || ty < 0 || ty >= 8) return false;
    const int piece = board[fy][fx], target = board[ty][tx];
    if (piece == 0 || (target != 0 && (piece > 0) == (target > 0))) return false;
    if (absInt(target) == 6) return false;
    if (enforceTurn && ((whiteToMove && piece < 0) || (!whiteToMove && piece > 0))) return false;
    const int dx = tx - fx, dy = ty - fy, ax = absInt(dx), ay = absInt(dy);
    const int type = absInt(piece);

    if (type == 1) {
        const int direction = piece > 0 ? -1 : 1, start = piece > 0 ? 6 : 1;
        if (dx == 0 && target == 0 && dy == direction) return true;
        if (dx == 0 && target == 0 && fy == start && dy == 2 * direction && board[fy + direction][fx] == 0) return true;
        if (ax == 1 && dy == direction && target != 0) return true;
        return ax == 1 && dy == direction && target == 0 && tx == enPassantX && ty == enPassantY;
    }
    if (type == 2) return (ax == 1 && ay == 2) || (ax == 2 && ay == 1);
    if (type == 6 && ax <= 1 && ay <= 1 && (ax || ay)) return true;
    if (type == 6 && fx == 4 && dy == 0 && ax == 2 && fy == (piece > 0 ? 7 : 0)) {
        const bool kingSide = dx > 0;
        const bool allowed = piece > 0 ? (kingSide ? whiteKingSide : whiteQueenSide)
                                       : (kingSide ? blackKingSide : blackQueenSide);
        const int rookX = kingSide ? 7 : 0;
        if (!allowed || board[fy][rookX] != (piece > 0 ? 4 : -4)) return false;
        const int step = kingSide ? 1 : -1;
        for (int x = fx + step; x != rookX; x += step) if (board[fy][x] != 0) return false;
        if (isSquareAttacked(fx, fy, piece < 0) || isSquareAttacked(fx + step, fy, piece < 0)) return false;
        return true;
    }
    if (type == 3 || type == 4 || type == 5) {
        const bool straight = dx == 0 || dy == 0, diagonal = ax == ay;
        if ((type == 3 && !diagonal) || (type == 4 && !straight) || (type == 5 && !(straight || diagonal))) return false;
        const int sx = dx == 0 ? 0 : (dx > 0 ? 1 : -1), sy = dy == 0 ? 0 : (dy > 0 ? 1 : -1);
        for (int x = fx + sx, y = fy + sy; x != tx || y != ty; x += sx, y += sy)
            if (board[y][x] != 0) return false;
        return true;
    }
    return false;
}

bool Board::isValidMove(const Move& move) const { return isMoveLegal(move); }

bool Board::isMoveLegal(const Move& move) const {
    if (!isPseudoLegal(move, true)) return false;
    Board next = *this;
    next.updateBoard(move);
    return !next.isInCheck(whiteToMove);
}

void Board::makeMove(const Move& move) {
    if (!isMoveLegal(move)) throw std::invalid_argument("Invalid chess move");
    updateBoard(move);
}

void Board::updateBoard(const Move& move) {
    const int fx = move.getFromX(), fy = move.getFromY(), tx = move.getToX(), ty = move.getToY();
    const int piece = board[fy][fx];
    const int type = absInt(piece);
    if (type == 1 && tx == enPassantX && ty == enPassantY && board[ty][tx] == 0)
        board[ty - (piece > 0 ? -1 : 1)][tx] = 0;
    board[ty][tx] = piece;
    board[fy][fx] = 0;

    if (type == 6 && absInt(tx - fx) == 2) {
        const int rookX = tx > fx ? 7 : 0, rookTo = tx > fx ? tx - 1 : tx + 1;
        board[fy][rookTo] = board[fy][rookX]; board[fy][rookX] = 0;
    }
    if (type == 1 && (ty == 0 || ty == 7)) board[ty][tx] = piece > 0 ? 5 : -5;

    if (piece == 6) whiteKingSide = whiteQueenSide = false;
    if (piece == -6) blackKingSide = blackQueenSide = false;
    if (fx == 0 && fy == 7) whiteQueenSide = false;
    if (fx == 7 && fy == 7) whiteKingSide = false;
    if (fx == 0 && fy == 0) blackQueenSide = false;
    if (fx == 7 && fy == 0) blackKingSide = false;
    if (tx == 0 && ty == 7) whiteQueenSide = false;
    if (tx == 7 && ty == 7) whiteKingSide = false;
    if (tx == 0 && ty == 0) blackQueenSide = false;
    if (tx == 7 && ty == 0) blackKingSide = false;
    enPassantX = enPassantY = -1;
    if (type == 1 && absInt(ty - fy) == 2) { enPassantX = fx; enPassantY = (fy + ty) / 2; }
    whiteToMove = !whiteToMove;
}

bool Board::isSquareAttacked(int x, int y, bool byWhite) const {
    for (int fy = 0; fy < 8; ++fy) for (int fx = 0; fx < 8; ++fx) {
        const int piece = board[fy][fx];
        if (piece == 0 || (piece > 0) != byWhite) continue;
        const int dx = x - fx, dy = y - fy, ax = absInt(dx), ay = absInt(dy), type = absInt(piece);
        if (type == 1 && ax == 1 && dy == (byWhite ? -1 : 1)) return true;
        if (type == 2 && ((ax == 1 && ay == 2) || (ax == 2 && ay == 1))) return true;
        if (type == 6 && ax <= 1 && ay <= 1 && (ax || ay)) return true;
        if (type == 3 || type == 4 || type == 5) {
            const bool straight = dx == 0 || dy == 0, diagonal = ax == ay;
            if ((type == 3 && !diagonal) || (type == 4 && !straight) || (type == 5 && !(straight || diagonal))) continue;
            const int sx = dx == 0 ? 0 : (dx > 0 ? 1 : -1), sy = dy == 0 ? 0 : (dy > 0 ? 1 : -1);
            bool clear = true;
            for (int px = fx + sx, py = fy + sy; px != x || py != y; px += sx, py += sy)
                if (board[py][px] != 0) { clear = false; break; }
            if (clear) return true;
        }
    }
    return false;
}

bool Board::isInCheck(bool white) const {
    for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x)
        if (board[y][x] == (white ? 6 : -6)) return isSquareAttacked(x, y, !white);
    return true;
}

std::vector<Move> Board::generateLegalMoves(bool white) const {
    std::vector<Move> moves;
    Board position = *this;
    position.whiteToMove = white;
    for (int fy = 0; fy < 8; ++fy) for (int fx = 0; fx < 8; ++fx) {
        if (position.board[fy][fx] == 0 || (position.board[fy][fx] > 0) != white) continue;
        for (int ty = 0; ty < 8; ++ty) for (int tx = 0; tx < 8; ++tx) {
            Move move(fx, fy, tx, ty);
            if (position.isMoveLegal(move)) moves.push_back(move);
        }
    }
    return moves;
}

bool Board::isCheckmate(bool white) const { return white == whiteToMove && isInCheck(white) && generateLegalMoves(white).empty(); }
bool Board::isStalemate(bool white) const { return white == whiteToMove && !isInCheck(white) && generateLegalMoves(white).empty(); }
int Board::getPiece(int x, int y) const { return (x >= 0 && x < 8 && y >= 0 && y < 8) ? board[y][x] : 0; }
bool Board::isWhiteTurn() const { return whiteToMove; }

bool Board::isPuzzleSolution(const Move& move, int puzzleNumber) const {
    const int pattern = (puzzleNumber - 1) % 6;
    const int solutions[6][4] = {
        {7, 3, 7, 1}, {0, 3, 0, 1}, {4, 3, 4, 1},
        {4, 3, 4, 1}, {1, 3, 1, 1}, {5, 3, 5, 1}
    };
    return move.getFromX() == solutions[pattern][0] && move.getFromY() == solutions[pattern][1] &&
           move.getToX() == solutions[pattern][2] && move.getToY() == solutions[pattern][3];
}

std::string Board::getPositionKey() const {
    std::string key;
    for (const auto& row : board) for (int piece : row) key.push_back(static_cast<char>(piece + 'A'));
    key.push_back(whiteToMove ? 'w' : 'b');
    key.push_back(whiteKingSide ? 'K' : '-');
    key.push_back(whiteQueenSide ? 'Q' : '-');
    key.push_back(blackKingSide ? 'k' : '-');
    key.push_back(blackQueenSide ? 'q' : '-');
    key.push_back(static_cast<char>(enPassantX + '0'));
    key.push_back(static_cast<char>(enPassantY + '0'));
    return key;
}
