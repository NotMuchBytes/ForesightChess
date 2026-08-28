#include "Move.h"

Move::Move(int fromX, int fromY, int toX, int toY, bool isSpecial)
    : fromX(fromX), fromY(fromY), toX(toX), toY(toY), specialMove(isSpecial) {}

int Move::getFromX() const { return fromX; }
int Move::getFromY() const { return fromY; }
int Move::getToX() const { return toX; }
int Move::getToY() const { return toY; }
bool Move::isSpecialMove() const { return specialMove; }