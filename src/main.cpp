#include "Board.h"
#include "Engine.h"
#include "Renderer.h"

int main() {
    Board board;
    Engine engine;
    Renderer renderer;
    bool inMenu = true;
    bool playerWhite = true;
    bool puzzleMode = false;
    bool puzzleOver = false;
    int puzzleNumber = 1;
    const char* puzzleMessage = "Find the winning move";
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
                puzzleMode = false;
                inMenu = false;
            } else if (action == 3) {
                board.setupPuzzle(1);
                puzzleNumber = 1;
                renderer.setPuzzleNumber(puzzleNumber);
                renderer.resetPuzzleIntro();
                puzzleMode = true;
                puzzleOver = false;
                puzzleMessage = "Find the winning move";
                inMenu = false;
            } else if (action == 4) {
                break;
            }
            continue;
        }

        if (puzzleMode) {
            if (puzzleOver) {
                renderer.renderPuzzle(board, puzzleMessage);
                if (renderer.wantsRestart()) board.setupPuzzle(puzzleNumber), renderer.resetPuzzleIntro(), puzzleOver = false, puzzleMessage = "Find the winning move";
                else if (renderer.wantsNextPuzzle()) {
                    puzzleNumber = puzzleNumber % 365 + 1;
                    board.setupPuzzle(puzzleNumber);
                    renderer.setPuzzleNumber(puzzleNumber);
                    renderer.resetPuzzleIntro();
                    puzzleOver = false;
                    puzzleMessage = "Find the winning move";
                }
                else if (renderer.quitClicked()) puzzleMode = false, inMenu = true;
                continue;
            }
            renderer.renderPuzzle(board, puzzleMessage);
            if (renderer.quitClicked()) { puzzleMode = false; inMenu = true; continue; }
            Move move = renderer.getPlayerMove();
            if (move.getFromX() < 0) continue;
            const bool correct = board.isPuzzleSolution(move, puzzleNumber);
            if (correct && board.isValidMove(move)) {
                board.makeMove(move);
                if (board.isCheckmate(false)) puzzleMessage = "Correct! Press N for the next puzzle";
                else puzzleMessage = "Check, but not checkmate - press R to try again";
            } else {
                puzzleMessage = "Not the solution - press R to try again";
            }
            puzzleOver = true;
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

        const bool currentSide = board.isWhiteTurn();
        if (board.isCheckmate(currentSide)) {
            result = currentSide == playerWhite ? "Engine wins - Checkmate" : "You win! Checkmate";
            gameOver = true;
            continue;
        }
        if (board.isStalemate(currentSide)) {
            result = "Draw - Stalemate";
            gameOver = true;
            continue;
        }

        if (board.isWhiteTurn() != playerWhite) {
            renderer.render(board);
            WaitTime(0.75);
            engine.setBoard(board);
            Move engineMove = engine.getBestMove(3);
            if (engineMove.getFromX() < 0) continue;
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
