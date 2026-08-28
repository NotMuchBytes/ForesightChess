#ifndef MOVE_H
#define MOVE_H

class Move {
public:
    Move(int fromX = 0, int fromY = 0, int toX = 0, int toY = 0, bool isSpecial = false);
    
    int getFromX() const;
    int getFromY() const;
    int getToX() const;
    int getToY() const;
    bool isSpecialMove() const;

private:
    int fromX;
    int fromY;
    int toX;
    int toY;
    bool specialMove;
};

#endif // MOVE_H