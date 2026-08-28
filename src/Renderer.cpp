#include "Renderer.h"
#include <string>

Renderer::Renderer() : selectedX(-1), selectedY(-1), lastEngineMove(), hasEngineMove(false) {
	InitWindow(boardSize, boardSize + 50, "ForesightChess");
	const char names[] = "pnbrqk";
	for (int type = 1; type <= 6; ++type) {
		std::string whitePath = std::string("assets/w") + names[type - 1] + ".bmp";
		std::string blackPath = std::string("assets/b") + names[type - 1] + ".bmp";
		pieceIcons[0][type] = LoadTexture(whitePath.c_str());
		pieceIcons[1][type] = LoadTexture(blackPath.c_str());
	}
	SetTargetFPS(60);
}

Renderer::~Renderer() {
	for (int side = 0; side < 2; ++side) for (int type = 1; type <= 6; ++type) UnloadTexture(pieceIcons[side][type]);
	CloseWindow();
}
bool Renderer::shouldClose() const { return WindowShouldClose(); }

void Renderer::renderMenu() const {
	BeginDrawing();
	ClearBackground(Color{25, 30, 42, 255});
	DrawText("FORESIGHT CHESS", 320 - MeasureText("FORESIGHT CHESS", 42) / 2, 120, 42, GOLD);
	DrawText("A chess game against the engine", 320 - MeasureText("A chess game against the engine", 20) / 2, 180, 20, LIGHTGRAY);
	DrawRectangle(200, 250, 240, 55, Color{55, 125, 75, 255});
	DrawText("PLAY WHITE", 320 - MeasureText("PLAY WHITE", 24) / 2, 265, 24, RAYWHITE);
	DrawRectangle(200, 320, 240, 55, Color{75, 95, 145, 255});
	DrawText("PLAY BLACK", 320 - MeasureText("PLAY BLACK", 24) / 2, 335, 24, RAYWHITE);
	DrawRectangle(200, 390, 240, 55, Color{150, 55, 55, 255});
	DrawText("QUIT", 320 - MeasureText("QUIT", 24) / 2, 405, 24, RAYWHITE);
	DrawText("Choose your side", 320 - MeasureText("Choose your side", 18) / 2, 490, 18, GRAY);
	EndDrawing();
}

int Renderer::menuAction() const {
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return 0;
	const Vector2 p = GetMousePosition();
	if (p.x >= 200 && p.x <= 440 && p.y >= 250 && p.y <= 305) return 1;
	if (p.x >= 200 && p.x <= 440 && p.y >= 320 && p.y <= 375) return 2;
	if (p.x >= 200 && p.x <= 440 && p.y >= 390 && p.y <= 445) return 3;
	return 0;
}

bool Renderer::quitClicked() const {
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return false;
	const Vector2 p = GetMousePosition();
	return p.x >= 535 && p.x <= 625 && p.y >= 642 && p.y <= 672;
}

void Renderer::render(const Board& board) const {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	const Color light = {240, 217, 181, 255}, dark = {181, 136, 99, 255};
	for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
		DrawRectangle(x * squareSize, y * squareSize, squareSize, squareSize, (x + y) % 2 ? dark : light);
		if (x == selectedX && y == selectedY) DrawRectangleLinesEx({x * 80.0f, y * 80.0f, 80, 80}, 4, YELLOW);
		const int piece = board.getPiece(x, y), type = piece < 0 ? -piece : piece;
		if (type > 0 && type < 7) {
			const int side = piece > 0 ? 0 : 1;
			DrawTexturePro(pieceIcons[side][type], {0, 0, 64, 64},
						   {x * 80.0f + 8, y * 80.0f + 8, 64, 64}, {0, 0}, 0, WHITE);
		}
	}
	std::string status = board.isWhiteTurn() ? "White to move - click a piece, then its destination"
											 : "Engine is thinking...";
	if (board.isWhiteTurn() && hasEngineMove) {
		status = "Engine moved " + std::string(1, static_cast<char>('a' + lastEngineMove.getFromX()))
			   + std::to_string(8 - lastEngineMove.getFromY()) + " to "
			   + std::string(1, static_cast<char>('a' + lastEngineMove.getToX()))
			   + std::to_string(8 - lastEngineMove.getToY()) + " - your turn";
	}
	DrawText(status.c_str(), 10, 645, 18, DARKGRAY);
	DrawRectangle(535, 642, 90, 28, Color{150, 55, 55, 255});
	DrawText("QUIT", 553, 647, 18, WHITE);
	if (board.isInCheck(board.isWhiteTurn())) {
		DrawRectangle(500, 642, 125, 28, RED);
		DrawText("CHECK!", 515, 646, 20, WHITE);
	}
	EndDrawing();
}

void Renderer::renderGameOver(const Board& board, const char* message) const {
	render(board);
	BeginDrawing();
	const bool checkmate = std::string(message).find("Checkmate") != std::string::npos;
	DrawRectangle(100, 220, 440, 190, Color{20, 25, 35, 235});
	DrawRectangleLinesEx({100, 220, 440, 190}, 4, checkmate ? RED : GOLD);
	DrawText(message, 320 - MeasureText(message, 42) / 2, 255, 42, checkmate ? RED : GOLD);
	DrawText("Press R to play again", 320 - MeasureText("Press R to play again", 22) / 2, 325, 22, RAYWHITE);
	DrawText("Close the window to exit", 320 - MeasureText("Close the window to exit", 18) / 2, 360, 18, LIGHTGRAY);
	EndDrawing();
}

bool Renderer::wantsRestart() const { return IsKeyPressed(KEY_R); }
void Renderer::setEngineMove(const Move& move) { lastEngineMove = move; hasEngineMove = true; }
void Renderer::clearEngineMove() { hasEngineMove = false; }

Move Renderer::getPlayerMove() const {
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return Move(-1, -1, -1, -1);
	const Vector2 mouse = GetMousePosition();
	const int x = static_cast<int>(mouse.x) / squareSize, y = static_cast<int>(mouse.y) / squareSize;
	if (x < 0 || x >= 8 || y < 0 || y >= 8) return Move(-1, -1, -1, -1);
	if (selectedX < 0) { selectedX = x; selectedY = y; return Move(-1, -1, -1, -1); }
	Move move(selectedX, selectedY, x, y);
	selectedX = selectedY = -1;
	return move;
}