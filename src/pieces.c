#include "pieces.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

#define INVALID -1

int tempLock[2] = {0, 0};

typedef struct {
    int x;
    int y;
    int occupied;   // 1 if a piece is on the square, 0 if empty
    int valid;      // 1 if the move is allowed, 0 if not
} Coordinate;

Coordinate tempValidMove[52];

void error(void) {
    printf("error");
}

void resetPosition(int position) {
    // Reset a specific move entry to INVALID (meaning: unused / invalid)
    tempValidMove[position].x = INVALID;
    tempValidMove[position].y = INVALID;
    tempValidMove[position].occupied = INVALID;
    tempValidMove[position].valid = INVALID;
}

void resetAll(void) {
    // Reset all stored moves
    for (int i = 0; i < 52; i++) {
        resetPosition(i);
    }
}

int pawnOrNot(int x,int y){
    // Returns 1 if the square contains *any* piece, 0 if empty
    if (board[x][y] != ' ') {
        return 1;
    }
    return 0;
}

/* Store the movement pointed by tempLock into tempValidMove */
int legalMove(int position) {

    // Store coordinates
    tempValidMove[position].x = tempLock[0];
    tempValidMove[position].y = tempLock[1];

    // Check if a piece is present on the target square
    if (pawnOrNot(tempLock[0], tempLock[1])) {

        // Uppercase = white piece
        if (isupper(board[tempLock[0]][tempLock[1]])) {
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 1;   // capturing allowed
        }
        // Lowercase = black piece
        else if (islower(board[tempLock[0]][tempLock[1]])) {
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 0;   // blocked by ally
        }

    } else {
        // Empty square = valid move
        tempValidMove[position].occupied = 0;
        tempValidMove[position].valid = 1;
    }

    return 1;
}

/* Movement helper functions:
   They update tempLock with the new coordinates.
*/

void forward(int x, int y, int step) {
    tempLock[0] = x;
    tempLock[1] = y + step;
}

void backward(int x, int y, int step) {
    tempLock[0] = x;
    tempLock[1] = y - step;
}

void left(int x, int y, int step) {
    tempLock[0] = x - step;
    tempLock[1] = y;
}

void right(int x, int y, int step) {
    tempLock[0] = x + step;
    tempLock[1] = y;
}

void diagonalLeftTop(int x, int y, int step) {
    tempLock[0] = x - step;
    tempLock[1] = y + step;
}

void diagonalRightTop(int x, int y, int step) {
    tempLock[0] = x + step;
    tempLock[1] = y + step;
}

void diagonalRightBottom(int x, int y, int step) {
    tempLock[0] = x + step;
    tempLock[1] = y - step;
}

void diagonalLeftBottom(int x, int y, int step) {
    tempLock[0] = x - step;
    tempLock[1] = y - step;
}

/* Pawns */

void blackPawn(int x, int y) {

    // Black pawns move downward (backward)
    backward(x, y, 1);

    // Store forward movement
    tempValidMove[0].x = tempLock[0];
    tempValidMove[0].y = tempLock[1];

    // Check if forward square is free or blocked
    if (pawnOrNot(tempLock[0], tempLock[1])) {
        tempValidMove[0].occupied = 1;
        tempValidMove[0].valid = 0;    // can't move forward if occupied
    }
    else {
        tempValidMove[0].occupied = 0;
        tempValidMove[0].valid = 1;
    }

    // Capture left
    diagonalLeftBottom(x, y, 1);
    legalMove(1);

    // Capture right
    diagonalRightBottom(x, y, 1);
    legalMove(2);
}

void whitePawn(int x, int y) {

    // White pawns move upward (forward)
    forward(x, y, 1);

    // Store forward movement
    tempValidMove[0].x = tempLock[0];
    tempValidMove[0].y = tempLock[1];

    // Check forward availability
    if (pawnOrNot(tempLock[0], tempLock[1])) {
        tempValidMove[0].occupied = 1;
        tempValidMove[0].valid = 0;
    }
    else {
        tempValidMove[0].occupied = 0;
        tempValidMove[0].valid = 1;
    }

    // Capture left
    diagonalLeftTop(x, y, 1);
    legalMove(1);

    // Capture right
    diagonalRightTop(x, y, 1);
    legalMove(2);
}

/* Knights */

void blackAndWhiteKnight(int x, int y) {
    /* A knight has 8 possible movements */

    forward(x, y, 2);
    left(x, y, 1);
    legalMove(0);
    
    forward(x, y, 2);
    right(x, y, 1);
    legalMove(1);

    backward(x, y, 2);
    left(x, y, 1);
    legalMove(2);

    backward(x, y, 2);
    right(x, y, 1);
    legalMove(3);

    right(x, y, 2);
    backward(x, y, 1);
    legalMove(4);

    right(x, y, 2);
    forward(x, y, 1);
    legalMove(5);

    left(x, y, 2);
    backward(x, y, 1);
    legalMove(6);

    left(x, y, 2);
    forward(x, y, 1);
    legalMove(7);
}

