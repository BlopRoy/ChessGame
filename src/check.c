#include "check.h"
#include "board.h"
#include "pieces.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Find the position of the king for a given color
void findKing(int color, int *kingX, int *kingY) {
    char kingChar = (color == 1) ? 'K' : 'k';
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x] == kingChar) {
                *kingX = x;
                *kingY = y;
                return;
            }
        }
    }
    // King not found (shouldn't happen in normal game)
    *kingX = -1;
    *kingY = -1;
}

// Check if a square is attacked by the opponent
// This function temporarily moves pieces to check if they can attack the target square
static int isSquareAttacked(int x, int y, int defendingColor) {
    int attackingColor = 1 - defendingColor;
    
    // Check all squares for attacking pieces
    for (int checkY = 0; checkY < 8; checkY++) {
        for (int checkX = 0; checkX < 8; checkX++) {
            char piece = board[checkY][checkX];
            
            // Skip empty squares and pieces of the defending color
            if (piece == ' ' || (defendingColor == 1 && isupper(piece)) || 
                (defendingColor == 0 && islower(piece))) {
                continue;
            }
            
            // This is an attacking piece, check if it can reach the target square
            // Temporarily calculate moves for this piece
            resetAll();
            pieces(checkX, checkY, attackingColor);
            
            // Check if the target square is in the valid moves
            if (isValidMove(x, y)) {
                return 1; // Square is attacked
            }
        }
    }
    
    return 0; // Square is not attacked
}

// Check if the king of given color is in check
int isKingInCheck(int color) {
    int kingX, kingY;
    findKing(color, &kingX, &kingY);
    
    if (kingX == -1 || kingY == -1) {
        return 0; // King not found, shouldn't happen
    }
    
    return isSquareAttacked(kingX, kingY, color);
}

// Check if a move would put the moving player's king in check
int wouldMovePutKingInCheck(int fromX, int fromY, int toX, int toY, int color) {
    // Save the current board state
    char savedPiece = board[toY][toX];
    char movingPiece = board[fromY][fromX];
    
    // Make the move temporarily
    board[toY][toX] = movingPiece;
    board[fromY][fromX] = ' ';
    
    // Check if king is in check after the move
    int inCheck = isKingInCheck(color);
    
    // Restore the board
    board[fromY][fromX] = movingPiece;
    board[toY][toX] = savedPiece;
    
    return inCheck;
}

// Check if the king can escape check by moving
static int canKingEscape(int color) {
    int kingX, kingY;
    findKing(color, &kingX, &kingY);
    
    if (kingX == -1 || kingY == -1) {
        return 0;
    }
    
    // Get all possible moves for the king
    resetAll();
    pieces(kingX, kingY, color);
    
    // Check each possible move (check all squares around king)
    for (int newY = 0; newY < 8; newY++) {
        for (int newX = 0; newX < 8; newX++) {
            if (isValidMove(newX, newY)) {
                // Check if this move would get the king out of check
                if (!wouldMovePutKingInCheck(kingX, kingY, newX, newY, color)) {
                    return 1; // King can escape
                }
            }
        }
    }
    
    return 0; // King cannot escape
}

// Check if any piece can block the check or capture the attacking piece
static int canCheckBeBlocked(int color) {
    int attackingColor = 1 - color;
    int kingX, kingY;
    findKing(color, &kingX, &kingY);
    
    if (kingX == -1 || kingY == -1) {
        return 0;
    }
    
    // Find all attacking pieces
    for (int checkY = 0; checkY < 8; checkY++) {
        for (int checkX = 0; checkX < 8; checkX++) {
            char piece = board[checkY][checkX];
            
            // Skip empty squares and pieces of the defending color
            if (piece == ' ' || (color == 1 && isupper(piece)) || 
                (color == 0 && islower(piece))) {
                continue;
            }
            
            // This is an attacking piece, check if it attacks the king
            resetAll();
            pieces(checkX, checkY, attackingColor);
            
            if (isValidMove(kingX, kingY)) {
                // This piece attacks the king, try to capture it or block
                // Check all pieces of the defending color
                for (int defY = 0; defY < 8; defY++) {
                    for (int defX = 0; defX < 8; defX++) {
                        char defPiece = board[defY][defX];
                        
                        // Skip empty squares and enemy pieces
                        if (defPiece == ' ' || 
                            (color == 1 && !isupper(defPiece)) || 
                            (color == 0 && !islower(defPiece))) {
                            continue;
                        }
                        
                        // Get moves for this defending piece
                        resetAll();
                        pieces(defX, defY, color);
                        
                        // Check if it can capture the attacker
                        if (isValidMove(checkX, checkY)) {
                            // Try the move
                            if (!wouldMovePutKingInCheck(defX, defY, checkX, checkY, color)) {
                                return 1; // Can capture attacker
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

// Check if a king of given color is in checkmate
int isCheckmate(int color) {
    // First check if king is in check
    if (!isKingInCheck(color)) {
        return 0; // Not in check, so not checkmate
    }
    
    // Check if king can escape
    if (canKingEscape(color)) {
        return 0; // King can escape, not checkmate
    }
    
    // Check if check can be blocked or attacker captured
    if (canCheckBeBlocked(color)) {
        return 0; // Check can be blocked, not checkmate
    }
    
    // King is in check and cannot escape or be blocked
    return 1;
}

