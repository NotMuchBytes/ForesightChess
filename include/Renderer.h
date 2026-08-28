#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"
#include "Move.h"
#include "raylib.h"
#include <array>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool shouldClose() const;
    void renderMenu() const;
    int menuAction() const;
    void renderPuzzle(const Board& board, const char* message) const;
    bool quitClicked() const;
    void render(const Board& board) const;
    void renderGameOver(const Board& board, const char* message) const;
    bool wantsRestart() const;
    bool wantsNextPuzzle() const;
    void setPuzzleNumber(int number);
    void setEngineMove(const Move& move);
    void clearEngineMove();
    void resetPuzzleIntro();
    Move getPlayerMove() const;

private:
    mutable int selectedX;
    mutable int selectedY;
    std::array<std::array<Texture2D, 7>, 2> pieceIcons;
    Move lastEngineMove;
    bool hasEngineMove;
    mutable std::string puzzleOverlayMessage;
    mutable double puzzleIntroUntil;
    mutable int puzzleNumber;
    static constexpr int boardSize = 640;
    static constexpr int squareSize = 80;
};

#endif // RENDERER_H