/* Bishops */

void blackAndWhiteBishop(int x, int y) {
    int pos = -1;

    // A bishop moves diagonally up to 7 squares
    for (int i = 1; i <= 7; i++) {
        diagonalLeftBottom(x, y, i);
        legalMove(++pos);

        diagonalLeftTop(x, y, i);
        legalMove(++pos);
        
        diagonalRightBottom(x, y, i);
        legalMove(++pos);
        
        diagonalRightTop(x, y, i);
        legalMove(++pos);
    }
}

/* Rooks */

void blackAndWhiteRook(int x, int y) {
    int pos = -1;

    // A rook moves horizontally and vertically up to 7 squares
    for (int i = 1; i <= 7; i++) {
        forward(x, y, i);
        legalMove(++pos);

        backward(x, y, i);
        legalMove(++pos);

        right(x, y, i);
        legalMove(++pos);

        left(x, y, i);
        legalMove(++pos);
    }
}

/* Queens */

void blackAndWhiteQueen(int x, int y) {
    int pos = -1;

    // A queen combines rook + bishop movement
    for (int i = 1; i <= 7; i++) {

        /* Diagonals */
        diagonalLeftBottom(x, y, i);
        legalMove(++pos);
        
        diagonalLeftTop(x, y, i);
        legalMove(++pos);
        
        diagonalRightBottom(x, y, i);
        legalMove(++pos);
        
        diagonalRightTop(x, y, i);
        legalMove(++pos);

        /* Straight lines */
        forward(x, y, i);
        legalMove(++pos);
        
        backward(x, y, i);
        legalMove(++pos);
        
        right(x, y, i);
        legalMove(++pos);
        
        left(x, y, i);
        legalMove(++pos);
    }
}

/* Kings */

void blackAndWhiteKing(int x, int y) {
    int pos = 0;

    // King moves 1 square in any direction
    diagonalLeftBottom(x, y, 1);
    legalMove(pos++);
    
    diagonalLeftTop(x, y, 1);
    legalMove(pos++);
    
    diagonalRightBottom(x, y, 1);
    legalMove(pos++);
    
    diagonalRightTop(x, y, 1);
    legalMove(pos++);

    forward(x, y, 1);
    legalMove(pos++);
    
    backward(x, y, 1);
    legalMove(pos++);
    
    right(x, y, 1);
    legalMove(pos++);
    
    left(x, y, 1);
    legalMove(pos++);
}

/* Discard moves outside 0–7 board limits */

void isInTheMatrix(void) {
    for (int i = 0; i < 52; i++) {
        // Remove move if outside chessboard limits
        if (tempValidMove[i].x < 0 || tempValidMove[i].y < 0 ||
            tempValidMove[i].x > 7 || tempValidMove[i].y > 7) {
            resetPosition(i);
        }
    }
}

/* Check if (x,y) matches any stored valid coordinate */

int isIn(int x, int y) {
    for (int i = 0; i < 52; i++) {
        if (x == tempValidMove[i].x && y == tempValidMove[i].y) {
            return i;   // return index if found
        }
    }
    return -1;
}

int action(int x, int y) {
    // Determine what kind of action happens on square (x,y)

    int position = isIn(x, y);

    if (position != -1) {
        if (tempValidMove[position].valid) {
            if (tempValidMove[position].occupied)
                return 2; // capture move
            return 1;     // normal move
        }

        else if (tempValidMove[position].occupied)
            return 3; // blocked by own piece

        return 0;       // invalid move
    }

    return -1;          // not part of possible moves
}

/* Public functions: calculate moves for each piece */

int pawn(int x, int y) {
    resetAll();
    if (isupper(board[x][y])) {
        whitePawn(x, y);
    }
    else {
        blackPawn(x, y);
    }
    isInTheMatrix();
    return action(x, y);
}

int knight(int x, int y) {
    resetAll();
    blackAndWhiteKnight(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Bishop(int x, int y) {
    resetAll();
    blackAndWhiteBishop(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Rook(int x, int y) {
    resetAll();
    blackAndWhiteRook(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Queen(int x, int y) {
    resetAll();
    blackAndWhiteQueen(x, y);
    isInTheMatrix();
    return action(x, y);
}

int King(int x, int y) {
    resetAll();
    blackAndWhiteKing(x, y);
    isInTheMatrix();
    return action(x, y);
}
