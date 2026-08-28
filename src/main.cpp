#include "Board.h"
#include "Engine.h"
#include "Renderer.h"

int main() {
    Board board;
    Engine engine;
    Renderer renderer;
    bool inMenu = true;
    bool playerWhite = true;
    bool gameOver = false;
    const char* result = "";

    while (!renderer.shouldClose()) {
        if (inMenu) {
            renderer.renderMenu();
            const int action = renderer.menuAction();
            if (action == 1 || action == 2) {
                playerWhite = action == 1;
                board.reset();
                renderer.clearEngineMove();
                gameOver = false;
                inMenu = false;
            } else if (action == 3) {
                break;
            }
            continue;
        }

        if (gameOver) {
            renderer.renderGameOver(board, result);
            if (renderer.wantsRestart()) {
                board.reset();
                renderer.clearEngineMove();
                gameOver = false;
            } else if (renderer.quitClicked()) {
                inMenu = true;
            }
            continue;
        }

        if (board.isWhiteTurn() != playerWhite) {
            renderer.render(board);
            WaitTime(0.75);
            engine.setBoard(board);
            Move engineMove = engine.getBestMove(3);
            if (engineMove.getFromX() < 0) {
                result = "You win! Checkmate";
                gameOver = true;
                continue;
            }
            board.makeMove(engineMove);
            renderer.setEngineMove(engineMove);
        } else {
            renderer.render(board);
            if (renderer.quitClicked()) {
                inMenu = true;
                continue;
            }
            Move playerMove = renderer.getPlayerMove();
            if (playerMove.getFromX() < 0) continue;
            if (!board.isValidMove(playerMove)) continue;
            board.makeMove(playerMove);
        }

        const bool sideToMove = board.isWhiteTurn();
        if (board.isCheckmate(sideToMove)) {
            result = sideToMove == playerWhite ? "Engine wins - Checkmate" : "You win! Checkmate";
            gameOver = true;
        } else if (board.isStalemate(sideToMove)) {
            result = "Draw - Stalemate";
            gameOver = true;
        }
    }
    return 0;
}
