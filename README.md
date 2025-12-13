# Chess Game - Compilation and Play Guide

## Prerequisites

- **GCC Compiler**: Make sure you have GCC installed on your system
  - Windows: Install MinGW or use WSL (Windows Subsystem for Linux)
  - Linux: Usually pre-installed, or install via `sudo apt-get install gcc make`
  - macOS: Install Xcode Command Line Tools: `xcode-select --install`

- **Make**: Build automation tool (usually comes with GCC)

## Compilation Instructions

### Method 1: Using Make (Recommended)

1. **Open a terminal/command prompt** in the project directory:
   ```bash
   cd "D:\Kushal\School\C and algo\ChessGame\ChessGame"
   ```

2. **Clean previous builds** (optional):
   ```bash
   make clean
   ```

3. **Compile the game**:
   ```bash
   make
   ```
   This will create an executable named `ChessGame` (or `ChessGame.exe` on Windows)

4. **Compile and run in one command**:
   ```bash
   make run
   ```

### Method 2: Manual Compilation

If you don't have `make`, you can compile manually:

```bash
gcc -Wall -Wextra -Iinclude -std=c23 -c src/board.c -o build/board.o
gcc -Wall -Wextra -Iinclude -std=c23 -c src/check.c -o build/check.o
gcc -Wall -Wextra -Iinclude -std=c23 -c src/main.c -o build/main.o
gcc -Wall -Wextra -Iinclude -std=c23 -c src/pieces.c -o build/pieces.o
gcc -Wall -Wextra -Iinclude -std=c23 -c src/rules.c -o build/rules.o
gcc -Wall -Wextra -Iinclude -std=c23 -o ChessGame build/*.o
```

**Note**: Create the `build` directory first: `mkdir build`

## Running the Game

### Windows:
```bash
.\ChessGame.exe
```
or simply:
```bash
ChessGame
```

### Linux/macOS:
```bash
./ChessGame
```

## How to Play

### Starting the Game

When you run the game, you'll see a main menu:
```
=== Chess Game ===
1. New Game
2. Load Game
3. Quit
Enter choice:
```

- **Option 1**: Start a new game
- **Option 2**: Load a previously saved game
- **Option 3**: Exit the game

### Game Controls

#### Making a Move

1. **Select a piece**: Enter the square coordinates (e.g., `E2` or `e2`)
   - Format: Letter (A-H) followed by Number (1-8)
   - Example: `E2` selects the white pawn on E2

2. **Choose destination**: Enter where you want to move the piece
   - Same format: Letter + Number
   - Example: `E4` moves the piece to E4

3. **Special Commands**:
   - `save` or `s`: Access the save menu during your turn
   - `undo` or `u`: Cancel piece selection and go back

#### Board Display

The board shows:
- **Pieces**: Uppercase = White, Lowercase = Black
  - `K/k` = King, `Q/q` = Queen, `R/r` = Rook
  - `B/b` = Bishop, `N/n` = Knight, `P/p` = Pawn
- **Possible moves**: 
  - `*` = Valid move
  - `%` = Capture move

### Saving and Loading Games

#### Saving:
1. During your turn, type `save` or `s` when prompted to select a piece
2. Enter a name for your save file (e.g., `game1`)
3. The game will be saved to `saves/game1.txt`

#### Loading:
1. Choose option 2 from the main menu
2. View the list of available saves
3. Enter the save name (without `.txt` extension)
4. The game will load and continue from where you left off

### Game Features

- ✅ **Check Detection**: The game warns you when your king is in check
- ✅ **Checkmate Detection**: Game ends automatically on checkmate
- ✅ **Move Validation**: Prevents illegal moves and moves that put your own king in check
- ✅ **Save/Load**: Save your progress and continue later
- ✅ **Turn-based**: Alternates between White and Black players

### Example Gameplay

```
You play the White
    A   B   C   D   E   F   G   H
  +---+---+---+---+---+---+---+---+
1 | r | n | b | q | k | b | n | r | 1
  +---+---+---+---+---+---+---+---+
2 | p | p | p | p | p | p | p | p | 2
  +---+---+---+---+---+---+---+---+
...
Select a piece on the board? ('save' or 's' to access saves menu)
Piece: E2
Where do you want to put it now? ('undo' or 'u' to get back)
Destination: E4
Move OK.
```

### Tips

- Always check if your king is in check before making a move
- Use `undo` if you change your mind about a piece selection
- Save frequently using `save` command
- The game automatically detects checkmate and declares the winner

## Troubleshooting

### Compilation Errors

**Error: "gcc: command not found"**
- Install GCC compiler (see Prerequisites)

**Error: "make: command not found"**
- Install make tool or use manual compilation

**Error: "No such file or directory: build"**
- The makefile should create it automatically, but you can create it manually:
  ```bash
  mkdir build
  ```

### Runtime Issues

**Game crashes on startup**
- Make sure all source files are present in the `src/` directory
- Recompile with `make clean && make`

**Save files not working**
- The `saves/` directory will be created automatically
- Make sure you have write permissions in the game directory

## Project Structure

```
ChessGame/
├── src/
│   ├── main.c      # Entry point
│   ├── board.c     # Board display and initialization
│   ├── pieces.c    # Piece movement logic
│   ├── rules.c     # Game rules and user interaction
│   └── check.c     # Check and checkmate detection
├── include/
│   ├── board.h
│   ├── pieces.h
│   ├── rules.h
│   └── check.h
├── build/          # Compiled object files (created automatically)
├── saves/          # Save game files (created automatically)
├── makefile        # Build configuration
└── README.md       # This file
```

## Enjoy Playing!

Have fun playing chess! Remember, this is a two-player game, so you'll be playing both sides.

