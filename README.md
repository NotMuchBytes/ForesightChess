# ForesightChess

A graphical chess game written in C++ using [Raylib](https://www.raylib.com/). The player controls White and plays against a lightweight chess engine controlling Black.

## Features

- Raylib graphical interface
- Main menu with **Play White**, **Play Black**, and **Quit** buttons
- Bitmap chess-piece icons
- Mouse-based piece selection and movement
- Legal movement and captures
- Check, checkmate, and stalemate detection
- Castling, en passant, and automatic pawn promotion to queen
- Engine move announcements
- Red check and checkmate indicators
- Victory screen with restart support
- Alpha-beta engine search

## Requirements

- Windows 10 or newer
- Visual Studio Build Tools 2022 with **Desktop development with C++**
- CMake 3.14 or newer
- Internet connection for the first build; CMake downloads Raylib automatically

## Build and Run

Open PowerShell in the project folder:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug --target ForesightChess
.\build\Debug\ForesightChess.exe
```

The build automatically downloads Raylib and copies the piece icons beside the executable.

## Controls

- **PLAY WHITE**: Play as White
- **PLAY BLACK**: Play as Black; the engine makes the opening move
- **Mouse click**: Select a piece, then select its destination
- **QUIT**: Return to the main menu
- **R**: Restart after game over
- **QUIT** in the main menu: Exit the application

## Project Structure

```text
ForesightChess/
├── assets/       Chess-piece bitmap icons
├── include/      C++ header files
├── src/          Board, engine, renderer, and application code
├── tests/        Test sources
├── CMakeLists.txt
└── README.md
```

## Main Components

- `Board`: Stores the position and enforces chess rules.
- `Engine`: Searches legal moves using material evaluation and alpha-beta pruning.
- `Renderer`: Handles Raylib graphics, menus, icons, mouse input, and status screens.
- `main.cpp`: Runs the menu, game loop, engine turn, and victory screen.

## Notes

The engine currently uses material evaluation with alpha-beta search. The best next upgrades would be piece-square tables, mobility scoring, king safety, pawn-structure evaluation, move ordering, and a transposition table. These improvements should make the engine feel much smarter without requiring excessive search depth. Increasing the search depth can also improve play, but engine moves may take longer.
