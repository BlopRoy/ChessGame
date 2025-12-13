# Quick Start Guide

## 🚀 Fastest Way to Play

### Step 1: Compile
```bash
make
```

### Step 2: Run
```bash
# Windows
.\ChessGame.exe

# Linux/macOS  
./ChessGame
```

Or do both at once:
```bash
make run
```

## 🎮 Quick Gameplay Guide

### Basic Move Format
- **Select piece**: `E2` (letter A-H, number 1-8)
- **Move to**: `E4` (same format)

### Special Commands
- `save` or `s` - Save game
- `undo` or `u` - Cancel selection

### Example Turn
```
Piece: E2          ← Select white pawn on E2
Destination: E4    ← Move it to E4
```

## 📋 Menu Options

1. **New Game** - Start fresh
2. **Load Game** - Continue saved game
3. **Quit** - Exit

## 💾 Saving Games

During your turn:
```
Piece: save
Enter save name: mygame
```

To load later, choose option 2 from main menu and enter `mygame`

## ⚠️ Important Notes

- **Check**: Game warns you when king is in check
- **Checkmate**: Game ends automatically
- **Illegal moves**: Automatically prevented
- **Two players**: You play both White and Black

## 🎯 Piece Notation

- **K/k** = King
- **Q/q** = Queen  
- **R/r** = Rook
- **B/b** = Bishop
- **N/n** = Knight
- **P/p** = Pawn

Uppercase = White, Lowercase = Black

## 🐛 Troubleshooting

**Won't compile?**
```bash
make clean
make
```

**Can't find executable?**
- Check if `ChessGame` (or `ChessGame.exe`) exists in project folder
- Make sure compilation completed without errors

**Game won't run?**
- Make sure you're in the correct directory
- Try: `./ChessGame` (Linux/Mac) or `ChessGame.exe` (Windows)

